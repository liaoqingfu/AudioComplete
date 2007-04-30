// Generated by gtkmmproc -- DO NOT MODIFY!


#include <gtkmm/tooltips.h>
#include <gtkmm/private/tooltips_p.h>

// -*- c++ -*-
/* $Id$ */

/* Copyright 1998-2002 The gtkmm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <gtk/gtkwidget.h>
#include <gtk/gtktooltips.h>
#include <gdkmm/color.h>
#include <gtkmm/widget.h>


namespace Gtk
{

void Tooltips::set_tip(Gtk::Widget& widget, const Glib::ustring& tip_text)
{
  gtk_tooltips_set_tip(gobj(), widget.gobj(), tip_text.c_str(), 0);
}

void Tooltips::unset_tip(Gtk::Widget& widget)
{
  gtk_tooltips_set_tip(gobj(), widget.gobj(), 0, 0);
}

} // namespace Gtk


namespace
{
} // anonymous namespace


namespace Glib
{

Gtk::Tooltips* wrap(GtkTooltips* object, bool take_copy)
{
  return dynamic_cast<Gtk::Tooltips *> (Glib::wrap_auto ((GObject*)(object), take_copy));
}

} /* namespace Glib */

namespace Gtk
{


/* The *_Class implementation: */

const Glib::Class& Tooltips_Class::init()
{
  if(!gtype_) // create the GType if necessary
  {
    // Glib::Class has to know the class init function to clone custom types.
    class_init_func_ = &Tooltips_Class::class_init_function;

    // This is actually just optimized away, apparently with no harm.
    // Make sure that the parent type has been created.
    //CppClassParent::CppObjectType::get_type();

    // Create the wrapper type, with the same class/instance size as the base type.
    register_derived_type(gtk_tooltips_get_type());

    // Add derived versions of interfaces, if the C type implements any interfaces:
  }

  return *this;
}

void Tooltips_Class::class_init_function(void* g_class, void* class_data)
{
  BaseClassType *const klass = static_cast<BaseClassType*>(g_class);
  CppClassParent::class_init_function(klass, class_data);

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED


Glib::ObjectBase* Tooltips_Class::wrap_new(GObject* o)
{
  return manage(new Tooltips((GtkTooltips*)(o)));

}


/* The implementation: */

Tooltips::Tooltips(const Glib::ConstructParams& construct_params)
:
  Gtk::Object(construct_params)
{
  }

Tooltips::Tooltips(GtkTooltips* castitem)
:
  Gtk::Object((GtkObject*)(castitem))
{
  }

Tooltips::~Tooltips()
{
  destroy_();
}

Tooltips::CppClassType Tooltips::tooltips_class_; // initialize static member

GType Tooltips::get_type()
{
  return tooltips_class_.init().get_type();
}

GType Tooltips::get_base_type()
{
  return gtk_tooltips_get_type();
}


Tooltips::Tooltips()
:
  Glib::ObjectBase(0), //Mark this class as gtkmmproc-generated, rather than a custom class, to allow vfunc optimisations.
  Gtk::Object(Glib::ConstructParams(tooltips_class_.init()))
{
  }

void Tooltips::enable()
{
gtk_tooltips_enable(gobj()); 
}

void Tooltips::disable()
{
gtk_tooltips_disable(gobj()); 
}

void Tooltips::set_tip(Widget& widget, const Glib::ustring& tip_text, const Glib::ustring& tip_private)
{
gtk_tooltips_set_tip(gobj(), (widget).gobj(), tip_text.c_str(), tip_private.c_str()); 
}

GtkTooltipsData* Tooltips::data_get(Widget& widget)
{
  return gtk_tooltips_data_get((widget).gobj());
}


void Tooltips::force_window()
{
gtk_tooltips_force_window(gobj()); 
}


#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED


} // namespace Gtk


