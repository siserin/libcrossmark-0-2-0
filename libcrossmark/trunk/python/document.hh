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

#ifndef DOCUMENT_HH
#define DOCUMENT_HH

#include <crossmark/crossmark.hh>

class Document : public crossmark::Document
{
public:
	virtual ~Document () {}

#ifndef SWIG
	virtual void pushStyle (crossmark::document::Style::Type type) { pushStyle ((int) type); }
	virtual void pushBlock (crossmark::document::Block::Type type) { pushBlock ((int) type); }
	virtual void pushLink (crossmark::document::Link::Type type, gchar const *target, gchar const *scroll) { pushLink (type, target, scroll); }
#endif

	virtual void pushDocument () {}
	virtual void popDocument () {}

	virtual void pushBlock (int type) {}
	virtual void popBlock () {}

	virtual void pushStyle (int type) {}
	virtual void popStyle () {}

	virtual void pushLink (int type, gchar const *target, gchar const *scroll) {}
	virtual void popLink () {}

	virtual void text (char const *str) {}
};

#endif // DOCUMENT_HH
