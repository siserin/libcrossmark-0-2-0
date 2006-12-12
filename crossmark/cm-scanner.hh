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

#ifndef CROSSMARK_SCANNER_H
#define CROSSMARK_SCANNER_H

#include <string>
#include <glib.h>
#include <stdio.h>

namespace crossmark {

namespace tokens {

class Token
{
public:
	virtual ~Token () {}

	virtual operator const gchar * () 
	{
		return "(void)";
	}

protected:
	Token () {}
};

class Eof : public Token
{
public:
	Eof () {}

	virtual ~Eof () {}

	virtual operator const gchar * () { return "\n\nEOF\n"; }
};

class Text : public Token
{
public:
	Text (const gchar *text)
	  : _text (text)
	{
		g_assert (text);
	}

	virtual ~Text () {}

	virtual void append (gchar c) { _text.append (1, c); }

	virtual const gchar * getBuffer () { return _text.c_str (); }

	virtual const std::string &getString () { return _text; }

	virtual operator const gchar * () { return _text.c_str (); }
private:
	std::string _text;
};

class Style : public Token
{
public:
	enum Type {
		ASTERISK, 
		SLASH,
		BACKTICK,
		UNDERSCORE
	};

	enum Pos {
		LEFT,
		RIGHT
	};

	Style (Type type_, Pos pos_)
	  : type (type_), 
	    pos (pos_)
	{}

	virtual ~Style () {}

	virtual operator const gchar * () 
	{
		if (pos == LEFT) {
			return "<style>";
		} else {
			return "</style>";
		}
	}

	Type type;
	Pos pos;
};

class Paragraph : public Token
{
public:
	Paragraph () {}

	virtual ~Paragraph () {}

	virtual operator const gchar * () 
	{
		return "<paragraph />";
	}
};

};

/*!
 * \todo Use libgsf.
 */
class Scanner
{
public: 
	Scanner (std::string &file);
	virtual ~Scanner ();

	virtual tokens::Token * fetchToken ();

protected:
	virtual tokens::Token * scanEof (gchar c2);
	virtual tokens::Token * scanParagraph (gchar c2);
	virtual tokens::Token * scanStyle (gchar c2);
	virtual tokens::Token * scanText (gchar c2);

private:
	FILE  		*_istream;
	tokens::Token 	*_next;
	gchar  		 _c1;
};

};

#endif // CROSSMARK_SCANNER_H
