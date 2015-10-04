/*
    Copyright (C) 1998-99 Paul Barton-Davis

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

#ifndef __libmisc_textreceiver_h__
#define __libmisc_textreceiver_h__

#include <string>

#include "pbd/libpbd_visibility.h"
#include "receiver.h"

class LIBPBD_API TextReceiver : public Receiver
{
  public:
	TextReceiver (const std::string &n);

  protected:
	void receive (Transmitter::Channel, const char *);

  private:
	std::string name;
};

#endif //__libmisc_textreceiver_h__
