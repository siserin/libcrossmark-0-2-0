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
 * \file cm-stdio-stream-private.hh
 * \brief Basic I/O streams.
 * \internal
 */

#ifndef CM_STDIO_STREAM_PRIVATE_HH
#define CM_STDIO_STREAM_PRIVATE_HH

#include <glib.h>
#include <stdio.h>
#include <string>
#include <crossmark/cm-stream.hh>

namespace crossmark {

namespace streams {

/*!
 * \internal
 * \brief Basic input stream.
 *
 * \warning This class is for testing and not guaranteed to be UTF-8 compliant.
 */
class StdInput : public Input 
{
public:
	StdInput (const std::string &file);
	virtual ~StdInput ();
	virtual gunichar getChar ();

protected:
	FILE *_istream;
};

/*!
 * \internal
 * \brief Basic output stream.
 *
 * \warning This class is for testing and not guaranteed to be UTF-8 compliant.
 */
class StdOutput : public Output
{
public:
	StdOutput (const  std::string &file);
	virtual ~StdOutput ();
	virtual void write (gunichar c);
	virtual void write (const gchar *s);

protected:
	FILE *_ostream;
};

}; // namespace streams

}; // namespace crossmark

#endif // CM_STDIO_STREAM_PRIVATE_HH
