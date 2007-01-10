/* Crossmark processing library 
 * Copyright (C) 2006, Robert Staudinger <robert.staudinger@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*!
 * \file string-private.hh
 * \brief String class.
 */

#ifndef CM_STRING_PRIVATE_HH
#define CM_STRING_PRIVATE_HH

#include <glib.h>
#include <string>
#include <crossmark/features.hh>

namespace crossmark {

/*!
 * \brief String class with an UTF-8 compliant append method.
 */
class String : public std::string
{
public:
	String ()
	  : std::string ()
	{}

	String (gchar const *str)
	  : std::string (str)
	{}

	String (std::string &str)
	  : std::string (str)
	{}

	void append (gunichar c)
	{
		gchar buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
		gint  n_bytes;

		n_bytes = g_unichar_to_utf8 (c, buf);
		std::string::append (buf);
	}
};

};  // namespace crossmark

#endif /* CM_STRING_PRIVATE_HH */
