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
 * \file cm-scanner-private.hh
 * \brief Crossmark scanner.
 * \internal
 */

#ifndef CM_SCANNER_PRIVATE_HH
#define CM_SCANNER_PRIVATE_HH

#include <list>
#include <string>
#include <glib.h>
#include <crossmark/cm-stream.hh>

namespace crossmark {

/*!
 * \internal
 * \brief Tokens recognised by the scanner.
 *
 * This namespace contains the various token types recognised by the 
 * scanner.
 */
namespace tokens {

/*!
 * \brief Abstract token base class.
 */
class Token
{
public:
	enum Class {
		START,
		END,
		TEXT,
		INDENT,
		STYLE,
		NEWLINE,
		PARAGRAPH
	};

	virtual ~Token () {}

	virtual Token::Class getClass () const = 0;
	virtual const gchar * toHtml () const = 0;
	virtual const gchar * toString () const = 0;

protected:
	Token () {}
};

/*!
 * \brief Start-of-file.
 */
class Start : public Token
{
public:
	Start () {}
	virtual ~Start () {}

	virtual Token::Class getClass () const { return Token::START; }
	virtual const gchar * toHtml () const { return "<html><body>\n"; }
	virtual const gchar * toString () const { return NULL; }
};

/*!
 * \brief End-of-file.
 */
class End : public Token
{
public:
	End () {}
	virtual ~End () {}

	virtual Token::Class getClass () const { return Token::END; }
	virtual const gchar * toHtml () const { return "\n</body></html>\n"; }
	virtual const gchar * toString () const { return NULL; }
};

/*!
 * \brief Text token.
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

	virtual Token::Class getClass () const { return Token::TEXT; }
	virtual const gchar * toHtml () const { return _text.c_str (); }
	virtual const gchar * toString () const { return _text.c_str (); }
private:
	std::string _text;
};

/*!
 * \brief Indentation token.
 */
class Indent : public Token
{
public:
	Indent () {}
	virtual ~Indent () {}

	virtual Token::Class getClass () const { return Token::INDENT; }
	virtual const gchar * toHtml () const { return "\t&emsp;"; }
	/*
	 * \todo Might become an issue with whitespace-indentation.
	 */
	virtual const gchar * toString () const { return "\t"; }
};

/*!
 * \brief Style token.
 */
class Style : public Token
{
public:
	// don't change order
	// see document::Style::Type
	enum Type {
		ASTERISK, 
		SLASH,
		BACKTICK,
		UNDERSCORE
	};

	enum Pos {
		LEFT,
		CENTER,
		RIGHT
	};

	Style (Type type, Pos pos)
	  : _type (type), 
	    _pos (pos)
	{
		if (_type == ASTERISK && _pos == LEFT) _text = " *"; 
		else if (_type == ASTERISK && _pos == CENTER) _text = "*";
		else if (_type == ASTERISK && _pos == RIGHT) _text = "* ";
		else if (_type == SLASH && _pos == LEFT) _text = " /";
		else if (_type == SLASH && _pos == CENTER) _text = "/";
		else if (_type == SLASH && _pos == RIGHT) _text = "/ ";
		else if (_type == BACKTICK && _pos == LEFT) _text = " `";
		else if (_type == BACKTICK && _pos == CENTER) _text = "`";
		else if (_type == BACKTICK && _pos == RIGHT) _text = "` ";
		else if (_type == UNDERSCORE && _pos == LEFT) _text = " _";
		else if (_type == UNDERSCORE && _pos == CENTER) _text = "_";
		else if (_type == UNDERSCORE && _pos == RIGHT) _text = " _";
		else g_assert (FALSE);
	}

	virtual ~Style () {}

	virtual Type getType () const { return _type; }
	virtual Pos getPos () const { return _pos; }
	virtual Token::Class getClass () const { return Token::STYLE; }
	virtual const gchar * toHtml () const
	{
		if (_type == ASTERISK && _pos == LEFT) return "<b>"; 
		else if (_type == ASTERISK && _pos == CENTER) return "*"; 
		else if (_type == ASTERISK && _pos == RIGHT) return "</b>"; 
		else if (_type == SLASH && _pos == LEFT) return "<i>"; 
		else if (_type == SLASH && _pos == CENTER) return "*"; 
		else if (_type == SLASH && _pos == RIGHT) return "</i>"; 
		else if (_type == BACKTICK && _pos == LEFT) return "<code>"; 
		else if (_type == BACKTICK && _pos == CENTER) return "`"; 
		else if (_type == BACKTICK && _pos == RIGHT) return "</code>"; 
		else if (_type == UNDERSCORE && _pos == LEFT) return "<u>"; 
		else if (_type == UNDERSCORE && _pos == CENTER) return "_";
		else if (_type == UNDERSCORE && _pos == RIGHT) return "</u>";
		else g_assert (FALSE);
	}
	virtual const gchar * toString () const { return _text.c_str (); }

private:
	Type		_type;
	Pos		_pos;
	std::string 	_text;
};

/*!
 * \brief Newline token.
 */
class Newline : public Token
{
public:
	Newline () {}
	virtual ~Newline () {}

	virtual Token::Class getClass () const { return Token::NEWLINE; }
	virtual const gchar * toHtml () const { return "<br />\n"; }
	virtual const gchar * toString () const { return "\n"; }	
};

/*!
 * \brief Paragraph break.
 */
class Paragraph : public Token
{
public:
	Paragraph () {}
	virtual ~Paragraph () {}

	virtual Token::Class getClass () const { return Token::PARAGRAPH; }
	virtual const gchar * toHtml () const { return "<br /><br />\n\n"; }
	virtual const gchar * toString () const { return "\n\n"; }
};

/*!
 * \brief Token factory interface.
 */
class FactoryIface
{
public:
	virtual ~FactoryIface () {}

	virtual Token * createTokenImpl (Token::Class klass) const = 0;
	virtual Style * createStyleTokenImpl (Style::Type type, Style::Pos pos) const = 0;
	virtual Text * createTextTokenImpl (const gchar *text) const = 0;
};

/*!
 * \brief Token factory.
 *
 * \todo Use Factory to create tokens, then get rid of Token::toHtml()
	 and hook factory from cm-scan.
 */
class Factory : public FactoryIface
{
public:
	virtual ~Factory () {}

	static Factory & instance ();

	virtual void hook (const FactoryIface *factory);
	virtual void unhook (const FactoryIface *factory);

	virtual Token * createToken (Token::Class klass) const;
	virtual Style * createStyleToken (Style::Type type, Style::Pos pos) const;
	virtual Text * createTextToken (const gchar *text) const;

	// token factory
	virtual Token * createTokenImpl (Token::Class klass) const;
	virtual Style * createStyleTokenImpl (Style::Type type, Style::Pos pos) const;
	virtual Text * createTextTokenImpl (const gchar *text) const;

protected:
	Factory ();

private:
	std::list<const FactoryIface *> _factories;
};

}; // namespace tokens

/*!
 * \internal
 * \brief The scanner provides token-based input.
 *
 * Initial scanner grammar
 * Yeah, this is flawed, but I'm trying to treat markup stuff 
 * as a single character (by using lookahead).
 * \verbatim
   {charset}  := {UTF-8} \ {" *", "* ", " /", "/ ", " `", "` ", " _", "_ "}
   token      := paragraph | style | text | sof | eof
   paragraph  := '\n' '\n' '\n'*
   style      := " *" | "* " | " /" | "/ " | " `" | "` " | " _" | "_ "
   text       := {charset}*
   \endverbatim
 */
class Scanner
{
public: 
	Scanner (const std::string &file);
	Scanner (streams::Input &istream);
	virtual ~Scanner ();

	virtual tokens::Token * fetchToken ();

protected:
	virtual tokens::Token * scanEnd ();
	virtual tokens::Token * scanEnd (gunichar c);
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

#endif // CM_SCANNER_PRIVATE_HH
