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
 * \file cm-validator-methods-private.hh
 * \brief Crossmark validator.
 * \internal
 */

#ifndef CM_VALIDATOR_METHODS_PRIVATE_HH
#define CM_VALIDATOR_METHODS_PRIVATE_HH

#include <list>
#include <string>
#include <crossmark/crossmark.hh>
#include <crossmark/cm-document.hh>

namespace crossmark {

namespace validators {

/*!
 * \internal
 * \brief Document action proxies.
 * \todo Templatise the methods.
 */
namespace methods {

/*!
 * \internal 
 * \brief Base interface for buffered calls to a document reader.
 */
class Method
{
public:
	enum Class {
		TEXT,
		PUSH_STYLE, 
		POP_STYLE,
		PUSH_BLOCK,
		POP_BLOCK
	};

	Method (Document &reader)
	  : _reader (reader)
	{}
	virtual ~Method () {}
	virtual void operator () () = 0;
	virtual Method::Class getClass () const = 0;

protected:
	Document &_reader;
};

/*!
 * \internal 
 * \brief Buffered "text" call.
 */
class Text : public Method
{
friend class ::crossmark::Validator; // DEBUG
public:
	Text (Document &reader, const gchar *text) 
	  : Method (reader),
	    _text (g_strdup (text))
	{}
	virtual ~Text () 
	{
		g_free (_text);
	}
	virtual void operator () (void) { _reader.text (_text); }
	virtual Method::Class getClass () const { return Method::TEXT; }

	static Text * fallback (Document &reader, document::Style::Type type) 
	{ 
		switch (type) {
		case document::Style::BOLD:
			return new Text (reader, "*");
			break;
		case document::Style::ITALIC:
			return new Text (reader, "/");
			break;
		case document::Style::MONOSPACE:
			return new Text (reader, "`");
			break;
		case document::Style::UNDERLINE:
			return new Text (reader, "_");
			break;
		default:
			g_assert_not_reached ();
		}
	}

private:
	gchar  *_text;
};

/*!
 * \internal 
 * \brief Buffered "push style" call.
 */
class PushStyle : public Method
{
public:
	PushStyle (Document &reader, document::Style::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PushStyle () {}
	virtual void operator () (void) { _reader.pushStyle (_type); }
	virtual Method::Class getClass () const { return Method::PUSH_STYLE; }
	virtual document::Style::Type getType () const { return _type; }
	virtual Text * createFallback () { return Text::fallback (_reader, _type); }

private:
	document::Style::Type _type;
};

/*!
 * \internal 
 * \brief Buffered "pop style" call.
 */
class PopStyle : public Method
{
public:
	PopStyle (Document &reader, document::Style::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PopStyle () {}
	virtual void operator () (void) { _reader.popStyle (_type); }
	virtual Method::Class getClass () const { return Method::POP_STYLE; }
	virtual document::Style::Type getType () const { return _type; }
	virtual Text * createFallback () { return Text::fallback (_reader, _type); }

private:
	document::Style::Type _type;
};

/*!
 * \internal 
 * \brief Buffered "push block" call.
 */
class PushBlock : public Method
{
public:
	PushBlock (Document &reader, document::Block::Type type) 
	  : Method (reader), 
	    _type (type)
	{}
	virtual ~PushBlock () {}
	virtual void operator () (void) { _reader.pushBlock (_type); }
	virtual Method::Class getClass () const { return Method::PUSH_BLOCK; }

private:
	document::Block::Type _type;
};

/*!
 * \internal 
 * \brief Buffered "pop block" call.
 */
class PopBlock : public Method
{
public:
	PopBlock (Document &reader, document::Block::Type type) 
	  : Method (reader),
	    _type (type)
	{}
	virtual ~PopBlock () {}
	virtual void operator () (void) { _reader.pushBlock (_type); }
	virtual Method::Class getClass () const { return Method::POP_BLOCK; }

private:
	document::Block::Type _type;
};

}; // namespace methods

}; // namespace validators

}; // namespace crossmark

#endif /* CM_VALIDATOR_METHODS_PRIVATE_HH */
