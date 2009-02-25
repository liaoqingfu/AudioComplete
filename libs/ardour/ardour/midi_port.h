/*
    Copyright (C) 2002 Paul Davis 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id: port.h 712 2006-07-28 01:08:57Z drobilla $
*/

#ifndef __ardour_midi_port_h__
#define __ardour_midi_port_h__

#include "ardour/port.h"
#include "ardour/midi_buffer.h"

namespace ARDOUR {

class MidiEngine;

class MidiPort : public Port {
   public:
	~MidiPort();

	DataType type () const {
		return DataType::MIDI;
	}

	void cycle_start (nframes_t nframes, nframes_t offset);
	void cycle_end (nframes_t nframes, nframes_t offset);
	void flush_buffers (nframes_t nframes, nframes_t offset);

	Buffer& get_buffer (nframes_t nframes, nframes_t offset) {
		return get_midi_buffer (nframes, offset);
	}
	
	MidiBuffer& get_midi_buffer (nframes_t nframes, nframes_t offset);

  protected:
	friend class AudioEngine;

	MidiPort (const std::string& name, Flags);
  
  private:

	MidiBuffer* _buffer;
	bool _has_been_mixed_down;
};
 
} // namespace ARDOUR

#endif /* __ardour_midi_port_h__ */
