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
 * \file cm-validator-private.hh
 * \brief Crossmark validator.
 * \internal
 */

#ifndef CM_VALIDATOR_PRIVATE_HH
#define CM_VALIDATOR_PRIVATE_HH

#include <list>
#include <string>
#include <crossmark/crossmark.hh>
#include <crossmark/cm-document.hh>
#include <crossmark/cm-validator-methods-private.hh>

namespace crossmark {

/*!
 * \internal
 * \brief Extended document interface for validation.
 *
 * The ``editor'' namespace specialises modules so they can be used for
 * automatic input conversion. 
 *
 * E.g. after input of "*foo" it should
 * automatically switch to boldface, but if the next "*" doesn't match
 * word boundaries boldface would have to be cancelled and reverted.
 * Blockquotes are similar, e.g. 3 times <enter> ends blockquote mode.
 * 
 * It will be important to (1) keep the input parser in sync when formatting 
 * is done using the toolbar and (2) resync when the cursor is moved using
 * mouse or keyboard navigation. 
 * For (2) we may just re-parse from the beginning of the block.
 * \see Validator::resume()
 * 
 * \todo Figure out a better name. Calling it trackers for now because 
	 it keeps track of keystrokes.
 * \todo This may be exported once we're implementing the input method parser.
 */
namespace validators {

/*!
 * \internal 
 * \brief Specialised text styling interface that can cancel styles.
 */
class Style : public document::Style
{
public:
	virtual ~Style () {}

	virtual void pushStyle (Type type) = 0;
	virtual void cancelStyle (Type type) = 0;
	virtual void popStyle (Type type) = 0;
};

}; // namespace validators

/*
 * \internal
 * \brief Extended crossmark::Document interface that is used between parser and validator.
 *
 * \todo Pull out this classes' public interface for the <i>input method</i>.
 */
class Validator : public Document,
		  public validators::Style
{
public:
	Validator (Document &reader);
	virtual ~Validator ();

	/*!
	 * This sets the tracker to initial state. 
	 * \see resume
 	 */
	// virtual void reset ();

	/*!
	 * Resume tracking and making formatting predictions.
	 *
	 * Between reset() and resume() all content between the 
	 * start of the block and the cursor position has to be 
	 * passed to the tracker, so it'll settle in a consistent 
	 * state before editing is resumed.
	 *
	 * While reparsing no predictions will be issued.
	 *
	 * The reset, reparse, resume chain has to be run when
	 * editing is resumed after the caret has been moved.
	 */
	// virtual void resume ();


	virtual void pushDocument ();
	virtual void popDocument ();

	// text interface
	virtual void text (const std::string &text);

	// style interface
	virtual void pushStyle (document::Style::Type type);
	virtual void cancelStyle (document::Style::Type type);
	virtual void popStyle (document::Style::Type type);

	// document structure interface
	virtual void pushBlock (document::Block::Type type);
	virtual void pushHeading (int level);
	virtual void popBlock ();

private:
	std::list<validators::methods::Method *> _methods;
	Document &_reader;

	gboolean _isBold;
	gboolean _isItalic;
	gboolean _isMonospace;
	gboolean _isUnderline;
};

};  // namespace crossmark

#endif /* CM_VALIDATOR_PRIVATE_HH */
