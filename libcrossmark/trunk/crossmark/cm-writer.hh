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
 * \file cm-writer.hh
 * \brief Crossmark output.
 */

#ifndef CM_WRITER_HH
#define CM_WRITER_HH

#include <glib.h>
#include <stack>
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
public:
	Writer (gchar const *file);
	Writer (stream::Output &ostream);

	virtual ~Writer ();

	virtual void pushDocument ();
	virtual void popDocument ();

	virtual void text (gchar const *str);

	virtual void pushStyle (document::Style::Type type);
	virtual void popStyle (document::Style::Type type);

	virtual void pushBlock (document::Block::Type type);
	virtual void popBlock ();

private:
	stream::Output 		&_ostream;
	gboolean		 _ownStream;
	document::Block::Type 	 _currentBlock;
};

};  // namespace crossmark

#endif /* CM_WRITER_HH */
