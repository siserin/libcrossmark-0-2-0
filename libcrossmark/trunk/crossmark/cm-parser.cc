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

#include "cm-parser.hh"
#include "cm-scanner-private.hh"
#include "cm-normalizer-private.hh"

#if 0
//#ifdef DEBUG
#define TRACE(M) fprintf(stderr,"Parser::");fprintf(stderr,M);fprintf(stderr,"\n");
#else
#define TRACE(M)
#endif

using namespace crossmark;

/*!
 * Create a parser for filename and reader.
 */
Parser::Parser (gchar const *file, Document &reader)
  : _scanner (new Scanner (file)),
    _normalizer (new Normalizer (reader))
{}

/*!
 * Create a parser input stream and reader.
 */
Parser::Parser (stream::Input &istream, Document &reader)
  : _scanner (new Scanner (istream)),
    _normalizer (new Normalizer (reader))
{}

/*!
 * Dtor.
 */
Parser::~Parser ()
{
	delete _scanner;
}

/*
 * Scanner tokens:
 * start end pbreak newline indent text 
 * style-lb style-rb style-li style-ri style-lm style-rm style-lu style-ru

# Some quickie top-down grammar, flaws:
# + way incomplete
# + arbitrary nesting of styling allowed
# + ...

document   := start newline* ( pbreak | heading | block | blockquote )* end
block      := heading | paragraph
heading    := h1 | h2 | h3 | h4
h1         := line* h1-leadout pbreak
h2         := line* h2-leadout pbreak
h3         := h3-leadin line* h3-leadout pbreak
h4         := h4-leadin line* h4-leadout pbreak
paragraph  := line* pbreak
blockquote := indent line ( indent line )* pbreak
line       := ( text | markup )* newline
markup     := bold | italic | monospace | underline
bold       := style-lb ( text | markup )* style-rb
italic     := style-li ( text | markup )* style-ri
monospace  := style-lm ( text | markup )* style-rm
underline  := style-lu ( text | markup )* style-ru
*/

/*!
 * Read document, this caused call reader methods to be called.
 *
 * \todo error handling;
 */
gboolean 
Parser::parse ()
{
	parseDocument ();
	return TRUE;
}

/*!
 * Toplevel parsing method.
 */
void 
Parser::parseDocument ()
{
	TRACE (__FUNCTION__);

	// document   := start newline* ( paragraph | blockquote )* end

	_normalizer->pushDocument ();

	token::Token *token = _scanner->fetchToken ();
	g_assert (token->getClass () == token::Token::START);
	delete token;

	token = _scanner->fetchToken ();
	while (token->getClass () == token::Token::NEWLINE) {
		delete token;
		token = _scanner->fetchToken ();
	}
	while (token->getClass () == token::Token::PARAGRAPH) {
		delete token;
		// ignore empty paragraph at beginning?
		// _normalizer->pushBlock (document::Block::PARAGRAPH);
		// _normalizer->popBlock ();
		token = _scanner->fetchToken ();
	}

	token::Token *next;
	while (token->getClass () != token::Token::END) {

		next = NULL;

		if (token->getClass () == token::Token::INDENT) {
			next = parseBlockquote (token);
		} else {
			next = parseBlock (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}
	delete token;

	_normalizer->popDocument ();
}

/*!
 * Parse a paragraph.
 */
token::Token * 
Parser::parseBlock (const token::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	// paragraph  := line* pbreak

	token::Token *token;
	if (first->getClass () == token::Token::HEADING) {
		const token::Heading *heading;
		heading = dynamic_cast<const token::Heading *> (first);
		_normalizer->pushBlock ((document::Block::Type) heading->getType ());
		token = _scanner->fetchToken ();
	} else {
		_normalizer->pushBlock (document::Block::PARAGRAPH);
		token = parseLine (first);
	}

	token::Token *next;
	while (token->getClass () != token::Token::PARAGRAPH && 
	       token->getClass () != token::Token::END) {

		next = parseLine (token);

		if (token != next) {
			delete token;
		}
		token = next;
	}
	
	_normalizer->popBlock ();

	if (token->getClass () == token::Token::PARAGRAPH) {
		delete token;
		token = _scanner->fetchToken ();
	}
	return token;
}

/*!
 * Parse a blockquote.
 */
token::Token * 
Parser::parseBlockquote (const token::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::PARAGRAPH ||
	    first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	// blockquote := indent line ( indent line )* pbreak
	_normalizer->pushBlock (document::Block::BLOCKQUOTE);

	g_assert (first->getClass () == token::Token::INDENT);

	token::Token *token = parseLine (_scanner->fetchToken ());
	if (token->getClass () == token::Token::END) {
		return token;
	}	
	
	while (token->getClass () != token::Token::PARAGRAPH && 
	       token->getClass () != token::Token::END) {

		g_assert (token->getClass () == token::Token::INDENT);
		delete token;		

		token = parseLine (_scanner->fetchToken ());		
	}

	_normalizer->popBlock ();

	if (token->getClass () == token::Token::PARAGRAPH) {
		delete token;
		token = _scanner->fetchToken ();
	}
	return token;
}

/*!
 * Parse a line.
 *
 * \todo What to do with tabs in running text?
 */
token::Token * 
Parser::parseLine (const token::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::PARAGRAPH ||
	    first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	// line       := ( text | markup )* newline

	token::Token *token;
	if (first->getClass () == token::Token::HEADING) {
		const token::Heading *heading;
		heading = dynamic_cast<const token::Heading *> (first);
		_normalizer->pushBlock ((document::Block::Type) heading->getType ());
		token = _scanner->fetchToken ();
	} else if (first->getClass () == token::Token::STYLE) {
		token = parseMarkup (first);
	} else {
		token = parseText (first);
	}

	token::Token *next;
	while (token->getClass () != token::Token::NEWLINE &&
	       token->getClass () != token::Token::PARAGRAPH &&  
	       token->getClass () != token::Token::END) {

		if (token->getClass () == token::Token::STYLE) {
			next = parseMarkup (token);
		} else if (token->getClass () == token::Token::INDENT) {
			_normalizer->text ("\t");
			next = _scanner->fetchToken ();
		} else {
			next = parseText (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}

	if (token->getClass () == token::Token::NEWLINE) {
		delete token;
		token = _scanner->fetchToken ();
	}
	return token;
}

/*!
 * Parse styled text "markup".
 */
token::Token * 
Parser::parseMarkup (const token::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::PARAGRAPH ||
	    first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	// markup     := bold | italic | monospace | underline

	const token::Style *style;
	token::Token 	    *next;

	style = dynamic_cast<const token::Style *> (first);
	if (style->getClass () == token::Token::STYLE && 
	    style->getPos () == token::Style::CENTER) {
		_normalizer->cancelStyle ((document::Style::Type) style->getType ());
		next = _scanner->fetchToken ();
	} else {
		g_assert (style && style->getPos () == token::Style::LEFT);
		next = parseStyle (style, (document::Style::Type) style->getType ());
	}
	return next;
}

/*!
 * This is a generalisation of parseBold(), parseItalic(), parseMonospace(), parseUnderline().
 *
 * \todo Abort at newline?
 */
token::Token * 
Parser::parseStyle (const token::Token *first, document::Style::Type type_)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::PARAGRAPH ||
	    first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	// bold       := style-lb ( text | markup )* style-rb
	// italic     := style-li ( text | markup )* style-ri
	// monospace  := style-lm ( text | markup )* style-rm
	// underline  := style-lu ( text | markup )* style-ru

	token::Style::Type type = (token::Style::Type) type_;
	g_assert (type == token::Style::ASTERISK || 
		  type == token::Style::SLASH || 
		  type == token::Style::BACKTICK || 
		  type == token::Style::UNDERSCORE);

	const token::Style *style;
	style = dynamic_cast<const token::Style *> (first);
	g_assert (style &&  style->getType () == type);

	_normalizer->pushStyle (type_);

	token::Token *token;
	token::Token *next;

	token = _scanner->fetchToken ();
	while (!((style = dynamic_cast<const token::Style *> (token)) != NULL &&
	          style->getType () == type && 
		  style->getPos () == token::Style::RIGHT) && 
	       token->getClass () != token::Token::PARAGRAPH &&
	       token->getClass () != token::Token::END) {

		if (token->getClass () == token::Token::STYLE) {
			next = parseMarkup (token);
		} else {
			next = parseText (token);
		}

		if (token != next) {
			delete token;
		}
		token = next;
	}	

	_normalizer->popStyle (type_);

	if (token->getClass () == token::Token::STYLE) {
		delete token;
		token = _scanner->fetchToken ();
	}
	return token;
}

/*!
 * Parse plain text.
 */
token::Token * 
Parser::parseText (const token::Token *first)
{
	TRACE (__FUNCTION__);

	if (first->getClass () == token::Token::PARAGRAPH ||
	    first->getClass () == token::Token::END) {
		// ok, because the token is owned by the caller
		return const_cast<token::Token *> (first);
	}

	g_assert (first->getClass () == token::Token::TEXT);
	gchar const *text = first->toString ();
	g_assert (text);
	g_return_val_if_fail (text, _scanner->fetchToken ());
	_normalizer->text (text);

	return _scanner->fetchToken ();
}
