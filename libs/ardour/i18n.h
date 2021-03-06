#ifndef __i18n_h__
#define __i18n_h__

#include "pbd/compose.h"
#include "pbd/convert.h"
#include "gettext.h"

#include <vector>
#include <string>

#define _(Text)  dgettext (PACKAGE,Text)
#define N_(Text) gettext_noop (Text)
#define X_(Text) Text
#define I18N(Array) PBD::internationalize (PACKAGE, Array)
/** Use this to translate strings that have different meanings in different places.
 *  Text should be of the form Context|Message.
 */
#define S_(Text) PBD::sgettext (PACKAGE, Text)

/** Use this to translate strings with plural forms
 */
#define P_(Singular,Plural,HowMany) dngettext (PACKAGE, (Singular), (Plural), (HowMany))

#endif // __i18n_h__
