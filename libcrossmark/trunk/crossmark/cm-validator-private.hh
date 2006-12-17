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
#include <string>
#include <crossmark/crossmark.hh>
#include <crossmark/cm-document.hh>

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
 * \brief Document action proxies.
 */
namespace methods {

class Method
{
public:
	Method (Reader &reader)
	  : _reader (reader)
	{}
	virtual ~Method () {}
	virtual void operator () () = 0;

protected:
	Reader &_reader;
};

class Text : public Method
{
public:
	Text (Reader &reader, const gchar *text) 
	  : Method (reader),
	    _text (g_strdup (text))
	{}
	virtual ~Text () 
	{
		g_free (_text);
	}
	virtual void operator () (void) { _reader.text (_text); }

private:
	gchar  *_text;
};

class PushStyle : public Method
{
public:
	PushStyle (Reader &reader, document::Style::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PushStyle () {}
	virtual void operator () (void) { _reader.pushStyle (_type); }

private:
	document::Style::Type _type;
};

class PopStyle : public Method
{
public:
	PopStyle (Reader &reader, document::Style::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PopStyle () {}
	virtual void operator () (void) { _reader.popStyle (); }

private:
	document::Style::Type _type;
};

class PushStructure : public Method
{
public:
	PushStructure (Reader &reader, document::Structure::Type type) 
	  : Method (reader), 
	    _type (type)
	{}
	virtual ~PushStructure () {}
	virtual void operator () (void) { _reader.pushStructure (_type); }

private:
	document::Structure::Type _type;
};

class PushHeadingStructure : public Method
{
public:
	PushHeadingStructure (Reader &reader, int level) 
	  : Method (reader),
	    _level (level)
	{}
	virtual ~PushHeadingStructure () {}
	virtual void operator () (void) { _reader.pushHeadingStructure (_level); }

private:
	int _level;
};

class PopStructure : public Method
{
public:
	PopStructure (Reader &reader, document::Structure::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PopStructure () {}
	virtual void operator () (void) { _reader.pushStructure (_type); }

private:
	document::Structure::Type _type;
};

}; // namespace methods

class Style : public document::Style
{
public:
	virtual ~Style () {}

	virtual void pushStyle (Type type) = 0;
	virtual void cancelStyle (Type type) = 0;
	virtual void popStyle () = 0;
};

}; // namespace validators

/*
 * \todo Pull out this classes' public interface for the <i>input method</i>
 */
class Validator : public Reader,
		  public validators::Style
{
public:
	Validator (Reader &reader);
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
	virtual void popStyle ();

	// document structure interface
	virtual void pushStructure (document::Structure::Type type);
	virtual void pushHeadingStructure (int level);
	virtual void popStructure ();

private:
	Reader &_reader;
	std::list<validators::methods::Method *> _methods;
};

};  // namespace crossmark

#endif /* CM_VALIDATOR_PRIVATE_HH */
