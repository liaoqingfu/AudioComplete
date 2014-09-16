/*
    Copyright (C) 2009 Paul Davis

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

#include "gtkmm2ext/cairo_widget.h"
#include "gtkmm2ext/gui_thread.h"

#include "i18n.h"

static const char* has_cairo_widget_background_info = "has_cairo_widget_background_info";

bool CairoWidget::_flat_buttons = false;

static void noop() { }
sigc::slot<void> CairoWidget::focus_handler (sigc::ptr_fun (noop));

void CairoWidget::set_source_rgb_a( cairo_t* cr, Gdk::Color col, float a)  //ToDo:  this one and the Canvas version should be in a shared file (?)
{
	float r = col.get_red_p ();
	float g = col.get_green_p ();
	float b = col.get_blue_p ();
	
	cairo_set_source_rgba(cr, r, g, b, a);
}

CairoWidget::CairoWidget ()
	: _active_state (Gtkmm2ext::Off)
	, _visual_state (Gtkmm2ext::NoVisualState)
	, _need_bg (true)
	, _grabbed (false)
	, _name_proxy (this, X_("name"))
	, _current_parent (0)
{
	_name_proxy.connect (sigc::mem_fun (*this, &CairoWidget::on_name_changed));
}

CairoWidget::~CairoWidget ()
{
	if (_parent_style_change) _parent_style_change.disconnect();
}

bool
CairoWidget::on_button_press_event (GdkEventButton*)
{
	focus_handler();
	return false;
}

bool
CairoWidget::on_expose_event (GdkEventExpose *ev)
{
	cairo_t* cr = gdk_cairo_create (get_window ()->gobj());
	cairo_rectangle (cr, ev->area.x, ev->area.y, ev->area.width, ev->area.height);
	cairo_clip_preserve (cr);

	/* paint expose area the color of the parent window bg
	*/
	
	Gdk::Color bg (get_parent_bg());
	
	cairo_set_source_rgb (cr, bg.get_red_p(), bg.get_green_p(), bg.get_blue_p());
	cairo_fill (cr);

	cairo_rectangle_t expose_area;
	expose_area.x = ev->area.x;
	expose_area.y = ev->area.y;
	expose_area.width = ev->area.width;
	expose_area.height = ev->area.height;

	render (cr, &expose_area);

	cairo_destroy (cr);

	return true;
}

/** Marks the widget as dirty, so that render () will be called on
 *  the next GTK expose event.
 */

void
CairoWidget::set_dirty ()
{
	ENSURE_GUI_THREAD (*this, &CairoWidget::set_dirty);
	queue_draw ();
}

/** Handle a size allocation.
 *  @param alloc GTK allocation.
 */
void
CairoWidget::on_size_allocate (Gtk::Allocation& alloc)
{
	Gtk::EventBox::on_size_allocate (alloc);

	set_dirty ();
}

Gdk::Color
CairoWidget::get_parent_bg ()
{
	Widget* parent;

	parent = get_parent ();

	while (parent) {
		void* p = g_object_get_data (G_OBJECT(parent->gobj()), has_cairo_widget_background_info);

		if (p) {
			Glib::RefPtr<Gtk::Style> style = parent->get_style();
			if (_current_parent != parent) {
				if (_parent_style_change) _parent_style_change.disconnect();
				_current_parent = parent;
				_parent_style_change = parent->signal_style_changed().connect (mem_fun (*this, &CairoWidget::on_style_changed));
			}
			return style->get_bg (get_state());
		}

		if (!parent->get_has_window()) {
			parent = parent->get_parent();
		} else {
			break;
		}
	}

	if (parent && parent->get_has_window()) {
		if (_current_parent != parent) {
			if (_parent_style_change) _parent_style_change.disconnect();
			_current_parent = parent;
			_parent_style_change = parent->signal_style_changed().connect (mem_fun (*this, &CairoWidget::on_style_changed));
		}
		return parent->get_style ()->get_bg (parent->get_state());
	}

	return get_style ()->get_bg (get_state());
}

void
CairoWidget::set_active_state (Gtkmm2ext::ActiveState s)
{
	if (_active_state != s) {
		_active_state = s;
		StateChanged ();
	}
}

void
CairoWidget::set_visual_state (Gtkmm2ext::VisualState s)
{
	if (_visual_state != s) {
		_visual_state = s;
		StateChanged ();
	}
}

void
CairoWidget::set_active (bool yn)
{
	/* this is an API simplification for buttons
	   that only use the Active and Normal states.
	*/

	if (yn) {
		set_active_state (Gtkmm2ext::ExplicitActive);
	} else {
		unset_active_state ();
	}
}

void
CairoWidget::on_style_changed (const Glib::RefPtr<Gtk::Style>&)
{
	queue_draw();
}

void
CairoWidget::on_state_changed (Gtk::StateType)
{
	/* this will catch GTK-level state changes from calls like
	   ::set_sensitive()
	*/

	if (get_state() == Gtk::STATE_INSENSITIVE) {
		set_visual_state (Gtkmm2ext::VisualState (visual_state() | Gtkmm2ext::Insensitive));
	} else {
		set_visual_state (Gtkmm2ext::VisualState (visual_state() & ~Gtkmm2ext::Insensitive));
	}

	queue_draw ();
}

void
CairoWidget::set_draw_background (bool yn)
{
	_need_bg = yn;
}

void
CairoWidget::provide_background_for_cairo_widget (Gtk::Widget& w, const Gdk::Color& bg)
{
	/* set up @w to be able to provide bg information to
	   any CairoWidgets that are packed inside it.
	*/

	w.modify_bg (Gtk::STATE_NORMAL, bg);
	w.modify_bg (Gtk::STATE_INSENSITIVE, bg);
	w.modify_bg (Gtk::STATE_ACTIVE, bg);
	w.modify_bg (Gtk::STATE_SELECTED, bg);

	g_object_set_data (G_OBJECT(w.gobj()), has_cairo_widget_background_info, (void*) 0xfeedface);
}

void
CairoWidget::set_flat_buttons (bool yn)
{
	_flat_buttons = yn;
}

void
CairoWidget::set_focus_handler (sigc::slot<void> s)
{
	focus_handler = s;
}
