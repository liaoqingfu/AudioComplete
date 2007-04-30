// Generated by gtkmmproc -- DO NOT MODIFY!


#include <gtkmm/plug.h>
#include <gtkmm/private/plug_p.h>

// -*- c++ -*-
/* $Id$ */

/* 
 *
 * Copyright 1998-2002 The gtkmm Development Team
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

#include <gtk/gtkplug.h>

namespace Gtk
{

Plug::Plug()
:
  Glib::ObjectBase(0), //Mark this class as gtkmmproc-generated, rather than a custom class, to allow vfunc optimisations.
  Gtk::Window(Glib::ConstructParams(plug_class_.init(), (char*) 0))
{
  gtk_plug_construct(gobj(), 0);
}

Plug::Plug(GdkNativeWindow socket_id)
:
  Glib::ObjectBase(0), //Mark this class as gtkmmproc-generated, rather than a custom class, to allow vfunc optimisations.
  Gtk::Window(Glib::ConstructParams(plug_class_.init(), (char*) 0))
{
  gtk_plug_construct(gobj(), socket_id);
}

Plug::Plug(const Glib::RefPtr<Gdk::Display>& display, GdkNativeWindow socket_id)
:
  Glib::ObjectBase(0), //Mark this class as gtkmmproc-generated, rather than a custom class, to allow vfunc optimisations.
  Gtk::Window(Glib::ConstructParams(plug_class_.init(), (char*) 0))
{
  gtk_plug_construct_for_display(gobj(), Glib::unwrap(display), socket_id);
}

} // namespace Gtk


namespace
{


static const Glib::SignalProxyInfo Plug_signal_embedded_info =
{
  "embedded",
  (GCallback) &Glib::SignalProxyNormal::slot0_void_callback,
  (GCallback) &Glib::SignalProxyNormal::slot0_void_callback
};


} // anonymous namespace


namespace Glib
{

Gtk::Plug* wrap(GtkPlug* object, bool take_copy)
{
  return dynamic_cast<Gtk::Plug *> (Glib::wrap_auto ((GObject*)(object), take_copy));
}

} /* namespace Glib */

namespace Gtk
{


/* The *_Class implementation: */

const Glib::Class& Plug_Class::init()
{
  if(!gtype_) // create the GType if necessary
  {
    // Glib::Class has to know the class init function to clone custom types.
    class_init_func_ = &Plug_Class::class_init_function;

    // This is actually just optimized away, apparently with no harm.
    // Make sure that the parent type has been created.
    //CppClassParent::CppObjectType::get_type();

    // Create the wrapper type, with the same class/instance size as the base type.
    register_derived_type(gtk_plug_get_type());

    // Add derived versions of interfaces, if the C type implements any interfaces:
  }

  return *this;
}

void Plug_Class::class_init_function(void* g_class, void* class_data)
{
  BaseClassType *const klass = static_cast<BaseClassType*>(g_class);
  CppClassParent::class_init_function(klass, class_data);

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
  klass->embedded = &embedded_callback;
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
}

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
void Plug_Class::embedded_callback(GtkPlug* self)
{
  CppObjectType *const obj = dynamic_cast<CppObjectType*>(
      Glib::ObjectBase::_get_current_wrapper((GObject*)self));

  // Non-gtkmmproc-generated custom classes implicitly call the default
  // Glib::ObjectBase constructor, which sets is_derived_. But gtkmmproc-
  // generated classes can use this optimisation, which avoids the unnecessary
  // parameter conversions if there is no possibility of the virtual function
  // being overridden:
  if(obj && obj->is_derived_())
  {
    #ifdef GLIBMM_EXCEPTIONS_ENABLED
    try // Trap C++ exceptions which would normally be lost because this is a C callback.
    {
    #endif //GLIBMM_EXCEPTIONS_ENABLED
      // Call the virtual member method, which derived classes might override.
      obj->on_embedded();
    #ifdef GLIBMM_EXCEPTIONS_ENABLED
    }
    catch(...)
    {
      Glib::exception_handlers_invoke();
    }
    #endif //GLIBMM_EXCEPTIONS_ENABLED
  }
  else
  {
    BaseClassType *const base = static_cast<BaseClassType*>(
        g_type_class_peek_parent(G_OBJECT_GET_CLASS(self)) // Get the parent class of the object class (The original underlying C class).
    );

    // Call the original underlying C function:
    if(base && base->embedded)
      (*base->embedded)(self);
  }
}
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED


Glib::ObjectBase* Plug_Class::wrap_new(GObject* o)
{
  return manage(new Plug((GtkPlug*)(o)));

}


/* The implementation: */

Plug::Plug(const Glib::ConstructParams& construct_params)
:
  Gtk::Window(construct_params)
{
  }

Plug::Plug(GtkPlug* castitem)
:
  Gtk::Window((GtkWindow*)(castitem))
{
  }

Plug::~Plug()
{
  destroy_();
}

Plug::CppClassType Plug::plug_class_; // initialize static member

GType Plug::get_type()
{
  return plug_class_.init().get_type();
}

GType Plug::get_base_type()
{
  return gtk_plug_get_type();
}


GdkNativeWindow Plug::get_id() const
{
  return gtk_plug_get_id(const_cast<GtkPlug*>(gobj()));
}


Glib::SignalProxy0< void > Plug::signal_embedded()
{
  return Glib::SignalProxy0< void >(this, &Plug_signal_embedded_info);
}


#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
void Gtk::Plug::on_embedded()
{
  BaseClassType *const base = static_cast<BaseClassType*>(
      g_type_class_peek_parent(G_OBJECT_GET_CLASS(gobject_)) // Get the parent class of the object class (The original underlying C class).
  );

  if(base && base->embedded)
    (*base->embedded)(gobj());
}
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED

#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED


} // namespace Gtk


