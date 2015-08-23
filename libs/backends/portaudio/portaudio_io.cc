/*
 * Copyright (C) 2015 Robin Gareus <robin@gareus.org>
 * Copyright (C) 2015 Tim Mayberry <mojofunk@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <glibmm.h>

#include "portaudio_io.h"

#ifdef WITH_ASIO
#include "pa_asio.h"
#endif

#include "pbd/compose.h"

#include "ardour/audio_backend.h"

#include "debug.h"

#define INTERLEAVED_INPUT
#define INTERLEAVED_OUTPUT

using namespace PBD;
using namespace ARDOUR;

PortAudioIO::PortAudioIO ()
	: _state (-1)
	, _initialized (false)
	, _capture_channels (0)
	, _playback_channels (0)
	, _stream (0)
	, _input_buffer (0)
	, _output_buffer (0)
	, _cur_sample_rate (0)
	, _cur_input_latency (0)
	, _cur_output_latency (0)
	, _host_api_index(-1)
{
}

PortAudioIO::~PortAudioIO ()
{
	if (_state == 0) {
		pcm_stop();
	}
	if (_initialized) {
		Pa_Terminate();
	}

	clear_device_lists ();

	free (_input_buffer); _input_buffer = NULL;
	free (_output_buffer); _output_buffer = NULL;
}

std::string
PortAudioIO::control_app_name (int device_id) const
{
#ifdef WITH_ASIO
	if (get_current_host_api_type() == paASIO) {
		// is this used for anything, or just acts as a boolean?
		return "PortaudioASIO";
	}
#endif

	return std::string();
}

void
PortAudioIO::launch_control_app (int device_id)
{
#ifdef WITH_ASIO
	PaError err = PaAsio_ShowControlPanel (device_id, NULL);

	if (err != paNoError) {
		// error << ?
		DEBUG_AUDIO (string_compose (
		    "Unable to show control panel for device with index %1\n", device_id));
	}
#endif
}

void
PortAudioIO::get_default_sample_rates (std::vector<float>& rates)
{
	rates.push_back(8000.0);
	rates.push_back(22050.0);
	rates.push_back(24000.0);
	rates.push_back(44100.0);
	rates.push_back(48000.0);
	rates.push_back(88200.0);
	rates.push_back(96000.0);
	rates.push_back(176400.0);
	rates.push_back(192000.0);
}

int
PortAudioIO::available_sample_rates(int device_id, std::vector<float>& sampleRates)
{
	if (!initialize_pa()) return -1;

	// TODO use  separate int device_input, int device_output ?!
	if (device_id == DeviceDefault) {
		device_id = get_default_input_device ();
	}

	DEBUG_AUDIO (
	    string_compose ("Querying Samplerates for device %1\n", device_id));

	sampleRates.clear();
	const PaDeviceInfo* nfo = Pa_GetDeviceInfo(device_id);

	if (nfo) {
		PaStreamParameters inputParam;
		PaStreamParameters outputParam;

		inputParam.device = device_id;
		inputParam.channelCount = nfo->maxInputChannels;
		inputParam.sampleFormat = paFloat32;
		inputParam.suggestedLatency = 0;
		inputParam.hostApiSpecificStreamInfo = 0;

		outputParam.device = device_id;
		outputParam.channelCount = nfo->maxOutputChannels;
		outputParam.sampleFormat = paFloat32;
		outputParam.suggestedLatency = 0;
		outputParam.hostApiSpecificStreamInfo = 0;

		std::vector<float> rates;
		get_default_sample_rates(rates);

		for (std::vector<float>::const_iterator i = rates.begin(); i != rates.end();
		     ++i) {
			if (paFormatIsSupported ==
			    Pa_IsFormatSupported(nfo->maxInputChannels > 0 ? &inputParam : NULL,
			                         nfo->maxOutputChannels > 0 ? &outputParam : NULL,
			                         *i)) {
				sampleRates.push_back(*i);
			}
		}
	}

	if (sampleRates.empty()) {
		// fill in something..
		get_default_sample_rates(sampleRates);
	}

	return 0;
}

#ifdef WITH_ASIO
bool
PortAudioIO::get_asio_buffer_properties (int device_id,
                                         long& min_size_frames,
                                         long& max_size_frames,
                                         long& preferred_size_frames,
                                         long& granularity)
{
	// we shouldn't really need all these checks but it shouldn't hurt
	const PaDeviceInfo* device_info = Pa_GetDeviceInfo(device_id);

	if (!device_info) {
		DEBUG_AUDIO (string_compose (
		    "Unable to get device info from device index %1\n", device_id));
		return false;
	}

	if (get_current_host_api_type() != paASIO) {
		DEBUG_AUDIO (string_compose (
		    "ERROR device_id %1 is not an ASIO device\n", device_id));
		return false;
	}

	PaError err = PaAsio_GetAvailableBufferSizes (device_id,
	                                              &min_size_frames,
	                                              &max_size_frames,
	                                              &preferred_size_frames,
	                                              &granularity);

	if (err != paNoError) {
		DEBUG_AUDIO (string_compose (
		    "Unable to determine available buffer sizes for device %1\n", device_id));
		return false;
	}
	return true;
}

bool
PortAudioIO::get_asio_buffer_sizes (int device_id, std::vector<uint32_t>& buffer_sizes)
{
	long min_size_frames = 0;
	long max_size_frames = 0;
	long preferred_size_frames = 0;
	long granularity = 0;

	if (!get_asio_buffer_properties (device_id,
	                                 min_size_frames,
	                                 max_size_frames,
	                                 preferred_size_frames,
	                                 granularity)) {
		DEBUG_AUDIO (string_compose (
		    "Unable to get device buffer properties from device index %1\n", device_id));
		return false;
	}

	DEBUG_AUDIO (string_compose ("ASIO buffer properties for device %1, "
	                             "min_size_frames: %2, max_size_frames: %3, "
	                             "preferred_size_frames: %4, granularity: %5\n",
	                             device_id,
	                             min_size_frames,
	                             max_size_frames,
	                             preferred_size_frames,
	                             granularity));

#ifdef USE_ASIO_MIN_MAX_BUFFER_SIZES
	if (min_size_frames >= max_size_frames) {
		buffer_sizes.push_back (preferred_size_frames);
		return true;
	}

	long buffer_size = min_size_frames;
	while (buffer_size <= max_size_frames) {
		buffer_sizes.push_back (buffer_size);

		if (granularity <= 0) {
			// buffer sizes are power of 2
			buffer_size = buffer_size * 2;
		} else {
			buffer_size += granularity;
		}
	}
#else
	buffer_sizes.push_back (preferred_size_frames);
#endif
	return true;
}
#endif

void
PortAudioIO::get_default_buffer_sizes(std::vector<uint32_t>& buffer_sizes)
{
	buffer_sizes.push_back(64);
	buffer_sizes.push_back(128);
	buffer_sizes.push_back(256);
	buffer_sizes.push_back(512);
	buffer_sizes.push_back(1024);
	buffer_sizes.push_back(2048);
	buffer_sizes.push_back(4096);
}

int
PortAudioIO::available_buffer_sizes(int device_id, std::vector<uint32_t>& buffer_sizes)
{
#ifdef WITH_ASIO
	if (get_current_host_api_type() == paASIO) {
		if (get_asio_buffer_sizes (device_id, buffer_sizes)) {
			return 0;
		}
	}
#endif

	get_default_buffer_sizes (buffer_sizes);

	return 0;
}

void
PortAudioIO::input_device_list(std::map<int, std::string> &devices) const
{
	for (std::map<int, paDevice*>::const_iterator i = _input_devices.begin ();
	     i != _input_devices.end ();
	     ++i) {
		devices.insert (std::pair<int, std::string>(i->first, Glib::locale_to_utf8(i->second->name)));
	}
}

void
PortAudioIO::output_device_list(std::map<int, std::string> &devices) const
{
	for (std::map<int, paDevice*>::const_iterator i = _output_devices.begin ();
	     i != _output_devices.end ();
	     ++i) {
		devices.insert (std::pair<int, std::string>(i->first, Glib::locale_to_utf8(i->second->name)));
	}
}

bool
PortAudioIO::initialize_pa ()
{
	PaError err = paNoError;

	if (!_initialized) {
		err = Pa_Initialize();
		if (err != paNoError) {
			return false;
		}
		_initialized = true;
		_host_api_index = Pa_GetDefaultHostApi ();
		_host_api_name = get_host_api_name_from_index (_host_api_index);
	}

	return true;
}

void
PortAudioIO::host_api_list (std::vector<std::string>& api_list)
{
	if (!initialize_pa()) return;

	PaHostApiIndex count = Pa_GetHostApiCount();

	if (count < 0) return;

	for (int i = 0; i < count; ++i) {
		const PaHostApiInfo* info = Pa_GetHostApiInfo (i);
		if (info->name != NULL) { // possible?
			api_list.push_back (info->name);
		}
	}
}


PaHostApiTypeId
PortAudioIO::get_current_host_api_type () const
{
	const PaHostApiInfo* info = Pa_GetHostApiInfo (_host_api_index);

	if (info == NULL) {
		DEBUG_AUDIO(string_compose(
		    "Unable to determine Host API type from index %1\n", _host_api_index));
		return (PaHostApiTypeId)0;
	}

	return info->type;
}

std::string
PortAudioIO::get_host_api_name_from_index (PaHostApiIndex index)
{
	std::vector<std::string> api_list;
	host_api_list(api_list);
	return api_list[index];
}

bool
PortAudioIO::set_host_api (const std::string& host_api_name)
{
	PaHostApiIndex new_index = get_host_api_index_from_name (host_api_name);

	if (new_index < 0) {
		DEBUG_AUDIO ("Portaudio: Error setting host API\n");
		return false;
	}
	_host_api_index = new_index;
	_host_api_name = host_api_name;
	return true;
}

PaHostApiIndex
PortAudioIO::get_host_api_index_from_name (const std::string& name)
{
	if (!initialize_pa()) return -1;

	PaHostApiIndex count = Pa_GetHostApiCount();

	if (count < 0) {
		DEBUG_AUDIO ("Host API count < 0\n");
		return -1;
	}

	for (int i = 0; i < count; ++i) {
		const PaHostApiInfo* info = Pa_GetHostApiInfo (i);
		if (info != NULL && info->name != NULL) { // possible?
			if (name == info->name) {
				return i;
			}
		}
	}
	DEBUG_AUDIO (string_compose ("Unable to get host API from name: %1\n", name));

	return -1;
}

PaDeviceIndex
PortAudioIO::get_default_input_device ()
{
	const PaHostApiInfo* info = Pa_GetHostApiInfo (_host_api_index);
	if (info == NULL) return -1;
	return info->defaultInputDevice;
}

PaDeviceIndex
PortAudioIO::get_default_output_device ()
{
	const PaHostApiInfo* info = Pa_GetHostApiInfo (_host_api_index);
	if (info == NULL) return -1;
	return info->defaultOutputDevice;
}

void
PortAudioIO::clear_device_lists ()
{
	for (std::map<int, paDevice*>::const_iterator i = _input_devices.begin (); i != _input_devices.end(); ++i) {
		delete i->second;
	}
	_input_devices.clear();

	for (std::map<int, paDevice*>::const_iterator i = _output_devices.begin (); i != _output_devices.end(); ++i) {
		delete i->second;
	}
	_output_devices.clear();
}

void
PortAudioIO::add_none_devices ()
{
	_input_devices.insert(std::pair<int, paDevice*>(
	    DeviceNone, new paDevice(AudioBackend::get_standard_device_name(AudioBackend::DeviceNone), 0, 0)));
	_output_devices.insert(std::pair<int, paDevice*>(
	    DeviceNone, new paDevice(AudioBackend::get_standard_device_name(AudioBackend::DeviceNone), 0, 0)));
}

void
PortAudioIO::add_default_devices ()
{
	const PaHostApiInfo* info = Pa_GetHostApiInfo (_host_api_index);
	if (info == NULL) return;

	const PaDeviceInfo* nfo_i = Pa_GetDeviceInfo(get_default_input_device());
	const PaDeviceInfo* nfo_o = Pa_GetDeviceInfo(get_default_output_device());
	if (nfo_i && nfo_o) {
		_input_devices.insert (std::pair<int, paDevice*> (DeviceDefault,
					new paDevice(AudioBackend::get_standard_device_name(AudioBackend::DeviceDefault),
						nfo_i->maxInputChannels,
						nfo_o->maxOutputChannels
						)));
		_output_devices.insert (std::pair<int, paDevice*> (DeviceDefault,
					new paDevice(AudioBackend::get_standard_device_name(AudioBackend::DeviceDefault),
						nfo_i->maxInputChannels,
						nfo_o->maxOutputChannels
						)));
	}
}

void
PortAudioIO::add_devices ()
{
	const PaHostApiInfo* info = Pa_GetHostApiInfo (_host_api_index);
	if (info == NULL) return;

	int n_devices = Pa_GetDeviceCount();

	DEBUG_AUDIO (string_compose ("PortAudio found %1 devices\n", n_devices));

	for (int i = 0 ; i < n_devices; ++i) {
		const PaDeviceInfo* nfo = Pa_GetDeviceInfo(i);

		if (!nfo) continue;
		if (nfo->hostApi != _host_api_index) continue;

		DEBUG_AUDIO (string_compose (" (%1) '%2' '%3' in: %4 (lat: %5 .. %6) out: %7 "
		                             "(lat: %8 .. %9) sr:%10\n",
		                             i,
		                             info->name,
		                             nfo->name,
		                             nfo->maxInputChannels,
		                             nfo->defaultLowInputLatency * 1e3,
		                             nfo->defaultHighInputLatency * 1e3,
		                             nfo->maxOutputChannels,
		                             nfo->defaultLowOutputLatency * 1e3,
		                             nfo->defaultHighOutputLatency * 1e3,
		                             nfo->defaultSampleRate));

		if ( nfo->maxInputChannels == 0 && nfo->maxOutputChannels == 0) {
			continue;
		}

		if (nfo->maxInputChannels > 0) {
			_input_devices.insert (std::pair<int, paDevice*> (i, new paDevice(
							nfo->name,
							nfo->maxInputChannels,
							nfo->maxOutputChannels
							)));
		}
		if (nfo->maxOutputChannels > 0) {
			_output_devices.insert (std::pair<int, paDevice*> (i, new paDevice(
							nfo->name,
							nfo->maxInputChannels,
							nfo->maxOutputChannels
							)));
		}
	}
}

void
PortAudioIO::discover()
{
	DEBUG_AUDIO ("PortAudio: discover\n");
	if (!initialize_pa()) return;

	clear_device_lists ();
	add_none_devices ();
	add_devices ();
}

void
PortAudioIO::pcm_stop ()
{
	if (_stream) {
		Pa_CloseStream (_stream);
	}
	_stream = NULL;

	_capture_channels = 0;
	_playback_channels = 0;
	_cur_sample_rate = 0;
	_cur_input_latency = 0;
	_cur_output_latency = 0;

	free (_input_buffer); _input_buffer = NULL;
	free (_output_buffer); _output_buffer = NULL;
	_state = -1;
}

int
PortAudioIO::pcm_start()
{
	PaError err = Pa_StartStream (_stream);

	if (err != paNoError) {
		_state = -1;
		return -1;
	}
	return 0;
}

#ifdef __APPLE__
static uint32_t lower_power_of_two (uint32_t v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v >> 1;
}
#endif

int
PortAudioIO::pcm_setup (
		int device_input, int device_output,
		double sample_rate, uint32_t samples_per_period)
{
	_state = -2;

	PaError err = paNoError;
	const PaDeviceInfo *nfo_in = NULL;
	const PaDeviceInfo *nfo_out = NULL;
	const PaStreamInfo *nfo_s = NULL;
		
	if (!initialize_pa()) {
		DEBUG_AUDIO ("PortAudio Initialization Failed\n");
		goto error;
	}

	if (device_input == DeviceDefault) {
		device_input = get_default_input_device ();
	}
	if (device_output == DeviceDefault) {
		device_output = get_default_output_device ();
	}

	_capture_channels = 0;
	_playback_channels = 0;
	_cur_sample_rate = 0;
	_cur_input_latency = 0;
	_cur_output_latency = 0;

	DEBUG_AUDIO (string_compose (
	    "PortAudio Device IDs: i:%1 o:%2\n", device_input, device_output));

	if (device_input == DeviceNone && device_output == DeviceNone) {
		// just send the error msg for now rather than return it
		error << AudioBackend::get_error_string(AudioBackend::DeviceConfigurationNotSupportedError)
		      << endmsg;
		return -1;
	}

	if (device_input != DeviceNone) {
		nfo_in = Pa_GetDeviceInfo(device_input);
	}

	if (device_output != DeviceNone) {
		nfo_out = Pa_GetDeviceInfo(device_output);
	}

	if (!nfo_in && ! nfo_out) {
		DEBUG_AUDIO ("PortAudio Cannot Query Device Info\n");
		goto error;
	}

	if (nfo_in) {
		_capture_channels = nfo_in->maxInputChannels;
	}
	if (nfo_out) {
		_playback_channels = nfo_out->maxOutputChannels;
	}

	if(_capture_channels == 0 && _playback_channels == 0) {
		DEBUG_AUDIO ("PortAudio no input or output channels.\n");
		goto error;
	}

#ifdef __APPLE__
	// pa_mac_core_blocking.c pa_stable_v19_20140130
	// BUG: ringbuffer alloc requires power-of-two chn count.
	if ((_capture_channels & (_capture_channels - 1)) != 0) {
		DEBUG_AUDIO (
		    "Adjusted capture channels to power of two (portaudio rb bug)\n");
		_capture_channels = lower_power_of_two (_capture_channels);
	}

	if ((_playback_channels & (_playback_channels - 1)) != 0) {
		DEBUG_AUDIO (
		    "Adjusted capture channels to power of two (portaudio rb bug)\n");
		_playback_channels = lower_power_of_two (_playback_channels);
	}
#endif

	DEBUG_AUDIO (string_compose ("PortAudio Channels: in:%1 out:%2\n",
	                             _capture_channels,
	                             _playback_channels));

	PaStreamParameters inputParam;
	PaStreamParameters outputParam;

	if (nfo_in) {
		inputParam.device = device_input;
		inputParam.channelCount = _capture_channels;
#ifdef INTERLEAVED_INPUT
		inputParam.sampleFormat = paFloat32;
#else
		inputParam.sampleFormat = paFloat32 | paNonInterleaved;
#endif
		inputParam.suggestedLatency = nfo_in->defaultLowInputLatency;
		inputParam.hostApiSpecificStreamInfo = NULL;
	}

	if (nfo_out) {
		outputParam.device = device_output;
		outputParam.channelCount = _playback_channels;
#ifdef INTERLEAVED_OUTPUT
		outputParam.sampleFormat = paFloat32;
#else
		outputParam.sampleFormat = paFloat32 | paNonInterleaved;
#endif
		outputParam.suggestedLatency = nfo_out->defaultLowOutputLatency;
		outputParam.hostApiSpecificStreamInfo = NULL;
	}

	// XXX re-consider using callback API, testing needed.
	err = Pa_OpenStream (
			&_stream,
			_capture_channels > 0 ? &inputParam: NULL,
			_playback_channels > 0 ? &outputParam: NULL,
			sample_rate,
			samples_per_period,
			paDitherOff,
			NULL, NULL);

	if (err != paNoError) {
		DEBUG_AUDIO ("PortAudio failed to start stream.\n");
		goto error;
	}

	nfo_s = Pa_GetStreamInfo (_stream);
	if (!nfo_s) {
		DEBUG_AUDIO ("PortAudio failed to query stream information.\n");
		pcm_stop();
		goto error;
	}

	_cur_sample_rate = nfo_s->sampleRate;
	_cur_input_latency = nfo_s->inputLatency * _cur_sample_rate;
	_cur_output_latency = nfo_s->outputLatency * _cur_sample_rate;

	DEBUG_AUDIO (string_compose ("PA Sample Rate %1 SPS\n", _cur_sample_rate));

	DEBUG_AUDIO (string_compose ("PA Input Latency %1ms, %2 spl\n",
	                             1e3 * nfo_s->inputLatency,
	                             _cur_input_latency));

	DEBUG_AUDIO (string_compose ("PA Output Latency %1ms, %2 spl\n",
	                             1e3 * nfo_s->outputLatency,
	                             _cur_output_latency));

	_state = 0;

	if (_capture_channels > 0) {
		_input_buffer = (float*) malloc (samples_per_period * _capture_channels * sizeof(float));
		if (!_input_buffer) {
			DEBUG_AUDIO ("PortAudio failed to allocate input buffer.\n");
			pcm_stop();
			goto error;
		}
	}

	if (_playback_channels > 0) {
		_output_buffer = (float*) calloc (samples_per_period * _playback_channels, sizeof(float));
		if (!_output_buffer) {
			DEBUG_AUDIO ("PortAudio failed to allocate output buffer.\n");
			pcm_stop();
			goto error;
		}
	}

	return 0;

error:
	_capture_channels = 0;
	_playback_channels = 0;
	free (_input_buffer); _input_buffer = NULL;
	free (_output_buffer); _output_buffer = NULL;
	return -1;
}

int
PortAudioIO::next_cycle (uint32_t n_samples)
{
	bool xrun = false;
	PaError err;
	err = Pa_IsStreamActive (_stream);
	if (err != 1) {
		//   0: inactive / aborted
		// < 0: error
		return -1;
	}

	// TODO, check drift..  process part with larger capacity first.
	// Pa_GetStreamReadAvailable(_stream) < Pa_GetStreamWriteAvailable(_stream)

	if (_playback_channels > 0) {
		err = Pa_WriteStream (_stream, _output_buffer, n_samples);
		if (err) xrun = true;
	}

	if (_capture_channels > 0) {
		err = Pa_ReadStream (_stream, _input_buffer, n_samples);
		if (err) {
			memset (_input_buffer, 0, sizeof(float) * n_samples * _capture_channels);
			xrun = true;
		}
	}


	return xrun ? 1 : 0;
}

std::string
PortAudioIO::get_input_channel_name (int device_id, uint32_t channel) const
{
#ifdef WITH_ASIO
	const char* channel_name;

	// This will return an error for non-ASIO devices so no need to check if
	// the device_id corresponds to an ASIO device.
	PaError err = PaAsio_GetInputChannelName (device_id, channel, &channel_name);

	if (err == paNoError) {
		DEBUG_AUDIO (
		    string_compose ("Input channel name for device %1, channel %2 is %3\n",
		                    device_id,
		                    channel,
		                    channel_name));
		return channel_name;
	}
#endif
	return std::string();
}

std::string
PortAudioIO::get_output_channel_name (int device_id, uint32_t channel) const
{
#ifdef WITH_ASIO
	const char* channel_name;

	PaError err = PaAsio_GetOutputChannelName (device_id, channel, &channel_name);

	if (err == paNoError) {
		DEBUG_AUDIO (
		    string_compose ("Output channel name for device %1, channel %2 is %3\n",
		                    device_id,
		                    channel,
		                    channel_name));
		return channel_name;
	}
#endif
	return std::string();
}

#ifdef INTERLEAVED_INPUT

int
PortAudioIO::get_capture_channel (uint32_t chn, float *input, uint32_t n_samples)
{
	assert(chn < _capture_channels);
	const uint32_t stride = _capture_channels;
	float *ptr = _input_buffer + chn;
	while (n_samples-- > 0) {
		*input++ = *ptr;
		ptr += stride;
	}
	return 0;
}

#else

int
PortAudioIO::get_capture_channel (uint32_t chn, float *input, uint32_t n_samples)
{
	assert(chn < _capture_channels);
	memcpy((void*)input, &(_input_buffer[chn * n_samples]), n_samples * sizeof(float));
	return 0;
}

#endif


#ifdef INTERLEAVED_OUTPUT

int
PortAudioIO::set_playback_channel (uint32_t chn, const float *output, uint32_t n_samples)
{
	assert(chn < _playback_channels);
	const uint32_t stride = _playback_channels;
	float *ptr = _output_buffer + chn;
	while (n_samples-- > 0) {
		*ptr = *output++;
		ptr += stride;
	}
	return 0;
}

#else

int
PortAudioIO::set_playback_channel (uint32_t chn, const float *output, uint32_t n_samples)
{
	assert(chn < _playback_channels);
	memcpy((void*)&(_output_buffer[chn * n_samples]), (void*)output, n_samples * sizeof(float));
	return 0;
}

#endif
