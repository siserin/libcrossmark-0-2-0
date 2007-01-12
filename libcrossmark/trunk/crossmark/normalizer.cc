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

#include "normalizer-private.hh"
// DEBUG
#include <iostream>

#if 0
//#ifdef DEBUG
#include <iostream>
#define _dump(M) dump(M)
#define TRACE(M) fprintf(stderr,"Normalizer::");fprintf(stderr,M);fprintf(stderr,"\n");
#else
#define TRACE(M)
#define _dump(M)
#endif

using namespace crossmark;
using namespace crossmark::normalizer;

/*!
 * Create normalizer for reader.
 */
Normalizer::Normalizer (Document &reader)
  : _reader (reader), 
    _isBold (FALSE),
    _isItalic (FALSE),
    _isMonospace (FALSE),
    _isUnderline (FALSE)
{}

/*!
 * Dtor.
 */
Normalizer::~Normalizer ()
{}

/*!
 * \sa Document::pushDocument()
 */
void
Normalizer::pushDocument ()
{
	TRACE (__FUNCTION__);

	_reader.pushDocument ();
}

/*!
 * \sa Document::popDocument()
 */
void
Normalizer::popDocument ()
{
	TRACE (__FUNCTION__);

	_reader.popDocument ();
}

/*!
 * \sa Document::text()
 */
void
Normalizer::text (gchar const *str)
{
	TRACE (__FUNCTION__);

	_methods.push_front (new normalizer::Text (_reader, str));
}

/*!
 * \sa Document::pushStyle()
 *
 * \todo Check if we're already in style of that type and add fallback instead if yes.
	 Wait! First we need to know which style is "stronger", the first or the second.
	 If it was the second we'd just add the fallback for a nested pushStyle.
 */
void
Normalizer::pushStyle (document::Style::Type type)
{
	TRACE (__FUNCTION__);

	Method *method;

	if (_isBold && type == document::Style::BOLD ||
	    _isItalic && type == document::Style::ITALIC ||
	    _isMonospace && type == document::Style::MONOSPACE ||
	    _isUnderline && type == document::Style::UNDERLINE) {

		method = normalizer::Text::fallback (_reader, type);

	} else {
		method = new PushStyle (_reader, type);
	}

	_methods.push_front (method);

	_isBold = type == document::Style::BOLD;
	_isItalic = type == document::Style::ITALIC;
	_isMonospace = type == document::Style::MONOSPACE;
	_isUnderline = type == document::Style::UNDERLINE;
}

/*!
 * For now we don't allow nesting of the same style (e.g. "foo *bar *baz* bar* foo").
 *
 * \sa Document::cancelStyle()
 */
void
Normalizer::cancelStyle (document::Style::Type type)
{
	TRACE (__FUNCTION__);

	_dump();

	std::list<Method *>::iterator iter;
	PushStyle *pushStyle;
	PopStyle *popStyle;
	
	iter = _methods.begin ();
	while (iter != _methods.end ()) {
		// search backwards until start of style and cancel it
		if ((*iter)->getClass () == Method::PUSH_STYLE) {
			pushStyle = dynamic_cast<PushStyle *> (*iter);
			if (pushStyle->getType () == type) {
				iter = _methods.erase (iter);
				_methods.insert (iter, pushStyle->createFallback ());
				delete pushStyle;
				_isBold = (_isBold && type == document::Style::BOLD) ? FALSE : _isBold;
				_isItalic = (_isItalic && type == document::Style::ITALIC) ? FALSE : _isItalic;
				_isMonospace = (_isMonospace && type == document::Style::MONOSPACE) ? FALSE : _isMonospace;
				_isUnderline = (_isUnderline && type == document::Style::UNDERLINE) ? FALSE : _isUnderline;
				break;
			}			
		} else if ((*iter)->getClass () == Method::POP_STYLE) {
			popStyle = dynamic_cast<PopStyle *> (*iter);
			if (popStyle->getType () == type) {
				// hitting upon closed style of same type
				// no need to go further or do anything.
				break;
			}
		}
		++iter;
	}

	_dump("    ");
}

/*!
 * \sa Document::popStyle()
 *
 * \todo Check if style is valid and either pop or cancel it.
	 Hmm, probably we need to pass the style here too, to correctly
	 unwind nested 
 */
void
Normalizer::popStyle (document::Style::Type type)
{
	TRACE (__FUNCTION__);

	Method *method;

	if (_isBold && type == document::Style::BOLD ||
	    _isItalic && type == document::Style::ITALIC ||
	    _isMonospace && type == document::Style::MONOSPACE ||
	    _isUnderline && type == document::Style::UNDERLINE) {

		method = new PopStyle (_reader, type);

	} else {
		// TODO maybe use cancelStyle() here?
		method = normalizer::Text::fallback (_reader, type);
	}

	_methods.push_front (method);

	_isBold = (_isBold && type == document::Style::BOLD) ? FALSE : _isBold;
	_isItalic = (_isItalic && type == document::Style::ITALIC) ? FALSE : _isItalic;
	_isMonospace = (_isMonospace && type == document::Style::MONOSPACE) ? FALSE : _isMonospace;
	_isUnderline = (_isUnderline && type == document::Style::UNDERLINE) ? FALSE : _isUnderline;
}

/*!
 * \todo implement
 */
void 
Normalizer::link (document::Link::Type type, 
		  gchar const *label, 
		  gchar const *target,
		  gchar const *scroll)
{

}

/*!
 * \sa Document::pushBlock()
 */
void
Normalizer::pushBlock (document::Block::Type type)
{
	TRACE (__FUNCTION__);

	if (_methods.empty ()) {
		_methods.push_front (new PushBlock (_reader, type));

	} else {
		// change block type
		std::list<normalizer::Method *>::iterator iter;
		normalizer::PushBlock *block;
		iter = _methods.end ();
		--iter;
		block = dynamic_cast<normalizer::PushBlock *> (*iter);
		block->setType (type);
	}
}

/*!
 * \sa Document::popBlock()
 */
void
Normalizer::popBlock ()
{
	TRACE (__FUNCTION__);

	if (_methods.empty ()) {
		_reader.popBlock ();
		return;
	}

	cancelStyle (document::Style::BOLD);
	cancelStyle (document::Style::ITALIC);
	cancelStyle (document::Style::MONOSPACE);
	cancelStyle (document::Style::UNDERLINE);

	// check if H3 or H4 to cut off lead-out
	std::list<Method *>::iterator iter;
	iter = _methods.end ();
	--iter;
	if ((*iter)->getClass () == normalizer::Method::PUSH_BLOCK) {
		PushBlock *push = dynamic_cast<PushBlock *> (*iter);
		if (push->getType () == document::Block::HEADING_3 ||
		    push->getType () == document::Block::HEADING_4) {

			iter = _methods.begin ();
			if ((*iter)->getClass () == normalizer::Method::TEXT) {
				normalizer::Text *text;
				text = dynamic_cast<normalizer::Text *> (*iter);
				text->rtrim (push->getType ());
			}
		}
	}

	std::list<Method *>::reverse_iterator riter;
	Method *method;
	riter = _methods.rbegin ();
	while (riter != _methods.rend ()) {
		method = *riter;
		(*method) ();
		delete method;
		++riter;
	}
	_reader.popBlock ();

	_methods.clear ();
}

/*!
 * Dump parsed representation of current block.
 *
 * \note This is used for debugging purpose only.
 */
void
Normalizer::dump (gchar const * indent)
{
	std::list<Method *>::reverse_iterator riter;
	
	riter = _methods.rbegin ();
	while (riter != _methods.rend ()) {
		std::cout << indent << (*riter)->getClass ();
		if ((*riter)->getClass () == Method::PUSH_STYLE) {
			std::cout << " <style>" << std::endl;
		} else if ((*riter)->getClass () == Method::POP_STYLE) {
			std::cout << " </style>" << std::endl;
		} else if ((*riter)->getClass () == Method::PUSH_BLOCK) {
			std::cout << " <block>" << std::endl;
		} else if ((*riter)->getClass () == Method::POP_BLOCK) {
			std::cout << " </block>" << std::endl;
		} else if ((*riter)->getClass () == Method::TEXT) {
			normalizer::Text *text = dynamic_cast<normalizer::Text *> (*riter);
			std::cout << " \"" << text->_text << "\"" << std::endl;
		} else {
			std::cout << std::endl;
		}			
		++riter;
	}
}
