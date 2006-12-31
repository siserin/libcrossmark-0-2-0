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
 * \file cm-parser.hh
 * \brief Crossmark input.
 */

#ifndef CM_PARSER_HH
#define CM_PARSER_HH

#include <crossmark/cm-features.hh>
#include <crossmark/cm-document.hh>
#include <crossmark/cm-stream.hh>

namespace crossmark {

namespace token {
	class Token;
};
class Scanner;
class Normalizer;

/*!
 * \brief Crossmark parser implementation.
 */
class Parser 
{
public:
	Parser (gchar const *file, Document &reader);

	Parser (stream::Input &istream, Document &reader);

	virtual ~Parser ();

	/*!
	 * \todo use GError or exceptions?
	 */
	gboolean sputter ();

protected:
	void parseDocument ();
	token::Token * parseBlock (const token::Token *first);
	token::Token * parseBlockquote (const token::Token *first);
	token::Token * parseLine (const token::Token *first);
	token::Token * parseMarkup (const token::Token *first);
	token::Token * parseStyle (const token::Token *first, document::Style::Type type);
	token::Token * parseText (const token::Token *first);

private:
	Scanner    *_scanner;
	Normalizer *_normalizer;
};

};  // namespace crossmark

#endif /* CM_PARSER_HH */
