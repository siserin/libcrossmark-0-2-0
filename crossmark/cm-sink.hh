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
 * \file cm-sink.hh
 * \brief Crossmark output.
 */

#ifndef CM_SINK_HH
#define CM_SINK_HH

#include <stack>
#include <string>
#include <crossmark/cm-features.hh>
#include <crossmark/cm-document.hh>
#include <crossmark/cm-stream.hh>

namespace crossmark {

/*!
 * \brief Writer must implement all specific document interfaces.
 *
 * \note Maybe later we'll split mandatory and optional ones.
 *
 * \todo Does the writer validate?
 * \todo Implement state machine.
 */
class Writer : public Document
{
friend class Sink;
public:
	virtual ~Writer ();

	virtual void text (const std::string &text);

	virtual void pushStyle (document::Style::Type type);
	virtual void popStyle (document::Style::Type type);

	// TODO implement

private:
	void setStream (FILE *ostream);

	FILE *_ostream;
};

/*!
 * \brief Wrapper for opening and closing files or streams to write to.
 */
class Sink
{
public:
	Sink (const std::string &file, 
	      Writer &writer);

	virtual ~Sink ();
};

};  // namespace crossmark

#endif /* CM_SINK_HH */
