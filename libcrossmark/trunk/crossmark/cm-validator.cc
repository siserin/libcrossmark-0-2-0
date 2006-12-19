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
  : _reader (reader), 
    _isBold (FALSE),
    _isItalic (FALSE),
    _isMonospace (FALSE),
    _isUnderline (FALSE)
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

/*!
 * \todo Check if we're already in style of that type and add fallback instead if yes.
	 Wait! First we need to know which style is "stronger", the first or the second.
	 If it was the second we'd just add the fallback for a nested pushStyle.
 */
void
Validator::pushStyle (document::Style::Type type)
{
	validators::methods::Method *method;

	if (_isBold && type == document::Style::BOLD ||
	    _isItalic && type == document::Style::ITALIC ||
	    _isMonospace && type == document::Style::MONOSPACE ||
	    _isUnderline && type == document::Style::UNDERLINE) {

		method = validators::methods::Text::fallback (_reader, type);

	} else {
		method = new validators::methods::PushStyle (_reader, type);
	}

	_methods.push_back (method);

	_isBold = type == document::Style::BOLD;
	_isItalic = type == document::Style::ITALIC;
	_isMonospace = type == document::Style::MONOSPACE;
	_isUnderline = type == document::Style::UNDERLINE;
}

/*!
 * For now we don't allow nesting of the same style (e.g. "foo *bar *baz* bar* foo").
 */
void
Validator::cancelStyle (document::Style::Type type)
{
	std::list<validators::methods::Method *>::reverse_iterator iter;
	validators::methods::PushStyle *pushStyle;
	validators::methods::PopStyle *popStyle;
	
	iter = _methods.rbegin ();
	while (iter != _methods.rend ()) {
		// search backwards until start of style and cancel it
		if ((*iter)->getClass () == validators::methods::Method::PUSH_STYLE) {
			pushStyle = dynamic_cast<validators::methods::PushStyle *> (*iter);
			if (pushStyle->getType () == type) {
				std::list<validators::methods::Method *>::iterator ins = _methods.erase (iter.base ());
				validators::methods::Method *fallback = pushStyle->createFallback ();
				_methods.insert (ins, fallback);
				delete pushStyle;
				break;
			}			
		} else if ((*iter)->getClass () == validators::methods::Method::POP_STYLE) {
			popStyle = dynamic_cast<validators::methods::PopStyle *> (*iter);
			if (popStyle->getType () == type) {
				// hitting upon closed style of same type
				// no need to go further or do anything.
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
	validators::methods::Method *method;

	if (_isBold && type == document::Style::BOLD ||
	    _isItalic && type == document::Style::ITALIC ||
	    _isMonospace && type == document::Style::MONOSPACE ||
	    _isUnderline && type == document::Style::UNDERLINE) {

		method = new validators::methods::PopStyle (_reader, type);

	} else {
		method = validators::methods::Text::fallback (_reader, type);
	}

	_methods.push_back (method);

	_isBold = (_isBold && type == document::Style::BOLD) ? FALSE : _isBold;
	_isItalic = (_isItalic && type == document::Style::ITALIC) ? FALSE : _isItalic;
	_isMonospace = (_isMonospace && type == document::Style::MONOSPACE) ? FALSE : _isMonospace;
	_isUnderline = (_isUnderline && type == document::Style::UNDERLINE) ? FALSE : _isUnderline;
}

/*!
 * \todo Transform the current structure into a heading structure if type is 
	 a heading type and we're in a block already.
 */
void
Validator::pushBlock (document::Block::Type type)
{
	_methods.push_back (new validators::methods::PushBlock (_reader, type));
}

/*!
 * \todo Cancel pending styles.
 */
void
Validator::popBlock ()
{
	cancelStyle (document::Style::BOLD);
	cancelStyle (document::Style::ITALIC);
	cancelStyle (document::Style::MONOSPACE);
	cancelStyle (document::Style::UNDERLINE);

	std::list<validators::methods::Method *>::iterator iter;
	validators::methods::Method *method;
	iter = _methods.begin ();
	while (iter != _methods.end ()) {
		method = *iter;
		(*method) ();
		delete method;
		iter++;
	}
	_reader.popBlock ();

	_methods.clear ();
}
