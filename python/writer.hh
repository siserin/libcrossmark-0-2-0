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

#ifndef WRITER_HH
#define WRITER_HH

#include <crossmark/crossmark.hh>

class Writer : public crossmark::Writer
{
public:
	Writer (char const *file)
	  : crossmark::Writer (file)
	{}

	virtual ~Writer () {}

	virtual void pushDocument () { crossmark::Writer::pushDocument (); }
	virtual void popDocument () { crossmark::Writer::popDocument (); }

	virtual void pushBlock (int type) { crossmark::Writer::pushBlock ((crossmark::document::Block::Type) type); }
	virtual void popBlock () { crossmark::Writer::popBlock (); }

	virtual void pushStyle (int type) { crossmark::Writer::pushStyle ((crossmark::document::Style::Type) type); }
	virtual void popStyle () { crossmark::Writer::popStyle (); }

	virtual void pushLink (int type, gchar const *target, gchar const *scroll) { crossmark::Writer::pushLink ((crossmark::document::Link::Type) type, target, scroll); }
	virtual void popLink () { crossmark::Writer::popLink (); }

	virtual void text (char const *str) { crossmark::Writer::text (str); }
};

#endif // WRITER_HH
