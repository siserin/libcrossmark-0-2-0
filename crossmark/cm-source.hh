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
 * \file cm-source.hh
 * \brief Crossmark input.
 */

#ifndef CM_SOURCE_HH
#define CM_SOURCE_HH

#include <string>
#include <crossmark/cm-features.hh>
#include <crossmark/cm-document.hh>
#include <crossmark/cm-stream.hh>

namespace crossmark {

namespace tokens {
	class Token;
};
class Scanner;
class Validator;

/*!
 * \brief Crossmark parser implementation.
 */
class Source 
{
public:
	Source (const std::string &file, 
		Document &reader);

	Source (stream::Input &istream, 
		Document &reader);

	virtual ~Source ();

	/*!
	 * \todo use GError or exceptions?
	 */
	gboolean sputter ();

protected:
	void parseDocument ();
	tokens::Token * parseParagraph (const tokens::Token *first);
	tokens::Token * parseBlockquote (const tokens::Token *first);
	tokens::Token * parseLine (const tokens::Token *first);
	tokens::Token * parseMarkup (const tokens::Token *first);
	tokens::Token * parseStyle (const tokens::Token *first, document::Style::Type type);
	tokens::Token * parseText (const tokens::Token *first);

private:
	Scanner   *_scanner;
	Validator *_validator;
};

};  // namespace crossmark

#endif /* CM_SOURCE_HH */
