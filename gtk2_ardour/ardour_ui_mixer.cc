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

/* This file contains any ARDOUR_UI methods that require knowledge of
   the mixer, and exists so that no compilation dependency exists
   between the main ARDOUR_UI modules and the mixer classes. This
   is to cut down on the nasty compile times for these classes.
*/

#include "gtkmm2ext/keyboard.h"

#include "actions.h"
#include "ardour_ui.h"
#include "mixer_ui.h"
#include "meterbridge.h"

#include "i18n.h"

using namespace ARDOUR;
using namespace PBD;

int
ARDOUR_UI::create_mixer ()

{
	try {
		mixer = Mixer_UI::instance ();
		mixer->StateChange.connect (sigc::mem_fun (*this, &ARDOUR_UI::tabbable_state_change));
		mixer->add_to_notebook (_tabs, _("Mixer"));
	}

	catch (failed_constructor& err) {
		return -1;
	}

        mixer->signal_event().connect (sigc::bind (sigc::ptr_fun (&Gtkmm2ext::Keyboard::catch_user_event_for_pre_dialog_focus), mixer));

	return 0;
}


int
ARDOUR_UI::create_meterbridge ()

{
	try {
		meterbridge = Meterbridge::instance ();
	}

	catch (failed_constructor& err) {
		return -1;
	}

	return 0;
}

