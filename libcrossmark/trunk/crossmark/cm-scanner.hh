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
#include <crossmark/cm-stream.hh>

namespace crossmark {

/*!
 * This namespace contains the various token types recognised by the 
 * scanner.
 */
namespace tokens {

/*!
 * Abstract base class.
 */
class Token
{
public:
	virtual ~Token () {}

	virtual const gchar * serialize () { return "\n<base />\n"; }

protected:
	Token () {}
};

/*!
 * Start-of-file.
 */
class Sof : public Token
{
public:
	Sof () {}

	virtual ~Sof () {}

	virtual const gchar * serialize () { return "<html><body>\n"; }
};

/*!
 * End-of-file.
 */
class Eof : public Token
{
public:
	Eof () {}

	virtual ~Eof () {}

	virtual const gchar * serialize () { return "\n</body></html>\n"; }
};

/*!
 * Text token.
 */
class Text : public Token
{
public:
	Text (const gchar *text)
	  : _text (text)
	{
		g_assert (text);
	}

	virtual ~Text () {}

	virtual void append (gunichar c) { _text.append (1, c); }

	virtual const gchar * getBuffer () { return _text.c_str (); }

	virtual const std::string &getString () { return _text; }

	virtual const gchar * serialize () { return _text.c_str (); }
private:
	std::string _text;
};

/*!
 * Indentation token.
 */
class Indent : public Token
{
public:
	Indent () {}
	virtual ~Indent () {}

	virtual const gchar * serialize () { return "<indent />"; }
};

/*!
 * Style token.
 */
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

	virtual const gchar * serialize ()
	{
		if (type == ASTERISK && pos == LEFT) return "<b>"; 
		if (type == ASTERISK && pos == RIGHT) return "</b>"; 
		if (type == SLASH && pos == LEFT) return "<i>"; 
		if (type == SLASH && pos == RIGHT) return "</i>"; 
		if (type == BACKTICK && pos == LEFT) return "<code>"; 
		if (type == BACKTICK && pos == RIGHT) return "</code>"; 
		if (type == UNDERSCORE && pos == LEFT) return "<u>"; 
		if (type == UNDERSCORE && pos == RIGHT) return "</u>";
		g_assert (FALSE);
	}

	Type type;
	Pos pos;
};

/*!
 * Paragraph break.
 */
class Newline : public Token
{
public:
	Newline () {}

	virtual ~Newline () {}

	virtual const gchar * serialize () { return "<br />"; }
};

/*!
 * Paragraph break.
 */
class Paragraph : public Token
{
public:
	Paragraph () {}

	virtual ~Paragraph () {}

	virtual const gchar * serialize () { return "<paragraph />"; }
};

};

/*!
 * Scanner.
 */
class Scanner
{
public: 
	Scanner (const std::string &file);
	Scanner (streams::Input &istream);
	virtual ~Scanner ();

	virtual tokens::Token * fetchToken ();

protected:
	virtual tokens::Token * scanEof ();
	virtual tokens::Token * scanEof (gunichar c);
	virtual tokens::Token * scanNewline (gboolean &restart);
	virtual tokens::Token * scanIndent ();
	virtual tokens::Token * scanStyle (gunichar c2, gunichar &tail);

private:
	streams::Input	&_istream;
	gboolean	 _ownStream;
	tokens::Token 	*_next;
	gunichar	 _c1;
};

}; // namespace crossmark

#endif // CROSSMARK_SCANNER_H
