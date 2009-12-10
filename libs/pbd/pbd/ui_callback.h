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

#ifndef __pbd_ui_callback_h__
#define __pbd_ui_callback_h__

#include <glibmm/thread.h>
#include <sigc++/slot.h>

namespace PBD
{

class UICallback 
{
  public:
	UICallback() {}
	virtual ~UICallback() {}

	virtual void call_slot (sigc::slot<void> theSlot) = 0;

	static UICallback* get_ui_for_thread() { return thread_ui.get(); }
	static void set_ui_for_thread (UICallback* ui) { return thread_ui.set (ui); }

  private:
	static Glib::StaticPrivate<UICallback> thread_ui;

};

}

#endif /* __pbd_ui_callback_h__ */
