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

#include <crossmark/cm-validator-private.hh>

using namespace crossmark;

Validator::Validator (Reader &reader)
  : _reader (reader)
{}

Validator::~Validator ()
{}

void
Validator::pushDocument ()
{
	_reader.pushDocument ();
}

void
Validator::popDocument ()
{
	_reader.popDocument ();
}

void
Validator::text (const std::string &text)
{
	_methods.push_back (new validators::methods::Text (_reader, text.c_str ()));
}

void
Validator::pushStyle (document::Style::Type type)
{
	_methods.push_back (new validators::methods::PushStyle (_reader, type));
}

/*!
 * \todo Cancel style.
 */
void
Validator::cancelStyle (document::Style::Type type)
{

}

/*!
 * \todo Check if style is valid and either pop or cancel it.
 */
void
Validator::popStyle ()
{

}

void
Validator::pushStructure (document::Structure::Type type)
{
	_methods.push_back (new validators::methods::PushStructure (_reader, type));
}

/*!
 * \todo Transform the current structure into a heading structure.
	 Maybe this should be named more clearly. For H2, H3 we need to 
	 push a new structure, so push one if we're not into one already.
 */
void
Validator::pushHeadingStructure (int level)
{

}

/*!
 * \todo Cancel pending styles.
 */
void
Validator::popStructure ()
{

}
