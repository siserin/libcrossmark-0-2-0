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
 * \file crossmark.hh
 * \brief Crossmark main include.
 */

#ifndef CROSSMARK_HH
#define CROSSMARK_HH

#include <stack>
#include <string>
#include <crossmark/cm-features.hh>
#include <crossmark/cm-document.hh>
#include <crossmark/cm-stream.hh>

/*!
 * \brief Toplevel crossmark namespace. 
 * 
 * \todo Maybe use glib::ustring instead of std::string and glib::TimeVal?
 * \todo Pass errors to the Document, return them from Source::sputter() or even both?
 * \todo Not quite happy with citations, e.g. the sample from the v4 spec. Since crossmark
	 parsers must not handle macros parsing would be up to the application. Or does
	 that not hold for builtin macros?
 * \todo Helper functions
	 Timestamp * parseTimestamp (std::string &time); // or find a widely used c++ time class
 * \author Robert Staudinger <robert.staudinger@gmail.com>
 */
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

/*!
 * \brief Writer must implement all specific document interfaces.
 *
 * \note Maybe later we'll split mandatory and optional ones.
 *
 * \todo Does the writer validate?
 * \todo Implement state machine.
 */
class Writer : public Document
{
friend class Sink;
public:
	virtual ~Writer ();

	virtual void text (const std::string &text);

	virtual void pushStyle (document::Style::Type type);
	virtual void popStyle (document::Style::Type type);

	// TODO implement

private:
	void setStream (FILE *ostream);

	FILE *_ostream;
};

/*!
 * \brief Wrapper for opening and closing files or streams to write to.
 */
class Sink
{
public:
	Sink (const std::string &file, 
	      Writer &writer);

	virtual ~Sink ();
};

};  // namespace crossmark

#endif /* CROSSMARK_HH */
