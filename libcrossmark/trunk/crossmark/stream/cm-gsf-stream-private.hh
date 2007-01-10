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
 * \file cm-gsf-stream-private.hh
 * \brief Crossmark libgsf support.
 * \internal
 */

#ifndef CM_GSF_STREAM_PRIVATE_HH
#define CM_GSF_STREAM_PRIVATE_HH

#include <glib.h>
#include <gsf/gsf.h>
#include <crossmark/cm-features.hh>
#include <crossmark/stream/cm-stream.hh>

namespace crossmark {

namespace stream {

/*!
 * \internal
 * \brief UTF-8 compliant input stream.
 */
class GsfInput : public Input 
{
public:
	GsfInput (gchar const *file);
	GsfInput (::GsfInput *input);
	virtual ~GsfInput ();
	virtual gunichar read ();

private:
	::GsfInput *_input;
};

/*!
 * \internal
 * \brief UTF-8 compliant output stream.
 */
class GsfOutput : public Output
{
public:
	GsfOutput (gchar const *file);
	GsfOutput (::GsfOutput *output);
	virtual ~GsfOutput ();
	virtual gboolean write (gunichar c);
	virtual gboolean write (gchar const *s);

protected:
	::GsfOutput *_output;
};

}; // namespace stream

}; // namespace crossmark

#endif // CM_GSF_STREAM_PRIVATE_HH
