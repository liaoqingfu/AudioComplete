// Generated by gtkmmproc -- DO NOT MODIFY!


#include <pangomm/color.h>
#include <pangomm/private/color_p.h>

/* Copyright (C) 2002 The gtkmm Development Team
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

namespace Pango
{

/* For some unknown reason pango doesn't provide pango_color_new(). Let's define an
 * equivalent function ourself! */
PangoColor* _pango_color_new()
{
  return g_new(PangoColor, 1);
}

Color::operator bool()
{
  return gobj() != 0;
}

} /* namespace Pango */

namespace
{
} // anonymous namespace


namespace Glib
{

Pango::Color& wrap(PangoColor* object)
{
  return *reinterpret_cast<Pango::Color*>(object);
}

const Pango::Color& wrap(const PangoColor* object)
{
  return *reinterpret_cast<const Pango::Color*>(object);
}

} // namespace Glib


namespace Pango
{


// static
GType Color::get_type()
{
  return pango_color_get_type();
}

Color::Color()
{
  GLIBMM_INITIALIZE_STRUCT(gobject_, PangoColor);
}

Color::Color(const PangoColor* gobject)
{
  if(gobject)
    gobject_ = *gobject;
  else
    GLIBMM_INITIALIZE_STRUCT(gobject_, PangoColor);
}


 guint16 Color::get_red() const
{
  return gobj()->red;
}
 
 guint16 Color::get_green() const
{
  return gobj()->green;
}
 
 guint16 Color::get_blue() const
{
  return gobj()->blue;
}
 
 void Color::set_red(const guint16& value)
{
  gobj()->red = value;
}
 
 void Color::set_green(const guint16& value)
{
  gobj()->green = value;
}
 
 void Color::set_blue(const guint16& value)
{
  gobj()->blue = value;
}
 
bool Color::parse(const Glib::ustring& spec)
{
  return pango_color_parse(gobj(), spec.c_str());
}


} // namespace Pango


