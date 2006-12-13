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

#ifndef CM_STREAM_HH
#define CM_STREAM_HH

#include <glib.h>

namespace crossmark {

namespace streams {

class Input 
{
public:
	virtual ~Input () {}
	virtual gunichar getChar () = 0;
};

class Output 
{
public:
	virtual ~Output () {};
	virtual void write (gunichar c) = 0;
	virtual void write (const gchar *s) = 0;
};

}; // namespace streams

}; // namespace crossmark

#endif // CM_STREAM_HH
