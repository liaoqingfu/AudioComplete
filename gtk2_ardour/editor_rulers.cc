/*
    Copyright (C) 2000 Paul Davis

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

#ifdef WAF_BUILD
#include "gtk2ardour-config.h"
#endif

#include <cstdio> // for sprintf, grrr
#include <cmath>
#include <inttypes.h>

#include <string>

#include <gtk/gtkaction.h>

#include "canvas/group.h"
#include "canvas/canvas.h"
#include "canvas/ruler.h"
#include "canvas/debug.h"

#include "ardour/session.h"
#include "ardour/tempo.h"
#include "ardour/profile.h"

#include "gtkmm2ext/gtk_ui.h"

#include "editor.h"
#include "editing.h"
#include "actions.h"
#include "gui_thread.h"
#include "time_axis_view.h"
#include "editor_drag.h"
#include "editor_cursors.h"
#include "utils.h"

#include "i18n.h"

using namespace ARDOUR;
using namespace PBD;
using namespace Gtk;
using namespace Editing;

/* the order here must match the "metric" enums in editor.h */

class TimecodeMetric : public ArdourCanvas::Ruler::Metric
{
    public:
	TimecodeMetric (Editor* e) : _editor (e) {}

	void get_marks (std::vector<ArdourCanvas::Ruler::Mark>& marks, double lower, double upper, int maxchars) const {
		_editor->metric_get_timecode (marks, lower, upper, maxchars);
	}

    private:
	Editor* _editor;
};

class SamplesMetric : public ArdourCanvas::Ruler::Metric
{
    public:
	SamplesMetric (Editor* e) : _editor (e) {}

	void get_marks (std::vector<ArdourCanvas::Ruler::Mark>& marks, double lower, double upper, int maxchars) const {
		_editor->metric_get_samples (marks, lower, upper, maxchars);
	}

    private:
	Editor* _editor;
};

class BBTMetric : public ArdourCanvas::Ruler::Metric
{
    public:
	BBTMetric (Editor* e) : _editor (e) {}

	void get_marks (std::vector<ArdourCanvas::Ruler::Mark>& marks, double lower, double upper, int maxchars) const {
		_editor->metric_get_bbt (marks, lower, upper, maxchars);
	}

    private:
	Editor* _editor;
};

class MinsecMetric : public ArdourCanvas::Ruler::Metric
{
    public:
	MinsecMetric (Editor* e) : _editor (e) {}

	void get_marks (std::vector<ArdourCanvas::Ruler::Mark>& marks, double lower, double upper, int maxchars) const {
		_editor->metric_get_minsec (marks, lower, upper, maxchars);
	}

    private:
	Editor* _editor;
};

static ArdourCanvas::Ruler::Metric* _bbt_metric;
static ArdourCanvas::Ruler::Metric* _timecode_metric;
static ArdourCanvas::Ruler::Metric* _samples_metric;
static ArdourCanvas::Ruler::Metric* _minsec_metric;

void
Editor::initialize_rulers ()
{
	ruler_grabbed_widget = 0;
	Pango::FontDescription font = get_font_for_style ("editor_time_ruler");

	_timecode_metric = new TimecodeMetric (this);
	_bbt_metric = new BBTMetric (this);
	_minsec_metric = new MinsecMetric (this);
	_samples_metric = new SamplesMetric (this);
	
	timecode_ruler = new ArdourCanvas::Ruler (_time_markers_group, *_timecode_metric,
						  ArdourCanvas::Rect (0, 0, ArdourCanvas::COORD_MAX, timebar_height));
	timecode_ruler->set_font_description (font);
	CANVAS_DEBUG_NAME (timecode_ruler, "timecode ruler");
	timecode_nmarks = 0;

	samples_ruler = new ArdourCanvas::Ruler (_time_markers_group, *_samples_metric,
						 ArdourCanvas::Rect (0, 0, ArdourCanvas::COORD_MAX, timebar_height));
	samples_ruler->set_font_description (font);
	CANVAS_DEBUG_NAME (samples_ruler, "samples ruler");

	minsec_ruler = new ArdourCanvas::Ruler (_time_markers_group, *_minsec_metric,
						ArdourCanvas::Rect (0, 0, ArdourCanvas::COORD_MAX, timebar_height));
	minsec_ruler->set_font_description (font);
	CANVAS_DEBUG_NAME (minsec_ruler, "minsec ruler");
	minsec_nmarks = 0;

	bbt_ruler = new ArdourCanvas::Ruler (_time_markers_group, *_bbt_metric,
					     ArdourCanvas::Rect (0, 0, ArdourCanvas::COORD_MAX, timebar_height));
	bbt_ruler->set_font_description (font);
	CANVAS_DEBUG_NAME (bbt_ruler, "bbt ruler");
	timecode_nmarks = 0;

	using namespace Box_Helpers;
	BoxList & lab_children =  time_bars_vbox.children();

	lab_children.push_back (Element(minsec_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(timecode_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(samples_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(bbt_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(meter_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(tempo_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(range_mark_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(transport_mark_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(cd_mark_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(mark_label, PACK_SHRINK, PACK_START));
	lab_children.push_back (Element(videotl_label, PACK_SHRINK, PACK_START));

	/* 1 event handler to bind them all ... */

	timecode_ruler->Event.connect (sigc::bind (sigc::mem_fun (*this, &Editor::canvas_ruler_event), timecode_ruler, TimecodeRulerItem));
	minsec_ruler->Event.connect (sigc::bind (sigc::mem_fun (*this, &Editor::canvas_ruler_event), minsec_ruler, MinsecRulerItem));
	bbt_ruler->Event.connect (sigc::bind (sigc::mem_fun (*this, &Editor::canvas_ruler_event), bbt_ruler, BBTRulerItem));
	samples_ruler->Event.connect (sigc::bind (sigc::mem_fun (*this, &Editor::canvas_ruler_event), samples_ruler, SamplesRulerItem));
	
	visible_timebars = 0; /*this will be changed below */
}

bool
Editor::ruler_scroll (GdkEventScroll* event)
{
	framepos_t xdelta;
	int direction = event->direction;
	bool handled = false;

	switch (direction) {
	case GDK_SCROLL_UP:
		temporal_zoom_step (false);
		handled = true;
		break;

	case GDK_SCROLL_DOWN:
		temporal_zoom_step (true);
		handled = true;
		break;

	case GDK_SCROLL_LEFT:
		xdelta = (current_page_samples() / 2);
		if (leftmost_frame > xdelta) {
			reset_x_origin (leftmost_frame - xdelta);
		} else {
			reset_x_origin (0);
		}
		handled = true;
		break;

	case GDK_SCROLL_RIGHT:
		xdelta = (current_page_samples() / 2);
		if (max_framepos - xdelta > leftmost_frame) {
			reset_x_origin (leftmost_frame + xdelta);
		} else {
			reset_x_origin (max_framepos - current_page_samples());
		}
		handled = true;
		break;

	default:
		/* what? */
		break;
	}

	return handled;
}


bool
Editor::ruler_button_press (GdkEventButton* /*ev*/)
{
	if (_session == 0) {
		return false;
	}

#if 0

	Widget * grab_widget = 0;

	if (bbt_ruler->is_realized() && ev->window == bbt_ruler->get_window()->gobj()) {
		grab_widget = bbt_ruler;
	} else if (samples_ruler->is_realized() && ev->window == samples_ruler->get_window()->gobj()) {
		grab_widget = samples_ruler;
	} else if (minsec_ruler->is_realized() && ev->window == minsec_ruler->get_window()->gobj()) {
		grab_widget = minsec_ruler;
	}

	if (grab_widget) {
		grab_widget->add_modal_grab ();
		ruler_grabbed_widget = grab_widget;
	}

	if (ev->button == 1) {
		// Since we will locate the playhead on button release, cancel any running
		// auditions.
		if (_session->is_auditioning()) {
			_session->cancel_audition ();
		}

		/* playhead cursor drag: CursorDrag expects an event with
		 * canvas coordinates, so convert from window coordinates,
		 * since for now, rulers are still Gtk::Widgets.
		 */

		GdkEventButton canvas_ev = *ev;
		ArdourCanvas::Duple d = _track_canvas->window_to_canvas (ArdourCanvas::Duple (ev->x, ev->y));
		canvas_ev.x = rint (d.x);
		canvas_ev.y = rint (d.y);

		_drags->set (new CursorDrag (this, *playhead_cursor, false), reinterpret_cast<GdkEvent *> (&canvas_ev));
		_dragging_playhead = true;
	}
#endif

	return true;
}

bool
Editor::ruler_button_release (GdkEventButton* ev)
{
	if (_session == 0) {
		return false;
	}

	if (_drags->active ()) {
		GdkEventButton canvas_ev = *ev;
		ArdourCanvas::Duple d = _track_canvas->window_to_canvas (ArdourCanvas::Duple (ev->x, ev->y));
		canvas_ev.x = rint (d.x);
		canvas_ev.x = rint (d.y);
		_drags->end_grab (reinterpret_cast<GdkEvent*> (&canvas_ev));
		_dragging_playhead = false;
	}

	if (ev->button == 3) {
		
		stop_canvas_autoscroll();

		framepos_t where = window_event_sample ((GdkEvent*) ev);

		snap_to (where);
		popup_ruler_menu (where);
	}

	if (ruler_grabbed_widget) {
		ruler_grabbed_widget->remove_modal_grab();
		ruler_grabbed_widget = 0;
	}

	return true;
}

bool
Editor::ruler_label_button_release (GdkEventButton* ev)
{
	if (ev->button == 3) {
		Gtk::Menu* m = dynamic_cast<Gtk::Menu*> (ActionManager::get_widget (X_("/RulerMenuPopup")));
		if (m) {
			m->popup (1, ev->time);
		}
	}

	return true;
}


bool
Editor::ruler_mouse_motion (GdkEventMotion* ev)
{
	if (_session == 0) {
		return false;
	}

	if (_drags->active ()) {
		GdkEventMotion canvas_ev = *ev;
		ArdourCanvas::Duple d = _track_canvas->window_to_canvas (ArdourCanvas::Duple (ev->x, ev->y));
		canvas_ev.x = rint (d.x);
		canvas_ev.y = rint (d.y);
		_drags->window_motion_handler (reinterpret_cast<GdkEvent*> (&canvas_ev), false);
	}

	return true;
}


void
Editor::popup_ruler_menu (framepos_t where, ItemType t)
{
	using namespace Menu_Helpers;

	if (editor_ruler_menu == 0) {
		editor_ruler_menu = new Menu;
		editor_ruler_menu->set_name ("ArdourContextMenu");
	}

	// always build from scratch
	MenuList& ruler_items = editor_ruler_menu->items();
	editor_ruler_menu->set_name ("ArdourContextMenu");
	ruler_items.clear();

	switch (t) {
	case MarkerBarItem:
		ruler_items.push_back (MenuElem (_("New location marker"), sigc::bind ( sigc::mem_fun(*this, &Editor::mouse_add_new_marker), where, false, false)));
		ruler_items.push_back (MenuElem (_("Clear all locations"), sigc::mem_fun(*this, &Editor::clear_markers)));
		ruler_items.push_back (MenuElem (_("Unhide locations"), sigc::mem_fun(*this, &Editor::unhide_markers)));
		ruler_items.push_back (SeparatorElem ());
		break;
	case RangeMarkerBarItem:
		ruler_items.push_back (MenuElem (_("New range"), sigc::bind (sigc::mem_fun (*this, &Editor::mouse_add_new_range), where)));
		ruler_items.push_back (MenuElem (_("Clear all ranges"), sigc::mem_fun(*this, &Editor::clear_ranges)));
		ruler_items.push_back (MenuElem (_("Unhide ranges"), sigc::mem_fun(*this, &Editor::unhide_ranges)));
		ruler_items.push_back (SeparatorElem ());

		break;
	case TransportMarkerBarItem:

		break;

	case CdMarkerBarItem:
		// TODO
		ruler_items.push_back (MenuElem (_("New CD track marker"), sigc::bind ( sigc::mem_fun(*this, &Editor::mouse_add_new_marker), where, true, false)));
		break;


	case TempoBarItem:
		ruler_items.push_back (MenuElem (_("New Tempo"), sigc::bind ( sigc::mem_fun(*this, &Editor::mouse_add_new_tempo_event), where)));
		ruler_items.push_back (SeparatorElem ());
		break;

	case MeterBarItem:
		ruler_items.push_back (MenuElem (_("New Meter"), sigc::bind ( sigc::mem_fun(*this, &Editor::mouse_add_new_meter_event), where)));
		ruler_items.push_back (SeparatorElem ());
		break;

	case VideoBarItem:
		ruler_items.push_back (MenuElem (_("Timeline height")));
		static_cast<MenuItem*>(&ruler_items.back())->set_sensitive(false);
		ruler_items.push_back (CheckMenuElem (_("Large"),  sigc::bind ( sigc::mem_fun(*this, &Editor::set_video_timeline_height), 6)));
		if (videotl_bar_height == 6) { static_cast<Gtk::CheckMenuItem*>(&ruler_items.back())->set_active(true);}
		ruler_items.push_back (CheckMenuElem (_("Normal"), sigc::bind ( sigc::mem_fun(*this, &Editor::set_video_timeline_height), 4)));
		if (videotl_bar_height == 4) { static_cast<Gtk::CheckMenuItem*>(&ruler_items.back())->set_active(true);}
		ruler_items.push_back (CheckMenuElem (_("Small"),  sigc::bind ( sigc::mem_fun(*this, &Editor::set_video_timeline_height), 3)));
		if (videotl_bar_height == 3) { static_cast<Gtk::CheckMenuItem*>(&ruler_items.back())->set_active(true);}
		ruler_items.push_back (SeparatorElem ());

		ruler_items.push_back (MenuElem (_("Align Video Track")));
		static_cast<MenuItem*>(&ruler_items.back())->set_sensitive(false);

		ruler_items.push_back (CheckMenuElem (_("Lock")));
		{
		Gtk::CheckMenuItem* vtl_lock = static_cast<Gtk::CheckMenuItem*>(&ruler_items.back());
		vtl_lock->set_active(is_video_timeline_locked());
		vtl_lock->signal_activate().connect (sigc::mem_fun(*this, &Editor::toggle_video_timeline_locked));
		}

		ruler_items.push_back (SeparatorElem ());
		break;

	default:
		break;
	}

	Glib::RefPtr<Action> action;

	action = ActionManager::get_action ("Rulers", "toggle-minsec-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	if (!Profile->get_sae()) {
		action = ActionManager::get_action ("Rulers", "toggle-timecode-ruler");
		if (action) {
			ruler_items.push_back (MenuElem (*action->create_menu_item()));
		}
	}
	action = ActionManager::get_action ("Rulers", "toggle-samples-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-bbt-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-meter-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-tempo-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	if (!Profile->get_sae()) {
		action = ActionManager::get_action ("Rulers", "toggle-range-ruler");
		if (action) {
			ruler_items.push_back (MenuElem (*action->create_menu_item()));
		}
	}
	action = ActionManager::get_action ("Rulers", "toggle-loop-punch-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-cd-marker-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-marker-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}
	action = ActionManager::get_action ("Rulers", "toggle-video-ruler");
	if (action) {
		ruler_items.push_back (MenuElem (*action->create_menu_item()));
	}

	editor_ruler_menu->popup (1, gtk_get_current_event_time());

	no_ruler_shown_update = false;
}

void
Editor::store_ruler_visibility ()
{
	XMLNode* node = new XMLNode(X_("RulerVisibility"));

	node->add_property (X_("timecode"), ruler_timecode_action->get_active() ? "yes": "no");
	node->add_property (X_("bbt"), ruler_bbt_action->get_active() ? "yes": "no");
	node->add_property (X_("samples"), ruler_samples_action->get_active() ? "yes": "no");
	node->add_property (X_("minsec"), ruler_minsec_action->get_active() ? "yes": "no");
	node->add_property (X_("tempo"), ruler_tempo_action->get_active() ? "yes": "no");
	node->add_property (X_("meter"), ruler_meter_action->get_active() ? "yes": "no");
	node->add_property (X_("marker"), ruler_marker_action->get_active() ? "yes": "no");
	node->add_property (X_("rangemarker"), ruler_range_action->get_active() ? "yes": "no");
	node->add_property (X_("transportmarker"), ruler_loop_punch_action->get_active() ? "yes": "no");
	node->add_property (X_("cdmarker"), ruler_cd_marker_action->get_active() ? "yes": "no");
	node->add_property (X_("videotl"), ruler_video_action->get_active() ? "yes": "no");

	_session->add_extra_xml (*node);
	_session->set_dirty ();
}

void
Editor::restore_ruler_visibility ()
{
	XMLProperty* prop;
	XMLNode * node = _session->extra_xml (X_("RulerVisibility"));

	no_ruler_shown_update = true;

	if (node) {
		if ((prop = node->property ("timecode")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_timecode_action->set_active (true);
			} else {
				ruler_timecode_action->set_active (false);
			}
		}
		if ((prop = node->property ("bbt")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_bbt_action->set_active (true);
			} else {
				ruler_bbt_action->set_active (false);
			}
		}
		if ((prop = node->property ("samples")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_samples_action->set_active (true);
			} else {
				ruler_samples_action->set_active (false);
			}
		}
		if ((prop = node->property ("minsec")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_minsec_action->set_active (true);
			} else {
				ruler_minsec_action->set_active (false);
			}
		}
		if ((prop = node->property ("tempo")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_tempo_action->set_active (true);
			} else {
				ruler_tempo_action->set_active (false);
			}
		}
		if ((prop = node->property ("meter")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_meter_action->set_active (true);
			} else {
				ruler_meter_action->set_active (false);
			}
		}
		if ((prop = node->property ("marker")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_marker_action->set_active (true);
			} else {
				ruler_marker_action->set_active (false);
			}
		}
		if ((prop = node->property ("rangemarker")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_range_action->set_active (true);
			} else {
				ruler_range_action->set_active (false);
			}
		}

		if ((prop = node->property ("transportmarker")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_loop_punch_action->set_active (true);
			} else {
				ruler_loop_punch_action->set_active (false);
			}
		}

		if ((prop = node->property ("cdmarker")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_cd_marker_action->set_active (true);
			} else {
				ruler_cd_marker_action->set_active (false);
			}

		} else {
			// this _session doesn't yet know about the cdmarker ruler
			// as a benefit to the user who doesn't know the feature exists, show the ruler if
			// any cd marks exist
			ruler_cd_marker_action->set_active (false);
			const Locations::LocationList & locs = _session->locations()->list();
			for (Locations::LocationList::const_iterator i = locs.begin(); i != locs.end(); ++i) {
				if ((*i)->is_cd_marker()) {
					ruler_cd_marker_action->set_active (true);
					break;
				}
			}
		}

		if ((prop = node->property ("videotl")) != 0) {
			if (string_is_affirmative (prop->value())) {
				ruler_video_action->set_active (true);
			} else {
				ruler_video_action->set_active (false);
			}
		}

	}

	no_ruler_shown_update = false;
	update_ruler_visibility ();
}

void
Editor::update_ruler_visibility ()
{
	int visible_timebars = 0;

	if (no_ruler_shown_update) {
		return;
	}

	/* the order of the timebars is fixed, so we have to go through each one
	 * and adjust its position depending on what is shown.
	 *
	 * Order: minsec, timecode, samples, bbt, meter, tempo, ranges,
	 * loop/punch, cd markers, location markers
	 */

	double tbpos = 0.0;
	double tbgpos = 0.0;
	double old_unit_pos;

#ifdef GTKOSX
	/* gtk update probs require this (damn) */
	meter_label.hide();
	tempo_label.hide();
	range_mark_label.hide();
	transport_mark_label.hide();
	cd_mark_label.hide();
	mark_label.hide();
	videotl_label.hide();
#endif

	if (ruler_minsec_action->get_active()) {
		old_unit_pos = minsec_ruler->position().y;
		if (tbpos != old_unit_pos) {
			minsec_ruler->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		minsec_ruler->show();
		minsec_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		minsec_ruler->hide();
		minsec_label.hide();
	}

	if (ruler_timecode_action->get_active()) {
		old_unit_pos = timecode_ruler->position().y;
		if (tbpos != old_unit_pos) {
			timecode_ruler->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		timecode_ruler->show();
		timecode_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		timecode_ruler->hide();
		timecode_label.hide();
	}

	if (ruler_samples_action->get_active()) {
		old_unit_pos = samples_ruler->position().y;
		if (tbpos != old_unit_pos) {
			samples_ruler->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		samples_ruler->show();
		samples_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		samples_ruler->hide();
		samples_label.hide();
	}

	if (ruler_bbt_action->get_active()) {
		old_unit_pos = bbt_ruler->position().y;
		if (tbpos != old_unit_pos) {
			bbt_ruler->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		bbt_ruler->show();
		bbt_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		bbt_ruler->hide();
		bbt_label.hide();
	}

	if (ruler_meter_action->get_active()) {
		old_unit_pos = meter_group->position().y;
		if (tbpos != old_unit_pos) {
			meter_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		meter_group->show();
		meter_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		meter_group->hide();
		meter_label.hide();
	}

	if (ruler_tempo_action->get_active()) {
		old_unit_pos = tempo_group->position().y;
		if (tbpos != old_unit_pos) {
			tempo_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		tempo_group->show();
		tempo_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		tempo_group->hide();
		tempo_label.hide();
	}

	if (!Profile->get_sae() && ruler_range_action->get_active()) {
		old_unit_pos = range_marker_group->position().y;
		if (tbpos != old_unit_pos) {
			range_marker_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		range_marker_group->show();
		range_mark_label.show();

		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		range_marker_group->hide();
		range_mark_label.hide();
	}

	if (ruler_loop_punch_action->get_active()) {
		old_unit_pos = transport_marker_group->position().y;
		if (tbpos != old_unit_pos) {
			transport_marker_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		transport_marker_group->show();
		transport_mark_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		transport_marker_group->hide();
		transport_mark_label.hide();
	}

	if (ruler_cd_marker_action->get_active()) {
		old_unit_pos = cd_marker_group->position().y;
		if (tbpos != old_unit_pos) {
			cd_marker_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		cd_marker_group->show();
		cd_mark_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
		// make sure all cd markers show up in their respective places
		update_cd_marker_display();
	} else {
		cd_marker_group->hide();
		cd_mark_label.hide();
		// make sure all cd markers show up in their respective places
		update_cd_marker_display();
	}

	if (ruler_marker_action->get_active()) {
		old_unit_pos = marker_group->position().y;
		if (tbpos != old_unit_pos) {
			marker_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		marker_group->show();
		mark_label.show();
		tbpos += timebar_height;
		tbgpos += timebar_height;
		visible_timebars++;
	} else {
		marker_group->hide();
		mark_label.hide();
	}

	if (ruler_video_action->get_active()) {
		old_unit_pos = videotl_group->position().y;
		if (tbpos != old_unit_pos) {
			videotl_group->move (ArdourCanvas::Duple (0.0, tbpos - old_unit_pos));
		}
		videotl_group->show();
		videotl_label.show();
		tbpos += timebar_height * videotl_bar_height;
		tbgpos += timebar_height * videotl_bar_height;
		visible_timebars+=videotl_bar_height;
		queue_visual_videotimeline_update();
	} else {
		videotl_group->hide();
		videotl_label.hide();
		update_video_timeline(true);
	}

	time_bars_vbox.set_size_request (-1, (int)(timebar_height * visible_timebars));

	/* move hv_scroll_group (trackviews) to the end of the timebars
	 */

	hv_scroll_group->set_y_position (timebar_height * visible_timebars);

	compute_fixed_ruler_scale ();
	update_fixed_rulers();
	redisplay_tempo (false);

	/* Changing ruler visibility means that any lines on markers might need updating */
	for (LocationMarkerMap::iterator i = location_markers.begin(); i != location_markers.end(); ++i) {
		i->second->setup_lines ();
	}
}

void
Editor::update_just_timecode ()
{
	ENSURE_GUI_THREAD (*this, &Editor::update_just_timecode)

	if (_session == 0) {
		return;
	}

	framepos_t rightmost_frame = leftmost_frame + current_page_samples();

	if (ruler_timecode_action->get_active()) {
		timecode_ruler->set_range (leftmost_frame, rightmost_frame);
	}
}

void
Editor::compute_fixed_ruler_scale ()
{
	if (_session == 0) {
		return;
	}

	if (ruler_timecode_action->get_active()) {
		set_timecode_ruler_scale (leftmost_frame, leftmost_frame + current_page_samples());
	}

	if (ruler_minsec_action->get_active()) {
		set_minsec_ruler_scale (leftmost_frame, leftmost_frame + current_page_samples());
	}

	if (ruler_samples_action->get_active()) {
		set_samples_ruler_scale (leftmost_frame, leftmost_frame + current_page_samples());
	}
}

void
Editor::update_fixed_rulers ()
{
	framepos_t rightmost_frame;

	if (_session == 0) {
		return;
	}

	compute_fixed_ruler_scale ();

	_timecode_metric->units_per_pixel = samples_per_pixel;
	_samples_metric->units_per_pixel = samples_per_pixel;
	_minsec_metric->units_per_pixel = samples_per_pixel;

	rightmost_frame = leftmost_frame + current_page_samples();

	/* these force a redraw, which in turn will force execution of the metric callbacks
	   to compute the relevant ticks to display.
	*/

	if (ruler_timecode_action->get_active()) {
		timecode_ruler->set_range (leftmost_frame, rightmost_frame);
	}

	if (ruler_samples_action->get_active()) {
		samples_ruler->set_range (leftmost_frame, rightmost_frame);
	}

	if (ruler_minsec_action->get_active()) {
		minsec_ruler->set_range (leftmost_frame, rightmost_frame);
	}
}

void
Editor::update_tempo_based_rulers (ARDOUR::TempoMap::BBTPointList::const_iterator& begin,
				    ARDOUR::TempoMap::BBTPointList::const_iterator& end)
{
	if (_session == 0) {
		return;
	}

	compute_bbt_ruler_scale (leftmost_frame, leftmost_frame+current_page_samples(),
				 begin, end);

	_bbt_metric->units_per_pixel = samples_per_pixel;

	if (ruler_bbt_action->get_active()) {
		bbt_ruler->set_range (leftmost_frame, leftmost_frame+current_page_samples());
	}
}

void
Editor::set_timecode_ruler_scale (framepos_t lower, framepos_t upper)
{
	framepos_t spacer;
	framepos_t fr;

	if (_session == 0) {
		return;
	}

	fr = _session->frame_rate();

	if (lower > (spacer = (framepos_t) (128 * Editor::get_current_zoom ()))) {
		lower = lower - spacer;
	} else {
		lower = 0;
	}
	upper = upper + spacer;
	framecnt_t const range = upper - lower;

	if (range < (2 * _session->frames_per_timecode_frame())) { /* 0 - 2 frames */
		timecode_ruler_scale = timecode_show_bits;
		timecode_mark_modulo = 20;
		timecode_nmarks = 2 + (2 * _session->config.get_subframes_per_frame());
	} else if (range <= (fr / 4)) { /* 2 frames - 0.250 second */
		timecode_ruler_scale = timecode_show_frames;
		timecode_mark_modulo = 1;
		timecode_nmarks = 2 + (range / (framepos_t)_session->frames_per_timecode_frame());
	} else if (range <= (fr / 2)) { /* 0.25-0.5 second */
		timecode_ruler_scale = timecode_show_frames;
		timecode_mark_modulo = 2;
		timecode_nmarks = 2 + (range / (framepos_t)_session->frames_per_timecode_frame());
	} else if (range <= fr) { /* 0.5-1 second */
		timecode_ruler_scale = timecode_show_frames;
		timecode_mark_modulo = 5;
		timecode_nmarks = 2 + (range / (framepos_t)_session->frames_per_timecode_frame());
	} else if (range <= 2 * fr) { /* 1-2 seconds */
		timecode_ruler_scale = timecode_show_frames;
		timecode_mark_modulo = 10;
		timecode_nmarks = 2 + (range / (framepos_t)_session->frames_per_timecode_frame());
	} else if (range <= 8 * fr) { /* 2-8 seconds */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 1;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 16 * fr) { /* 8-16 seconds */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 2;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 30 * fr) { /* 16-30 seconds */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 5;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 60 * fr) { /* 30-60 seconds */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 5;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 2 * 60 * fr) { /* 1-2 minutes */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 15;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 4 * 60 * fr) { /* 2-4 minutes */
		timecode_ruler_scale = timecode_show_seconds;
		timecode_mark_modulo = 30;
		timecode_nmarks = 2 + (range / fr);
	} else if (range <= 10 * 60 * fr) { /* 4-10 minutes */
		timecode_ruler_scale = timecode_show_minutes;
		timecode_mark_modulo = 2;
		timecode_nmarks = 2 + 10;
	} else if (range <= 30 * 60 * fr) { /* 10-30 minutes */
		timecode_ruler_scale = timecode_show_minutes;
		timecode_mark_modulo = 5;
		timecode_nmarks = 2 + 30;
	} else if (range <= 60 * 60 * fr) { /* 30 minutes - 1hr */
		timecode_ruler_scale = timecode_show_minutes;
		timecode_mark_modulo = 10;
		timecode_nmarks = 2 + 60;
	} else if (range <= 4 * 60 * 60 * fr) { /* 1 - 4 hrs*/
		timecode_ruler_scale = timecode_show_minutes;
		timecode_mark_modulo = 30;
		timecode_nmarks = 2 + (60 * 4);
	} else if (range <= 8 * 60 * 60 * fr) { /* 4 - 8 hrs*/
		timecode_ruler_scale = timecode_show_hours;
		timecode_mark_modulo = 1;
		timecode_nmarks = 2 + 8;
	} else if (range <= 16 * 60 * 60 * fr) { /* 16-24 hrs*/
		timecode_ruler_scale = timecode_show_hours;
		timecode_mark_modulo = 1;
		timecode_nmarks = 2 + 24;
	} else {

		/* not possible if framepos_t is a 32 bit quantity */

		timecode_ruler_scale = timecode_show_hours;
		timecode_mark_modulo = 4;
		timecode_nmarks = 2 + 24;
	}

}

void
Editor::metric_get_timecode (std::vector<ArdourCanvas::Ruler::Mark>& marks, gdouble lower, gdouble /*upper*/, gint /*maxchars*/)
{
	framepos_t pos;
	framecnt_t spacer;
	Timecode::Time timecode;
	gchar buf[16];
	gint n;
	ArdourCanvas::Ruler::Mark mark;

	if (_session == 0) {
		return;
	}

	if (lower > (spacer = (framecnt_t)(128 * Editor::get_current_zoom ()))) {
		lower = lower - spacer;
	} else {
		lower = 0;
	}

	pos = (framecnt_t) floor (lower);

	switch (timecode_ruler_scale) {
	case timecode_show_bits:

		// Find timecode time of this sample (pos) with subframe accuracy
		_session->sample_to_timecode(pos, timecode, true /* use_offset */, true /* use_subframes */ );

		for (n = 0; n < timecode_nmarks; n++) {
			_session->timecode_to_sample(timecode, pos, true /* use_offset */, true /* use_subframes */ );
			if ((timecode.subframes % timecode_mark_modulo) == 0) {
				if (timecode.subframes == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
					snprintf (buf, sizeof(buf), "%s%02u:%02u:%02u:%02u", timecode.negative ? "-" : "", timecode.hours, timecode.minutes, timecode.seconds, timecode.frames);
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
					snprintf (buf, sizeof(buf), ".%02u", timecode.subframes);
				}
			} else {
				snprintf (buf, sizeof(buf)," ");
				mark.style = ArdourCanvas::Ruler::Mark::Micro;
				
			}
			mark.label = buf;
			mark.position = pos;

			marks.push_back (mark);

			// Increment subframes by one
			Timecode::increment_subframes( timecode, _session->config.get_subframes_per_frame() );
		}
	  break;
	case timecode_show_seconds:
		// Find timecode time of this sample (pos)
		_session->sample_to_timecode(pos, timecode, true /* use_offset */, false /* use_subframes */ );
		// Go to next whole second down
		Timecode::seconds_floor( timecode );

		for (n = 0; n < timecode_nmarks; n++) {
			_session->timecode_to_sample(timecode, pos, true /* use_offset */, false /* use_subframes */ );
			if ((timecode.seconds % timecode_mark_modulo) == 0) {
				if (timecode.seconds == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
					mark.position = pos;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
					mark.position = pos;
				}
				snprintf (buf, sizeof(buf), "%s%02u:%02u:%02u:%02u", timecode.negative ? "-" : "", timecode.hours, timecode.minutes, timecode.seconds, timecode.frames);
			} else {
				snprintf (buf, sizeof(buf)," ");
				mark.style = ArdourCanvas::Ruler::Mark::Micro;
				mark.position = pos;

			}
			mark.label = buf;
			marks.push_back (mark);

			Timecode::increment_seconds( timecode, _session->config.get_subframes_per_frame() );
		}
	  break;
	case timecode_show_minutes:
		// Find timecode time of this sample (pos)
		_session->sample_to_timecode(pos, timecode, true /* use_offset */, false /* use_subframes */ );
		// Go to next whole minute down
		Timecode::minutes_floor( timecode );

		for (n = 0; n < timecode_nmarks; n++) {
			_session->timecode_to_sample(timecode, pos, true /* use_offset */, false /* use_subframes */ );
			if ((timecode.minutes % timecode_mark_modulo) == 0) {
				if (timecode.minutes == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				}
				snprintf (buf, sizeof(buf), "%s%02u:%02u:%02u:%02u", timecode.negative ? "-" : "", timecode.hours, timecode.minutes, timecode.seconds, timecode.frames);
			} else {
				snprintf (buf, sizeof(buf)," ");
				mark.style = ArdourCanvas::Ruler::Mark::Micro;

			}
			mark.label = buf;
			mark.position = pos;
			marks.push_back (mark);
			Timecode::increment_minutes( timecode, _session->config.get_subframes_per_frame() );
		}

	  break;
	case timecode_show_hours:
		// Find timecode time of this sample (pos)
		_session->sample_to_timecode(pos, timecode, true /* use_offset */, false /* use_subframes */ );
		// Go to next whole hour down
		Timecode::hours_floor( timecode );

		for (n = 0; n < timecode_nmarks; n++) {
			_session->timecode_to_sample(timecode, pos, true /* use_offset */, false /* use_subframes */ );
			if ((timecode.hours % timecode_mark_modulo) == 0) {
				mark.style = ArdourCanvas::Ruler::Mark::Major;
				snprintf (buf, sizeof(buf), "%s%02u:%02u:%02u:%02u", timecode.negative ? "-" : "", timecode.hours, timecode.minutes, timecode.seconds, timecode.frames);
			} else {
				snprintf (buf, sizeof(buf)," ");
				mark.style = ArdourCanvas::Ruler::Mark::Micro;

			}
			mark.label = buf;
			mark.position = pos;
			marks.push_back (mark);
			Timecode::increment_hours( timecode, _session->config.get_subframes_per_frame() );
		}
	  break;
	case timecode_show_frames:
		// Find timecode time of this sample (pos)
		_session->sample_to_timecode(pos, timecode, true /* use_offset */, false /* use_subframes */ );
		// Go to next whole frame down
		Timecode::frames_floor( timecode );

		for (n = 0; n < timecode_nmarks; n++) {
			_session->timecode_to_sample(timecode, pos, true /* use_offset */, false /* use_subframes */ );
			if ((timecode.frames % timecode_mark_modulo) == 0)  {
				if (timecode.frames == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				}
				mark.position = pos;
				snprintf (buf, sizeof(buf), "%s%02u:%02u:%02u:%02u", timecode.negative ? "-" : "", timecode.hours, timecode.minutes, timecode.seconds, timecode.frames);
			} else {
				snprintf (buf, sizeof(buf)," ");
				mark.style = ArdourCanvas::Ruler::Mark::Micro;
				mark.position = pos;

			}
			mark.label = buf;
			marks.push_back (mark);
			Timecode::increment( timecode, _session->config.get_subframes_per_frame() );
		}

	  break;
	}
}



void
Editor::compute_bbt_ruler_scale (framepos_t lower, framepos_t upper,
				 ARDOUR::TempoMap::BBTPointList::const_iterator begin,
				 ARDOUR::TempoMap::BBTPointList::const_iterator end)
{
	if (_session == 0) {
		return;
	}

	TempoMap::BBTPointList::const_iterator i;
	Timecode::BBT_Time lower_beat, upper_beat; // the beats at each end of the ruler

	_session->bbt_time (lower, lower_beat);
	_session->bbt_time (upper, upper_beat);
	uint32_t beats = 0;

	bbt_accent_modulo = 1;
	bbt_bar_helper_on = false;
	bbt_bars = 0;
	bbt_nmarks = 1;

	bbt_ruler_scale =  bbt_over;

	switch (_snap_type) {
	case SnapToBeatDiv2:
		bbt_beat_subdivision = 2;
		break;
	case SnapToBeatDiv3:
                bbt_beat_subdivision = 3;
		break;
	case SnapToBeatDiv4:
                bbt_beat_subdivision = 4;
		break;
	case SnapToBeatDiv5:
                bbt_beat_subdivision = 5;
		bbt_accent_modulo = 2; // XXX YIKES
		break;
	case SnapToBeatDiv6:
                bbt_beat_subdivision = 6;
		bbt_accent_modulo = 2; // XXX YIKES
		break;
	case SnapToBeatDiv7:
                bbt_beat_subdivision = 7;
		bbt_accent_modulo = 2; // XXX YIKES
		break;
	case SnapToBeatDiv8:
                bbt_beat_subdivision = 8;
		bbt_accent_modulo = 2;
		break;
	case SnapToBeatDiv10:
                bbt_beat_subdivision = 10;
		bbt_accent_modulo = 2; // XXX YIKES
		break;
	case SnapToBeatDiv12:
                bbt_beat_subdivision = 12;
		bbt_accent_modulo = 3;
		break;
	case SnapToBeatDiv14:
                bbt_beat_subdivision = 14;
		bbt_accent_modulo = 3; // XXX YIKES!
		break;
	case SnapToBeatDiv16:
                bbt_beat_subdivision = 16;
		bbt_accent_modulo = 4;
		break;
	case SnapToBeatDiv20:
                bbt_beat_subdivision = 20;
		bbt_accent_modulo = 5;
		break;
	case SnapToBeatDiv24:
                bbt_beat_subdivision = 24;
		bbt_accent_modulo = 6;
		break;
	case SnapToBeatDiv28:
                bbt_beat_subdivision = 28;
		bbt_accent_modulo = 7;
		break;
	case SnapToBeatDiv32:
                bbt_beat_subdivision = 32;
		bbt_accent_modulo = 8;
		break;
	case SnapToBeatDiv64:
                bbt_beat_subdivision = 64;
		bbt_accent_modulo = 8;
		break;
	case SnapToBeatDiv128:
                bbt_beat_subdivision = 128;
		bbt_accent_modulo = 8;
		break;
	default:
                bbt_beat_subdivision = 4;
		break;
	}

	if (distance (begin, end) == 0) {
		return;
	}

	i = end;
	i--;
	if ((*i).beat >= (*begin).beat) {
		bbt_bars = (*i).bar - (*begin).bar;
	} else {
		bbt_bars = (*i).bar - (*begin).bar - 1;
	}
	beats = distance (begin, end) - bbt_bars;

	/* Only show the bar helper if there aren't many bars on the screen */
	if ((bbt_bars < 2) || (beats < 5)) {
	        bbt_bar_helper_on = true;
	}

	if (bbt_bars > 8192) {
		bbt_ruler_scale =  bbt_over;
	} else if (bbt_bars > 1024) {
		bbt_ruler_scale = bbt_show_64;
	} else if (bbt_bars > 256) {
		bbt_ruler_scale = bbt_show_16;
	} else if (bbt_bars > 64) {
		bbt_ruler_scale = bbt_show_4;
	} else if (bbt_bars > 10) {
		bbt_ruler_scale =  bbt_show_1;
	} else if (bbt_bars > 2) {
		bbt_ruler_scale =  bbt_show_beats;
	} else  if (bbt_bars > 0) {
		bbt_ruler_scale =  bbt_show_ticks;
	} else {
		bbt_ruler_scale =  bbt_show_ticks_detail;
	}
	
	if ((bbt_ruler_scale == bbt_show_ticks_detail) && (lower_beat.beats == upper_beat.beats) && (upper_beat.ticks - lower_beat.ticks <= Timecode::BBT_Time::ticks_per_beat / 4)) {
		bbt_ruler_scale =  bbt_show_ticks_super_detail;
	}
}

static void 
edit_last_mark_label (std::vector<ArdourCanvas::Ruler::Mark>& marks, const std::string& newlabel)
{
	ArdourCanvas::Ruler::Mark copy = marks.back();
	copy.label = newlabel;
	marks.pop_back ();
	marks.push_back (copy);
}		

void
Editor::metric_get_bbt (std::vector<ArdourCanvas::Ruler::Mark>& marks, gdouble lower, gdouble upper, gint /*maxchars*/)
{
	if (_session == 0) {
		return;
	}

	TempoMap::BBTPointList::const_iterator i;

	char buf[64];
	gint  n = 0;
	framepos_t pos;
	Timecode::BBT_Time next_beat;
	framepos_t next_beat_pos;
	uint32_t beats = 0;
	uint32_t tick = 0;
	uint32_t skip;
	uint32_t t;
	framepos_t frame_skip;
	double frame_skip_error;
	double bbt_position_of_helper;
	double accumulated_error;
	bool i_am_accented = false;
	bool helper_active = false;
	ArdourCanvas::Ruler::Mark mark;

	ARDOUR::TempoMap::BBTPointList::const_iterator begin;
	ARDOUR::TempoMap::BBTPointList::const_iterator end;

	compute_current_bbt_points (lower, upper, begin, end);

	if (distance (begin, end) == 0) {
		return;
	}

	switch (bbt_ruler_scale) {

	case bbt_show_beats:
		beats = distance (begin, end);
		bbt_nmarks = beats + 2;

		mark.label = "";
		mark.position = lower;
		mark.style = ArdourCanvas::Ruler::Mark::Micro;
		marks.push_back (mark);

		for (n = 1, i = begin; n < bbt_nmarks && i != end; ++i) {

			if ((*i).frame < lower && (bbt_bar_helper_on)) {
				snprintf (buf, sizeof(buf), "<%" PRIu32 "|%" PRIu32, (*i).bar, (*i).beat);
				edit_last_mark_label (marks, buf);
				helper_active = true;
			} else {

				if ((*i).is_bar()) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
					snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
				} else if (((*i).beat % 2 == 1)) {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
					buf[0] = '\0';
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Micro;
					buf[0] = '\0';
				}
				mark.label = buf;
				mark.position = (*i).frame;
				marks.push_back (mark);
				n++;
			}
		}
		break;

	case bbt_show_ticks:

		beats = distance (begin, end);
		bbt_nmarks = (beats + 2) * bbt_beat_subdivision;

		bbt_position_of_helper = lower + (30 * Editor::get_current_zoom ());
		
		// could do marks.assign() here to preallocate

		mark.label = "";
		mark.position = lower;
		mark.style = ArdourCanvas::Ruler::Mark::Micro;
		marks.push_back (mark);

		for (n = 1, i = begin; n < bbt_nmarks && i != end; ++i) {

			if ((*i).frame < lower && (bbt_bar_helper_on)) {
				snprintf (buf, sizeof(buf), "<%" PRIu32 "|%" PRIu32, (*i).bar, (*i).beat);
				edit_last_mark_label (marks, buf);
				helper_active = true;
			} else {

			        if ((*i).is_bar()) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
					snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
					snprintf (buf, sizeof(buf), "%" PRIu32, (*i).beat);
				}
				if (((*i).frame < bbt_position_of_helper) && helper_active) {
					buf[0] = '\0';
				}
				mark.label =  buf;
				mark.position = (*i).frame;
				marks.push_back (mark);
				n++;
			}

			/* Add the tick marks */

			/* Find the next beat */
			next_beat.beats = (*i).beat;
			next_beat.bars = (*i).bar;
			next_beat.ticks = 0;

			if ((*i).meter->divisions_per_bar() > (next_beat.beats + 1)) {
				  next_beat.beats += 1;
			} else {
				  next_beat.bars += 1;
				  next_beat.beats = 1;
			}

			next_beat_pos = _session->tempo_map().frame_time(next_beat);

			frame_skip = (framepos_t) floor (frame_skip_error = (_session->frame_rate() *  60) / (bbt_beat_subdivision * (*i).tempo->beats_per_minute()));
			frame_skip_error -= frame_skip;
			skip = (uint32_t) (Timecode::BBT_Time::ticks_per_beat / bbt_beat_subdivision);

			pos = (*i).frame + frame_skip;
			accumulated_error = frame_skip_error;

			tick = skip;

			for (t = 0; (tick < Timecode::BBT_Time::ticks_per_beat) && (n < bbt_nmarks) && (pos < next_beat_pos) ; pos += frame_skip, tick += skip, ++t) {

			        if (t % bbt_accent_modulo == (bbt_accent_modulo - 1)) {
					i_am_accented = true;
				}

				mark.label = "";

				/* Error compensation for float to framepos_t*/
				accumulated_error += frame_skip_error;
				if (accumulated_error > 1) {
					pos += 1;
					accumulated_error -= 1.0f;
				}

				mark.position = pos;

				if ((bbt_beat_subdivision > 4) && i_am_accented) {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Micro;
				}
				i_am_accented = false;
				marks.push_back (mark);
				n++;
			}
		}

	  break;

	case bbt_show_ticks_detail:

		beats = distance (begin, end);
		bbt_nmarks = (beats + 2) * bbt_beat_subdivision;

		bbt_position_of_helper = lower + (30 * Editor::get_current_zoom ());

		mark.label = "";
		mark.position = lower;
		mark.style = ArdourCanvas::Ruler::Mark::Micro;
		marks.push_back (mark);

		for (n = 1,   i = begin; n < bbt_nmarks && i != end; ++i) {

			if ((*i).frame < lower && (bbt_bar_helper_on)) {
			        snprintf (buf, sizeof(buf), "<%" PRIu32 "|%" PRIu32, (*i).bar, (*i).beat);
				edit_last_mark_label (marks, buf);
				helper_active = true;
			} else {

				if ((*i).is_bar()) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
					snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
					snprintf (buf, sizeof(buf), "%" PRIu32, (*i).beat);
				}
				if (((*i).frame < bbt_position_of_helper) && helper_active) {
					buf[0] = '\0';
				}
				mark.label =  buf;
				mark.position = (*i).frame;
				marks.push_back (mark);
				n++;
			}

			/* Add the tick marks */

			/* Find the next beat */

			next_beat.beats = (*i).beat;
			next_beat.bars = (*i).bar;

			if ((*i).meter->divisions_per_bar() > (next_beat.beats + 1)) {
				  next_beat.beats += 1;
			} else {
				  next_beat.bars += 1;
				  next_beat.beats = 1;
			}

			next_beat_pos = _session->tempo_map().frame_time(next_beat);

			frame_skip = (framepos_t) floor (frame_skip_error = (_session->frame_rate() *  60) / (bbt_beat_subdivision * (*i).tempo->beats_per_minute()));
			frame_skip_error -= frame_skip;
			skip = (uint32_t) (Timecode::BBT_Time::ticks_per_beat / bbt_beat_subdivision);

			pos = (*i).frame + frame_skip;
			accumulated_error = frame_skip_error;

			tick = skip;

			for (t = 0; (tick < Timecode::BBT_Time::ticks_per_beat) && (n < bbt_nmarks) && (pos < next_beat_pos) ; pos += frame_skip, tick += skip, ++t) {

			        if (t % bbt_accent_modulo == (bbt_accent_modulo - 1)) {
				        i_am_accented = true;
				}

				if (i_am_accented && (pos > bbt_position_of_helper)){
				        snprintf (buf, sizeof(buf), "%" PRIu32, tick);
				} else {
					buf[0] = '\0';
				}

				mark.label = buf;

				/* Error compensation for float to framepos_t*/
				accumulated_error += frame_skip_error;
				if (accumulated_error > 1) {
				        pos += 1;
					accumulated_error -= 1.0f;
				}

				mark.position = pos;

				if ((bbt_beat_subdivision > 4) && i_am_accented) {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Micro;
				}
				i_am_accented = false;
				n++;
			}
		}

	  break;

	case bbt_show_ticks_super_detail:

		beats = distance (begin, end);
		bbt_nmarks = (beats + 2) * bbt_beat_subdivision;

		bbt_position_of_helper = lower + (30 * Editor::get_current_zoom ());

		mark.label = "";
		mark.position = lower;
		mark.style = ArdourCanvas::Ruler::Mark::Micro;
		marks.push_back (mark);

		for (n = 1,   i = begin; n < bbt_nmarks && i != end; ++i) {

			if ((*i).frame < lower && (bbt_bar_helper_on)) {
				  snprintf (buf, sizeof(buf), "<%" PRIu32 "|%" PRIu32, (*i).bar, (*i).beat);
				  edit_last_mark_label (marks, buf);
				  helper_active = true;
			} else {

				  if ((*i).is_bar()) {
					  mark.style = ArdourCanvas::Ruler::Mark::Major;
					  snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
				  } else {
					  mark.style = ArdourCanvas::Ruler::Mark::Minor;
					  snprintf (buf, sizeof(buf), "%" PRIu32, (*i).beat);
				  }
				  if (((*i).frame < bbt_position_of_helper) && helper_active) {
					  buf[0] = '\0';
				  }
				  mark.label =  buf;
				  mark.position = (*i).frame;
				  marks.push_back (mark);
				  n++;
			}

			/* Add the tick marks */

			/* Find the next beat */

			next_beat.beats = (*i).beat;
			next_beat.bars = (*i).bar;

			if ((*i).meter->divisions_per_bar() > (next_beat.beats + 1)) {
				  next_beat.beats += 1;
			} else {
				  next_beat.bars += 1;
				  next_beat.beats = 1;
			}

			next_beat_pos = _session->tempo_map().frame_time(next_beat);

			frame_skip = (framepos_t) floor (frame_skip_error = (_session->frame_rate() *  60) / (bbt_beat_subdivision * (*i).tempo->beats_per_minute()));
			frame_skip_error -= frame_skip;
			skip = (uint32_t) (Timecode::BBT_Time::ticks_per_beat / bbt_beat_subdivision);

			pos = (*i).frame + frame_skip;
			accumulated_error = frame_skip_error;

			tick = skip;

			for (t = 0; (tick < Timecode::BBT_Time::ticks_per_beat) && (n < bbt_nmarks) && (pos < next_beat_pos) ; pos += frame_skip, tick += skip, ++t) {

				  if (t % bbt_accent_modulo == (bbt_accent_modulo - 1)) {
					  i_am_accented = true;
				  }

				  if (pos > bbt_position_of_helper) {
 					  snprintf (buf, sizeof(buf), "%" PRIu32, tick);
				  } else {
					  buf[0] = '\0';
				  }

				  mark.label = buf;

				  /* Error compensation for float to framepos_t*/
				  accumulated_error += frame_skip_error;
				  if (accumulated_error > 1) {
					  pos += 1;
					  accumulated_error -= 1.0f;
				  }

				  mark.position = pos;

				  if ((bbt_beat_subdivision > 4) && i_am_accented) {
					  mark.style = ArdourCanvas::Ruler::Mark::Minor;
				  } else {
					  mark.style = ArdourCanvas::Ruler::Mark::Micro;
				  }
				  i_am_accented = false;
				  marks.push_back (mark);
				  n++;
			}
		}

	  break;

	case bbt_over:
	                bbt_nmarks = 1;
			snprintf (buf, sizeof(buf), "cannot handle %" PRIu32 " bars", bbt_bars );
        		mark.style = ArdourCanvas::Ruler::Mark::Major;
        		mark.label = buf;
			mark.position = lower;
			marks.push_back (mark);

	  break;

	case bbt_show_64:
        		bbt_nmarks = (gint) (bbt_bars / 64) + 1;
			for (n = 0,   i = begin; i != end && n < bbt_nmarks; i++) {
				if ((*i).is_bar()) {
					if ((*i).bar % 64 == 1) {
						if ((*i).bar % 256 == 1) {
							snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
							mark.style = ArdourCanvas::Ruler::Mark::Major;
						} else {
							buf[0] = '\0';
							if ((*i).bar % 256 == 129)  {
								mark.style = ArdourCanvas::Ruler::Mark::Minor;
							} else {
								mark.style = ArdourCanvas::Ruler::Mark::Micro;
							}
						}
						mark.label = buf;
						mark.position = (*i).frame;
						marks.push_back (mark);
						++n;
					}
				}
			}
			break;

	case bbt_show_16:
       		bbt_nmarks = (bbt_bars / 16) + 1;
		for (n = 0,  i = begin; i != end && n < bbt_nmarks; i++) {
		        if ((*i).is_bar()) {
			  if ((*i).bar % 16 == 1) {
			        if ((*i).bar % 64 == 1) {
				        snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					buf[0] = '\0';
					if ((*i).bar % 64 == 33)  {
					        mark.style = ArdourCanvas::Ruler::Mark::Minor;
					} else {
					        mark.style = ArdourCanvas::Ruler::Mark::Micro;
					}
				}
				mark.label = buf;
				mark.position = (*i).frame;
				marks.push_back (mark);
				++n;
			  }
			}
		}
	  break;

	case bbt_show_4:
		bbt_nmarks = (bbt_bars / 4) + 1;
		for (n = 0,   i = begin; i != end && n < bbt_nmarks; ++i) {
		        if ((*i).is_bar()) {
			  if ((*i).bar % 4 == 1) {
			        if ((*i).bar % 16 == 1) {
				        snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					buf[0] = '\0';
					if ((*i).bar % 16 == 9)  {
					        mark.style = ArdourCanvas::Ruler::Mark::Minor;
					} else {
					        mark.style = ArdourCanvas::Ruler::Mark::Micro;
					}
				}
				mark.label = buf;
				mark.position = (*i).frame;
				marks.push_back (mark);
				++n;
			  }
			}
		}
	  break;

	case bbt_show_1:
//	default:
	        bbt_nmarks = bbt_bars + 2;
		for (n = 0,  i = begin; i != end && n < bbt_nmarks; ++i) {
		        if ((*i).is_bar()) {
			  if ((*i).bar % 4 == 1) {
				        snprintf (buf, sizeof(buf), "%" PRIu32, (*i).bar);
					mark.style = ArdourCanvas::Ruler::Mark::Major;
			  } else {
				  buf[0] = '\0';
				  if ((*i).bar % 4 == 3)  {
					  mark.style = ArdourCanvas::Ruler::Mark::Minor;
				  } else {
					  mark.style = ArdourCanvas::Ruler::Mark::Micro;
				  }
			  }
			  mark.label = buf;
			  mark.position = (*i).frame;
			  marks.push_back (mark);
			  ++n;
			}
		}
		break;

	}
}

void
Editor::set_samples_ruler_scale (framepos_t lower, framepos_t upper)
{
	_samples_ruler_interval = (upper - lower) / 5;
}

void
Editor::metric_get_samples (std::vector<ArdourCanvas::Ruler::Mark>& marks, gdouble lower, gdouble /*upper*/, gint /*maxchars*/)
{
	framepos_t pos;
	framepos_t const ilower = (framepos_t) floor (lower);
	gchar buf[16];
	gint nmarks;
	gint n;
	ArdourCanvas::Ruler::Mark mark;

	if (_session == 0) {
		return;
	}

	nmarks = 5;
	for (n = 0, pos = ilower; n < nmarks; pos += _samples_ruler_interval, ++n) {
		snprintf (buf, sizeof(buf), "%" PRIi64, pos);
		mark.label = buf;
		mark.position = pos;
		mark.style = ArdourCanvas::Ruler::Mark::Major;
		marks.push_back (mark);
	}
}

static void
sample_to_clock_parts ( framepos_t sample,
			framepos_t sample_rate,
			long *hrs_p,
			long *mins_p,
			long *secs_p,
			long *millisecs_p)

{
	framepos_t left;
	long hrs;
	long mins;
	long secs;
	long millisecs;

	left = sample;
	hrs = left / (sample_rate * 60 * 60 * 1000);
	left -= hrs * sample_rate * 60 * 60 * 1000;
	mins = left / (sample_rate * 60 * 1000);
	left -= mins * sample_rate * 60 * 1000;
	secs = left / (sample_rate * 1000);
	left -= secs * sample_rate * 1000;
	millisecs = left / sample_rate;

	*millisecs_p = millisecs;
	*secs_p = secs;
	*mins_p = mins;
	*hrs_p = hrs;

	return;
}

void
Editor::set_minsec_ruler_scale (framepos_t lower, framepos_t upper)
{
	framepos_t fr;
	framepos_t spacer;

	if (_session == 0) {
		return;
	}

	fr = _session->frame_rate() * 1000;

	/* to prevent 'flashing' */
	if (lower > (spacer = (framepos_t)(128 * Editor::get_current_zoom ()))) {
		lower -= spacer;
	} else {
		lower = 0;
	}
	upper += spacer;
	framecnt_t const range = (upper - lower) * 1000;

	if (range <  (fr / 50)) {
		minsec_mark_interval =  fr / 1000; /* show 1/1000 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 10;
	} else if (range <= (fr / 10)) { /* 0-0.1 second */
		minsec_mark_interval = fr / 1000; /* show 1/1000 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 10;
	} else if (range <= (fr / 2)) { /* 0-0.5 second */
		minsec_mark_interval = fr / 100;  /* show 1/100 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 100;
	} else if (range <= fr) { /* 0-1 second */
		minsec_mark_interval = fr / 10;  /* show 1/10 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 200;
	} else if (range <= 2 * fr) { /* 1-2 seconds */
		minsec_mark_interval = fr / 10; /* show 1/10 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 500;
	} else if (range <= 8 * fr) { /* 2-5 seconds */
		minsec_mark_interval =  fr / 5; /* show 2 seconds */
		minsec_ruler_scale = minsec_show_frames;
		minsec_mark_modulo = 1000;
	} else if (range <= 16 * fr) { /* 8-16 seconds */
		minsec_mark_interval =  fr; /* show 1 seconds */
		minsec_ruler_scale = minsec_show_seconds;
		minsec_mark_modulo = 2;
	} else if (range <= 30 * fr) { /* 10-30 seconds */
		minsec_mark_interval =  fr; /* show 1 seconds */
		minsec_ruler_scale = minsec_show_seconds;
                minsec_mark_modulo = 5;
	} else if (range <= 60 * fr) { /* 30-60 seconds */
                minsec_mark_interval = fr; /* show 1 seconds */
                minsec_ruler_scale = minsec_show_seconds;
                minsec_mark_modulo = 5;
        } else if (range <= 2 * 60 * fr) { /* 1-2 minutes */
                minsec_mark_interval = 5 * fr; /* show 5 seconds */
                minsec_ruler_scale = minsec_show_seconds;
                minsec_mark_modulo = 3;
        } else if (range <= 4 * 60 * fr) { /* 4 minutes */
                minsec_mark_interval = 5 * fr; /* show 10 seconds */
                minsec_ruler_scale = minsec_show_seconds;
                minsec_mark_modulo = 30;
        } else if (range <= 10 * 60 * fr) { /* 10 minutes */
                minsec_mark_interval = 30 * fr; /* show 30 seconds */
                minsec_ruler_scale = minsec_show_seconds;
                minsec_mark_modulo = 120;
        } else if (range <= 30 * 60 * fr) { /* 10-30 minutes */
                minsec_mark_interval =  60 * fr; /* show 1 minute */
                minsec_ruler_scale = minsec_show_minutes;
		minsec_mark_modulo = 5;
        } else if (range <= 60 * 60 * fr) { /* 30 minutes - 1hr */
                minsec_mark_interval = 2 * 60 * fr; /* show 2 minutes */
                minsec_ruler_scale = minsec_show_minutes;
                minsec_mark_modulo = 10;
        } else if (range <= 4 * 60 * 60 * fr) { /* 1 - 4 hrs*/
                minsec_mark_interval = 5 * 60 * fr; /* show 10 minutes */
                minsec_ruler_scale = minsec_show_minutes;
                minsec_mark_modulo = 30;
        } else if (range <= 8 * 60 * 60 * fr) { /* 4 - 8 hrs*/
                minsec_mark_interval = 20 * 60 * fr; /* show 20 minutes */
                minsec_ruler_scale = minsec_show_minutes;
                minsec_mark_modulo = 60;
        } else if (range <= 16 * 60 * 60 * fr) { /* 16-24 hrs*/
                minsec_mark_interval =  60 * 60 * fr; /* show 60 minutes */
                minsec_ruler_scale = minsec_show_hours;
		minsec_mark_modulo = 2;
        } else {

                /* not possible if framepos_t is a 32 bit quantity */

                minsec_mark_interval = 4 * 60 * 60 * fr; /* show 4 hrs */
        }
	minsec_nmarks = 2 + (range / minsec_mark_interval);
}

void
Editor::metric_get_minsec (std::vector<ArdourCanvas::Ruler::Mark>& marks, gdouble lower, gdouble /*upper*/, gint /*maxchars*/)
{
	framepos_t pos;
	framepos_t spacer;
	long hrs, mins, secs, millisecs;
	gchar buf[16];
	gint n;
	ArdourCanvas::Ruler::Mark mark;

	if (_session == 0) {
		return;
	}

	/* to prevent 'flashing' */
	if (lower > (spacer = (framepos_t) (128 * Editor::get_current_zoom ()))) {
		lower = lower - spacer;
	} else {
		lower = 0;
	}

	pos = (((1000 * (framepos_t) floor(lower)) + (minsec_mark_interval/2))/minsec_mark_interval) * minsec_mark_interval;
	switch (minsec_ruler_scale) {
	case minsec_show_seconds:
		for (n = 0; n < minsec_nmarks; pos += minsec_mark_interval, ++n) {
                	sample_to_clock_parts (pos, _session->frame_rate(), &hrs, &mins, &secs, &millisecs);
              	  	if (secs % minsec_mark_modulo == 0) {
				if (secs == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				}
				snprintf (buf, sizeof(buf), "%02ld:%02ld:%02ld.%03ld", hrs, mins, secs, millisecs);
                	} else {
				buf[0] = '\0';
	                        mark.style = ArdourCanvas::Ruler::Mark::Micro;
        	        }
                	mark.label = buf;
			mark.position = pos/1000.0;
			marks.push_back (mark);
		}
	  break;
	case minsec_show_minutes:
		for (n = 0; n < minsec_nmarks; pos += minsec_mark_interval, ++n) {
                        sample_to_clock_parts (pos, _session->frame_rate(), &hrs, &mins, &secs, &millisecs);
                        if (mins % minsec_mark_modulo == 0) {
                                if (mins == 0) {
                                        mark.style = ArdourCanvas::Ruler::Mark::Major;
                                } else {
                                        mark.style = ArdourCanvas::Ruler::Mark::Minor;
                                }
				snprintf (buf, sizeof(buf), "%02ld:%02ld:%02ld.%03ld", hrs, mins, secs, millisecs);
                        } else {
				buf[0] = '\0';
                                mark.style = ArdourCanvas::Ruler::Mark::Micro;
                        }
                        mark.label = buf;
                        mark.position = pos/1000.0;
			marks.push_back (mark);
                }
	  break;
	case minsec_show_hours:
		 for (n = 0; n < minsec_nmarks; pos += minsec_mark_interval, ++n) {
                        sample_to_clock_parts (pos, _session->frame_rate(), &hrs, &mins, &secs, &millisecs);
                        if (hrs % minsec_mark_modulo == 0) {
                                mark.style = ArdourCanvas::Ruler::Mark::Major;
                                snprintf (buf, sizeof(buf), "%02ld:%02ld:%02ld.%03ld", hrs, mins, secs, millisecs);
                        } else {
				buf[0] = '\0';
                                mark.style = ArdourCanvas::Ruler::Mark::Micro;
                        }
                        mark.label = buf;
                        mark.position = pos/1000.0;
			marks.push_back (mark);
                }
	      break;
	case minsec_show_frames:
		for (n = 0; n < minsec_nmarks; pos += minsec_mark_interval, ++n) {
			sample_to_clock_parts (pos, _session->frame_rate(), &hrs, &mins, &secs, &millisecs);
			if (millisecs % minsec_mark_modulo == 0) {
				if (millisecs == 0) {
					mark.style = ArdourCanvas::Ruler::Mark::Major;
				} else {
					mark.style = ArdourCanvas::Ruler::Mark::Minor;
				}
				snprintf (buf, sizeof(buf), "%02ld:%02ld:%02ld.%03ld", hrs, mins, secs, millisecs);
			} else {
				buf[0] = '\0';
				mark.style = ArdourCanvas::Ruler::Mark::Micro;
			}
			mark.label = buf;
			mark.position = pos/1000.0;
			marks.push_back (mark);
		}
	  break;
	}
}
