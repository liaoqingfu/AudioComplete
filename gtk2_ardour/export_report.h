/*
 * Copyright (C) 2016 Robin Gareus <robin@gareus.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <cairo/cairo.h>
#include <gtkmm/notebook.h>

#include "gtkmm2ext/cairo_widget.h"
#include "gtkmm2ext/gui_thread.h"

#include "ardour/export_status.h"

#include "ardour_dialog.h"

class CimgArea : public CairoWidget
{
public:
	CimgArea (Cairo::RefPtr<Cairo::ImageSurface> sf)
		: CairoWidget()
		, _surface(sf)
		, _playhead(-1)
		, _x0 (0)
		, _aw (0)
	{
		set_size_request (sf->get_width (), sf->get_height ());
	}

	virtual void render (cairo_t* cr, cairo_rectangle_t* r)
	{
		cairo_rectangle (cr, r->x, r->y, r->width, r->height);
		cairo_clip (cr);
		cairo_set_source_surface (cr, _surface->cobj(), 0, 0);
		cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
		cairo_paint (cr);

		if (_playhead > 0 && _playhead < 1.0 && _aw > 0) {
			cairo_rectangle (cr, _x0, 0, _aw, _surface->get_height());
			cairo_set_source_rgba (cr, .4, .4, .6, .4);
			cairo_fill (cr);

			const float x = _playhead * _aw;
			const float h = _surface->get_height();
			cairo_set_source_rgba (cr, 1, 0, 0, 1);
			cairo_set_line_width (cr, 1.5);
			cairo_move_to (cr, _x0 + x, 0);
			cairo_line_to (cr, _x0 + x, h);
			cairo_stroke (cr);
		}
	}

	void set_playhead (float pos) {
		// TODO re-expose minimal area only, old playhead pos, new playead pos
		if (_playhead == pos) { return; }
		_playhead = pos;
		set_dirty ();
	}

	void set_audition_axis (float x0, float w) {
		_x0 = x0;
		_aw = w;
	}

private:
	Cairo::RefPtr<Cairo::ImageSurface> _surface;
	float _playhead;
	float _x0, _aw;
};

class ExportReport : public ArdourDialog
{
public:
	typedef boost::shared_ptr<ARDOUR::ExportStatus> StatusPtr;
	ExportReport (ARDOUR::Session*, StatusPtr);
	int run ();

private:
	void open_folder (std::string);
	void audition (std::string, unsigned int, int);
	void stop_audition ();
	void audition_active (bool);
	void audition_progress (ARDOUR::framecnt_t, ARDOUR::framecnt_t);
	void on_switch_page (GtkNotebookPage*, guint page_num);

	StatusPtr        status;
	Gtk::Notebook    pages;
	ARDOUR::Session* _session;
	Gtk::Button*     stop_btn;
	PBD::ScopedConnectionList auditioner_connections;

	std::vector<CimgArea*> timeline;
	int _audition_num;
	int _page_num;
};
