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
	virtual ~Token ();
protected:
	Token ();
};

class Eof : public Token
{
public:
	Eof () {}
	virtual ~Eof () {}
};

class Text : public Token
{
public:
	/*!
	 * The object takes over ownership.
	 */
	Text (gchar *text)
	  : _text (text)
	{
		g_assert (text);
	}
	virtual ~Text ()
	{
		g_free (_text);
	}
private:
	gchar *_text;
};

class Style : public Token
{
public:
	enum Type {
		BOLD, 
		ITALIC,
		MONOSPACE,
		UNDERLINE
	};
	Style (Type type)
	  : _type (type)
	{}
	virtual ~Style () {}
	virtual Type getType () const { return _type; }
private:
	Type _type;
};

class Outline : Token
{
public:
	enum Type {
		PARAGRAPH,
		INDENT
	};
	Outline (Type type)
	  : _type (type)
	{}
	virtual ~Outline () {}
private:
	Type _type;	
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

private:
	FILE *_istream;
};

};

#endif // CROSSMARK_SCANNER_H
