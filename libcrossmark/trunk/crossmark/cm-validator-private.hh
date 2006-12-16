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

#ifndef CM_VALIDATOR_PRIVATE_HH
#define CM_VALIDATOR_PRIVATE_HH

#include <list>
#include <map>
#include <stack>
#include <string>
#include <crossmark/crossmark.hh>

namespace crossmark {

/*!
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
 * \see Tracker::resume()
 * 
 * \todo Figure out a better name. Calling it trackers for now because 
	 it keeps track of keystrokes.
 */
namespace validators {

class Style : public document::Style
{
public:
	virtual ~Style ();

	virtual void pushStyle (Type type) = 0;
	virtual void cancelStyle (Type type) = 0;
	virtual void popStyle () = 0;
};

}; // namespace validators

/*
 * \todo Figure out a better name. Calling it trackers for now because 
	 it keeps track of keystrokes.
 * \todo Should this be in the namespace of the same name?
 */
class Validator : public document::Text, 
		  //public document::Note,
		  //public document::Link,
		  public validators::Style,
		  //public document::Image,
		  public document::Structure
		  //public document::List,
		  //public document::Table,
		  //public document::Math, 
		  //public document::Macro
{
public:
	virtual ~Tracker ();

	/*!
	 * This sets the tracker to initial state. 
	 * \see resume
 	 */
	virtual void reset ();

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
	virtual void resume ();


	virtual void pushDocument () = 0;
	virtual void popDocument () = 0;

	// text interface
	virtual void text (const std::string &text) = 0;

	// style interface
	virtual void pushStyle (document::Style::Type type) = 0;
	virtual void cancelStyle (Type type) = 0;
	virtual void popStyle () = 0;

	// document structure interface
	virtual void pushStructure (document::Structure::Type type) = 0;
	virtual void pushHeadingStructure (int level) = 0;
	virtual void popStructure () = 0;
};

};  // namespace crossmark

#endif /* CM_VALIDATOR_PRIVATE_HH */
