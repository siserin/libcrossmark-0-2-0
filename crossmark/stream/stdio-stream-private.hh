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
 * \file stdio-stream-private.hh
 * \brief Basic I/O streams.
 * \internal
 */

#ifndef CM_STDIO_STREAM_PRIVATE_HH
#define CM_STDIO_STREAM_PRIVATE_HH

#include <glib.h>
#include <stdio.h>
#include <crossmark/features.hh>
#include <crossmark/stream/stream.hh>

namespace crossmark {

namespace stream {

/*!
 * \internal
 * \brief Basic input stream.
 *
 * \warning This class is for testing and not guaranteed to be UTF-8 compliant.
 */
class StdInput : public Input 
{
public:
	StdInput (gchar const *file);
	StdInput (FILE *istream);
	virtual ~StdInput ();
	virtual gunichar read ();

protected:
	FILE     *_istream;
	gboolean  _ownStream;
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
	StdOutput (gchar const *file);
	StdOutput (FILE *ostream);
	virtual ~StdOutput ();
	virtual gboolean write (gunichar c);
	virtual gboolean write (gchar const *s);

protected:
	FILE     *_ostream;
	gboolean  _ownStream;
};

}; // namespace stream

}; // namespace crossmark

#endif // CM_STDIO_STREAM_PRIVATE_HH
