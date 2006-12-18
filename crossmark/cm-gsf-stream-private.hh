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
#include <gsf/gsf-input-textline.h>
#include <string>
#include <crossmark/cm-stream.hh>

namespace crossmark {

namespace streams {

/*!
 * \internal
 * \brief UTF-8 compliant input stream.
 */
class GsfInput : public Input 
{
public:
	GsfInput (const std::string &file);
	virtual ~GsfInput ();
	virtual gunichar getChar ();

protected:
	GsfInputTextline *_input;

private:
	guint8 		*_line;
	const guint8 	*_iter;
	guint   	 _len;
};

/*!
 * \internal
 * \brief UTF-8 compliant output stream.
 */
class GsfOutput : public Output
{
public:
	GsfOutput (const  std::string &file);
	virtual ~GsfOutput ();
	virtual void write (gunichar c);
	virtual void write (const gchar *s);

protected:
	GsfOutput *_output;
};

}; // namespace streams

}; // namespace crossmark

#endif // CM_GSF_STREAM_PRIVATE_HH
