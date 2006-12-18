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

#include "config.h"
#include "cm-validator-private.hh"

using namespace crossmark;

Validator::Validator (Document &reader)
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
 * For now we don't allow nesting of the same style (e.g. "foo *bar *baz* bar* foo").
 * \todo Cancel style.
 */
void
Validator::cancelStyle (document::Style::Type type)
{
	std::list<validators::methods::Method *>::reverse_iterator iter;
	validators::methods::PushStyle *pushStyle;
	validators::methods::PopStyle *popStyle;
	
	iter = _methods.rbegin ();
	while (iter != _methods.rend ()) {
		if ((*iter)->getClass () == validators::methods::Method::PUSH_STYLE) {
			pushStyle = dynamic_cast<validators::methods::PushStyle *> (*iter);
			if (pushStyle->getType () == type) {
				// TODO replace with text
				break;
			}			
		} else if ((*iter)->getClass () == validators::methods::Method::POP_STYLE) {
			popStyle = dynamic_cast<validators::methods::PopStyle *> (*iter);
			if (popStyle->getType () == type) {
				// hitting upon closed style of same type
				// no need to go further
				break;
			}
		}
		iter++;
	}
}

/*!
 * \todo Check if style is valid and either pop or cancel it.
	 Hmm, probably we need to pass the style here too, to correctly
	 unwind nested 
 */
void
Validator::popStyle (document::Style::Type type)
{

}

void
Validator::pushBlock (document::Block::Type type)
{
	_methods.push_back (new validators::methods::PushBlock (_reader, type));
}

/*!
 * \todo Transform the current structure into a heading structure.
	 Maybe this should be named more clearly. For H2, H3 we need to 
	 push a new structure, so push one if we're not into one already.
 */
void
Validator::pushHeading (int level)
{

}

/*!
 * \todo Cancel pending styles.
 */
void
Validator::popBlock ()
{

}
