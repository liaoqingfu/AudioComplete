// -*- c++ -*-
// Generated by gtkmmproc -- DO NOT MODIFY!
#ifndef _GTKMM_CELLLAYOUT_P_H
#define _GTKMM_CELLLAYOUT_P_H
#include <glibmm/private/interface_p.h>

#include <glibmm/private/interface_p.h>

namespace Gtk
{

class CellLayout_Class : public Glib::Interface_Class
{
public:
  typedef CellLayout CppObjectType;
  typedef GtkCellLayout BaseObjectType;
  typedef GtkCellLayoutIface BaseClassType;
  typedef Glib::Interface_Class CppClassParent;

  friend class CellLayout;

  const Glib::Interface_Class& init();

  static void iface_init_function(void* g_iface, void* iface_data);

  static Glib::ObjectBase* wrap_new(GObject*);

protected:

  //Callbacks (default signal handlers):
  //These will call the *_impl member methods, which will then call the existing default signal callbacks, if any.
  //You could prevent the original default signal handlers being called by overriding the *_impl method.

  //Callbacks (virtual functions):
  static void pack_start_vfunc_callback(GtkCellLayout* self, GtkCellRenderer* cell, gboolean expand);
  static void pack_end_vfunc_callback(GtkCellLayout* self, GtkCellRenderer* cell, gboolean expand);
  static void clear_vfunc_callback(GtkCellLayout* self);
  static void add_attribute_vfunc_callback(GtkCellLayout* self, GtkCellRenderer* cell, const gchar* attribute, gint column);
  static void clear_attributes_vfunc_callback(GtkCellLayout* self, GtkCellRenderer* cell);
  static void reorder_vfunc_callback(GtkCellLayout* self, GtkCellRenderer* cell, gint position);
};


} // namespace Gtk

#endif /* _GTKMM_CELLLAYOUT_P_H */

