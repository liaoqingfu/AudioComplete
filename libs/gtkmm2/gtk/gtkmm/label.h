// -*- c++ -*-
// Generated by gtkmmproc -- DO NOT MODIFY!
#ifndef _GTKMM_LABEL_H
#define _GTKMM_LABEL_H


#include <glibmm.h>

/* $Id$ */

/* Copyright(C) 1998-2002 The gtkmm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or(at your option) any later version.
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


#include <gtkmm/misc.h>
#include <pangomm/attrlist.h>


#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct _GtkLabel GtkLabel;
typedef struct _GtkLabelClass GtkLabelClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


namespace Gtk
{ class Label_Class; } // namespace Gtk
namespace Gtk
{

class Menu;

/** A widget that displays a small to medium amount of text.
 *
 * A simple setable widget for holding a Glib::ustring.
 * @ingroup Widgets
 */

class Label : public Misc
{
  public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  typedef Label CppObjectType;
  typedef Label_Class CppClassType;
  typedef GtkLabel BaseObjectType;
  typedef GtkLabelClass BaseClassType;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

  virtual ~Label();

#ifndef DOXYGEN_SHOULD_SKIP_THIS

private:
  friend class Label_Class;
  static CppClassType label_class_;

  // noncopyable
  Label(const Label&);
  Label& operator=(const Label&);

protected:
  explicit Label(const Glib::ConstructParams& construct_params);
  explicit Label(GtkLabel* castitem);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static GType get_type()      G_GNUC_CONST;
  static GType get_base_type() G_GNUC_CONST;
#endif

  ///Provides access to the underlying C GtkObject.
  GtkLabel*       gobj()       { return reinterpret_cast<GtkLabel*>(gobject_); }

  ///Provides access to the underlying C GtkObject.
  const GtkLabel* gobj() const { return reinterpret_cast<GtkLabel*>(gobject_); }


public:
  //C++ methods used to invoke GTK+ virtual functions:
#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

protected:
  //GTK+ Virtual Functions (override these to change behaviour):
#ifdef GLIBMM_VFUNCS_ENABLED
#endif //GLIBMM_VFUNCS_ENABLED

  //Default Signal Handlers::
#ifdef GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED
  virtual void on_populate_popup(Menu* menu);
#endif //GLIBMM_DEFAULT_SIGNAL_HANDLERS_ENABLED


private:

  
public:
  Label();
  explicit Label(const Glib::ustring& label, bool mnemonic = false);

  /** This constructor is a shortcut for often used code
   * when you want to create a label with alignment different
   * than default one.
   * @code
   * Gtk::Label label(text, x, y, mnemonic);
   * @endcode
   * is equivalent to:
   * @code
   * Gtk::Label label(text, mnemonic);
   * label.set_alignment(x, y);
   * @endcode
   */
  Label(const Glib::ustring& label, float xalign, float yalign, bool mnemonic = false);

  /** This constructor is a shortcut for often used code
   * when you want to create a label with alignment different
   * than default one.
   * @code
   * Gtk::Label label(text, x, y, mnemonic);
   * @endcode
   * is equivalent to:
   * @code
   * Gtk::Label label(text, mnemonic);
   * label.set_alignment(x, y);
   * @endcode
   */
  Label(const Glib::ustring& label, AlignmentEnum xalign, AlignmentEnum yalign = ALIGN_CENTER, bool mnemonic = false);


  /// Doesn't use markup.
  
  /** Sets the text within the Gtk::Label widget.  It overwrites any text that
   * was there before.  
   * 
   * This will also clear any previously set mnemonic accelerators.
   * @param str The text you want to set.
   */
  void set_text(const Glib::ustring &str);
  
  /** Fetches the text from a label widget, as displayed on the
   * screen. This does not include any embedded underlines
   * indicating mnemonics or Pango markup. (See get_label())
   * @return The text in the label widget.
   */
  Glib::ustring get_text() const;

  
  /** Sets a Pango::AttrList; the attributes in the list are applied to the
   * label text. The attributes set with this function will be ignored
   * if the "use_underline" property or the "use_markup" property
   * is <tt>true</tt>.
   * @param attrs A Pango::AttrList.
   */
  void set_attributes(Pango::AttrList& attrs);
  
  /** Gets the attribute list that was set on the label using
   * set_attributes(), if any. This function does
   * not reflect attributes that come from the labels markup
   * (see set_markup()). If you want to get the
   * effective attributes for the label, use
   * pango_layout_get_attribute (gtk_label_get_layout (label)).
   * @return The attribute list, or <tt>0</tt> if none was set.
   */
  Pango::AttrList get_attributes() const;

  
  /** Sets the text of the label. The label is interpreted as
   * including embedded underlines and/or Pango markup depending
   * on the values of label->use_underline and label->use_markup.
   * @param str The new text to set for the label.
   */
  void set_label(const Glib::ustring& str);
  
  /** Fetches the text from a label widget including any embedded
   * underlines indicating mnemonics and Pango markup. (See
   * get_text()).
   * @return The text of the label widget..
   */
  Glib::ustring get_label() const;
  
  /** Parses @a str  which is marked up with the Pango text markup language, setting the
   * label's text and attribute list based on the parse results.  If the @a str  is
   * external data, you may need to escape it with Glib::markup_escape_text() or
   * Glib::markup_printf_escaped()&lt;!-- --&gt;:
   * @code
   * char *markup;
   * &lt;!-- --&gt;
   * markup = g_markup_printf_escaped ("&lt;span style=\"italic\"&gt;&percnt;s&lt;/span&gt;", str);
   * gtk_label_set_markup (GTK_LABEL (label), markup);
   * g_free (markup);
   * @endcode
   * @param str A markup string (see Pango markup format).
   */
  void set_markup(const Glib::ustring& str);
  
  /** Sets whether the text of the label contains markup in Pango's text markup
   * language. See set_markup().
   * @param setting <tt>true</tt> if the label's text should be parsed for markup.
   */
  void set_use_markup(bool setting = true);
  
  /** Returns whether the label's text is interpreted as marked up with
   * the Pango text markup
   * language. See set_use_markup().
   * @return <tt>true</tt> if the label's text will be parsed for markup.
   */
  bool get_use_markup() const;
  
  /** If true, an underline in the text indicates the next character should be
   * used for the mnemonic accelerator key.
   * @param setting <tt>true</tt> if underlines in the text indicate mnemonics.
   */
  void set_use_underline(bool setting = true);
  
  /** Returns whether an embedded underline in the label indicates a
   * mnemonic. See set_use_underline().
   * @return <tt>true</tt> whether an embedded underline in the label indicates
   * the mnemonic accelerator keys.
   */
  bool get_use_underline() const;

  
  /** Parses @a str  which is marked up with the Pango text markup language,
   * setting the label's text and attribute list based on the parse results.
   * If characters in @a str  are preceded by an underscore, they are underlined
   * indicating that they represent a keyboard accelerator called a mnemonic.
   * 
   * The mnemonic key can be used to activate another widget, chosen automatically,
   * or explicitly using set_mnemonic_widget().
   * @param str A markup string (see Pango markup format).
   */
  void set_markup_with_mnemonic(const Glib::ustring& str);
  
  /** If the label has been set so that it has an mnemonic key this function
   * returns the keyval used for the mnemonic accelerator. If there is no
   * mnemonic set up it returns #GDK_VoidSymbol.
   * @return GDK keyval usable for accelerators, or #GDK_VoidSymbol.
   */
  guint get_mnemonic_keyval() const;
  
  /** If the label has been set so that it has an mnemonic key (using
   * i.e.\  set_markup_with_mnemonic(),
   * set_text_with_mnemonic(), new_with_mnemonic()
   * or the "use_underline" property) the label can be associated with a
   * widget that is the target of the mnemonic. When the label is inside
   * a widget (like a Gtk::Button or a Gtk::Notebook tab) it is
   * automatically associated with the correct widget, but sometimes
   * (i.e. when the target is a Gtk::Entry next to the label) you need to
   * set it explicitly using this function.
   * 
   * The target widget will be accelerated by emitting "mnemonic_activate" on it.
   * The default handler for this signal will activate the widget if there are no
   * mnemonic collisions and toggle focus between the colliding widgets otherwise.
   * @param widget The target Gtk::Widget.
   */
  void set_mnemonic_widget(Widget& widget);
  
  /** Retrieves the target of the mnemonic (keyboard shortcut) of this
   * label. See set_mnemonic_widget().
   * @return The target of the label's mnemonic, or <tt>0</tt> if none
   * has been set and the default algorithm will be used.
   */
  Widget* get_mnemonic_widget();
  
  /** Retrieves the target of the mnemonic (keyboard shortcut) of this
   * label. See set_mnemonic_widget().
   * @return The target of the label's mnemonic, or <tt>0</tt> if none
   * has been set and the default algorithm will be used.
   */
  const Widget* get_mnemonic_widget() const;
  
  /** Sets the label's text from the string @a str .
   * If characters in @a str  are preceded by an underscore, they are underlined
   * indicating that they represent a keyboard accelerator called a mnemonic.
   * The mnemonic key can be used to activate another widget, chosen automatically,
   * or explicitly using set_mnemonic_widget().
   * @param str A string.
   */
  void set_text_with_mnemonic(const Glib::ustring& str);
  
  /** Sets the alignment of the lines in the text of the label relative to
   * each other.  Gtk::JUSTIFY_LEFT is the default value when the
   * widget is first created with new(). If you instead want
   * to set the alignment of the label as a whole, use
   * Gtk::Misc::set_alignment() instead. set_justify() has no
   * effect on labels containing only a single line.
   * @param jtype A Gtk::Justification.
   */
  void set_justify(Justification jtype);
  
  /** Returns the justification of the label. See set_justify().
   * @return Gtk::Justification.
   */
  Justification get_justify() const;

//TODO: Default value?:
  
  /** Sets the mode used to ellipsize (add an ellipsis: "...") to the text if there
   * is not enough space to render the entire string.
   * 
   * @newin2p6
   * @param mode A Pango::EllipsizeMode.
   */
  void set_ellipsize(Pango::EllipsizeMode mode);
  
  /** Returns the ellipsizing position of the label. See set_ellipsize().
   * @return Pango::EllipsizeMode
   * 
   * @newin2p6.
   */
  Pango::EllipsizeMode get_ellipsize() const;

  
  /** Sets the desired width in characters of @a label  to @a n_chars .
   * 
   * @newin2p6
   * @param n_chars The new desired width, in characters.
   */
  void set_width_chars(int n_chars);
  
  /** Retrieves the desired width of @a label , in characters. See
   * set_width_chars().
   * @return The width of the label in characters.
   * 
   * @newin2p6.
   */
  int get_width_chars() const;

  
  /** Sets the desired maximum width in characters of @a label  to @a n_chars .
   * 
   * @newin2p6
   * @param n_chars The new desired maximum width, in characters.
   */
  void set_max_width_chars(int n_chars);
  
  /** Retrieves the desired maximum width of @a label , in characters. See
   * set_width_chars().
   * @return The maximum width of the label in characters.
   * 
   * @newin2p6.
   */
  int get_max_width_chars() const;

  
  void set_pattern(const Glib::ustring& pattern);
  
  /** Toggles line wrapping within the Gtk::Label widget.  <tt>true</tt> makes it break
   * lines if text exceeds the widget's size.  <tt>false</tt> lets the text get cut off
   * by the edge of the widget if it exceeds the widget size.
   * @param wrap The setting.
   */
  void set_line_wrap(bool wrap = true);
  
  /** Returns whether lines in the label are automatically wrapped. See set_line_wrap().
   * @return <tt>true</tt> if the lines of the label are automatically wrapped.
   */
  bool get_line_wrap() const;
  
  void set_line_wrap_mode(Pango::WrapMode wrap_mode);
  
  Pango::WrapMode get_line_wrap_mode() const;
  
  /** Selectable labels allow the user to select text from the label, for
   * copy-and-paste.
   * @param setting <tt>true</tt> to allow selecting text in the label.
   */
  void set_selectable(bool setting = true);
  
  /** Gets the value set by set_selectable().
   * @return <tt>true</tt> if the user can copy text from the label.
   */
  bool get_selectable() const;
  
  /** Sets the angle of rotation for the label. An angle of 90 reads from
   * from bottom to top, an angle of 270, from top to bottom. The angle
   * setting for the label is ignored if the label is selectable,
   * wrapped, or ellipsized.
   * 
   * @newin2p6
   * @param angle The angle that the baseline of the label makes with
   * the horizontal, in degrees, measured counterclockwise.
   */
  void set_angle(double angle);
  
  /** Gets the angle of rotation for the label. See
   * gtk_label_set_angle.
   * @return The angle of rotation for the label
   * 
   * @newin2p6.
   */
  double get_angle() const;
  
  /** Selects a range of characters in the label, if the label is selectable.
   * See set_selectable(). If the label is not selectable,
   * this function has no effect. If @a start_offset  or
   *  @a end_offset  are -1, then the end of the label will be substituted.
   * @param start_offset Start offset (in characters not bytes).
   * @param end_offset End offset (in characters not bytes).
   */
  void select_region(int start_offset, int end_offset);

  /** Selects a range of characters in the label, from @a start_offset to the end,
   * if the label is selectable.
   * See gtk_label_set_selectable(). If the label is not selectable,
   * this function has no effect.
   *
   * @param start_offset Start offset (in characters not bytes)
   */
  void select_region(int start_offset);
  
  
  /** Gets the selected range of characters in the label, returning <tt>true</tt>
   * if there's a selection.
   * @param start Return location for start of selection, as a character offset.
   * @param end Return location for end of selection, as a character offset.
   * @return <tt>true</tt> if selection is non-empty.
   */
  bool get_selection_bounds(int& start, int& end) const;

  
  /** Gets the Pango::Layout used to display the label.
   * The layout is useful to e.g. convert text positions to
   * pixel positions, in combination with get_layout_offsets().
   * @return The Pango::Layout for this label.
   */
  Glib::RefPtr<Pango::Layout> get_layout();
  
  /** Gets the Pango::Layout used to display the label.
   * The layout is useful to e.g. convert text positions to
   * pixel positions, in combination with get_layout_offsets().
   * @return The Pango::Layout for this label.
   */
  Glib::RefPtr<const Pango::Layout> get_layout() const;
  
  /** Obtains the coordinates where the label will draw the Pango::Layout
   * representing the text in the label; useful to convert mouse events
   * into coordinates inside the Pango::Layout, e.g.\ to take some action
   * if some part of the label is clicked. Of course you will need to
   * create a Gtk::EventBox to receive the events, and pack the label
   * inside it, since labels are a Gtk::NO_WINDOW widget. Remember
   * when using the Pango::Layout functions you need to convert to
   * and from pixels using PANGO_PIXELS() or Pango::SCALE.
   * @param x Location to store X offset of layout, or <tt>0</tt>.
   * @param y Location to store Y offset of layout, or <tt>0</tt>.
   */
  void get_layout_offsets(int& x, int& y) const;
  
  
  /** Sets whether the label is in single line mode.
   * 
   * @newin2p6
   * @param single_line_mode <tt>true</tt> if the label should be in single line mode.
   */
  void set_single_line_mode(bool single_line_mode = true);
  
  /** Returns whether the label is in single line mode.
   * @return <tt>true</tt> when the label is in single line mode.
   * 
   * @newin2p6.
   */
  bool get_single_line_mode() const;

  
/**
   * @par Prototype:
   * <tt>void %populate_popup(Menu* menu)</tt>
   */

  Glib::SignalProxy1< void,Menu* > signal_populate_popup();


  //Keybinding signals:
  
  
  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The text of the label.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Glib::ustring> property_label() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The text of the label.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Glib::ustring> property_label() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** A list of style attributes to apply to the text of the label.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Pango::AttrList> property_attributes() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** A list of style attributes to apply to the text of the label.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Pango::AttrList> property_attributes() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The text of the label includes XML markup. See pango_parse_markup.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<bool> property_use_markup() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The text of the label includes XML markup. See pango_parse_markup.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<bool> property_use_markup() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** If set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<bool> property_use_underline() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** If set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<bool> property_use_underline() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The alignment of the lines in the text of the label relative to each other. This does NOT affect the alignment of the label within its allocation. See GtkMisc::xalign for that.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Justification> property_justify() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The alignment of the lines in the text of the label relative to each other. This does NOT affect the alignment of the label within its allocation. See GtkMisc::xalign for that.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Justification> property_justify() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** A string with _ characters in positions correspond to characters in the text to underline.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_WriteOnly<Glib::ustring> property_pattern() ;
#endif //#GLIBMM_PROPERTIES_ENABLED


  #ifdef GLIBMM_PROPERTIES_ENABLED
/** If set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<bool> property_wrap() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** If set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<bool> property_wrap() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** If wrap is set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Pango::WrapMode> property_wrap_mode() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** If wrap is set
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Pango::WrapMode> property_wrap_mode() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** Whether the label text can be selected with the mouse.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<bool> property_selectable() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** Whether the label text can be selected with the mouse.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<bool> property_selectable() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The mnemonic accelerator key for this label.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<guint> property_mnemonic_keyval() const;
#endif //#GLIBMM_PROPERTIES_ENABLED


  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The widget to be activated when the label's mnemonic key is pressed.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Widget*> property_mnemonic_widget() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The widget to be activated when the label's mnemonic key is pressed.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Widget*> property_mnemonic_widget() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The current position of the insertion cursor in chars.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<int> property_cursor_position() const;
#endif //#GLIBMM_PROPERTIES_ENABLED


  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The position of the opposite end of the selection from the cursor in chars.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<int> property_selection_bound() const;
#endif //#GLIBMM_PROPERTIES_ENABLED


  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The preferred place to ellipsize the string
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<Pango::EllipsizeMode> property_ellipsize() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The preferred place to ellipsize the string
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<Pango::EllipsizeMode> property_ellipsize() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The desired width of the label
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<int> property_width_chars() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The desired width of the label
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<int> property_width_chars() const;
#endif //#GLIBMM_PROPERTIES_ENABLED
    
  #ifdef GLIBMM_PROPERTIES_ENABLED
/** Whether the label is in single line mode.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<bool> property_single_line_mode() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** Whether the label is in single line mode.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<bool> property_single_line_mode() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** Angle at which the label is rotated.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<double> property_angle() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** Angle at which the label is rotated.
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<double> property_angle() const;
#endif //#GLIBMM_PROPERTIES_ENABLED

  #ifdef GLIBMM_PROPERTIES_ENABLED
/** The desired maximum width of the label
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy<int> property_max_width_chars() ;
#endif //#GLIBMM_PROPERTIES_ENABLED

#ifdef GLIBMM_PROPERTIES_ENABLED
/** The desired maximum width of the label
   *
   * You rarely need to use properties because there are get_ and set_ methods for almost all of them.
   * @return A PropertyProxy that allows you to get or set the property of the value, or receive notification when
   * the value of the property changes.
   */
  Glib::PropertyProxy_ReadOnly<int> property_max_width_chars() const;
#endif //#GLIBMM_PROPERTIES_ENABLED


};

}  // namespace Gtk


namespace Glib
{
  /** @relates Gtk::Label
   * @param object The C instance
   * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
   * @result A C++ instance that wraps this C instance.
   */
  Gtk::Label* wrap(GtkLabel* object, bool take_copy = false);
} //namespace Glib


#endif /* _GTKMM_LABEL_H */

