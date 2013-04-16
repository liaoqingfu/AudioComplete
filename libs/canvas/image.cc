/*
    Copyright (C) 2013 Paul Davis

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
*/

#include "canvas/image.h"

#include "gtkmm2ext/gui_thread.h"

using namespace ArdourCanvas;

Image::Image (Group* group, Cairo::Format fmt, int width, int height)
	: Item (group)
	, _format (fmt)
	, _width (width)
	, _height (height)
	, _need_render (false)
{
	DataReady.connect (data_connections, MISSING_INVALIDATOR, boost::bind (&Image::accept_data, this), gui_context());
}

void 
Image::render (Rect const& area, Cairo::RefPtr<Cairo::Context> context) const
{
	if (_current) {
		_surface = Cairo::ImageSurface::create (_current->data.get(),
							_current->format,
							_current->width,
							_current->height,
							_current->stride);
	}

	_current.reset ();

	context->set_source (_surface, 0, 0);
	context->rectangle (area.x0, area.y0, area.width(), area.height());
	context->fill ();
}

void
Image::compute_bounding_box () const
{
	_bounding_box = boost::optional<Rect> (Rect (0, 0, _width, _height));
	_bounding_box_dirty = false;
}

boost::shared_ptr<Image::Data>
Image::get_image ()
{
	int stride = Cairo::ImageSurface::format_stride_for_width (_format, _width);
	boost::shared_ptr<Data> d (new Data (boost::shared_array<uint8_t> (new uint8_t[stride*_height]), _width, _height, stride, _format));

	return d;
}

void
Image::put_image (boost::shared_ptr<Data> d)
{
	_pending = d;
	DataReady (); /* EMIT SIGNAL */
}

void
Image::accept_data () 
{
	/* must be executed in gui thread */
	_current = _pending;
	_pending.reset ();
	_need_render = true;
}	     

