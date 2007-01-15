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
 * \example xm2dox.cc
 * \brief Simple crossmark to doxygen converter example.
 * This application reads a crossmark file and writes the equivalent 
 * in doxygen format to stdout.
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <crossmark/crossmark.hh>

using namespace crossmark;

class DoxConverter : public crossmark::Document
{
public:
	DoxConverter () 
	{
		srandom (time (0));
	}

	~DoxConverter () {}

	void pushDocument () 
	{}

	void popDocument ()
	{}

	void text (gchar const *str)
	{
		std::cout << str;
	}

	void pushStyle (document::Style::Type type)
	{
		const gchar *style;

		switch (type) {
		case document::Style::BOLD: 
			style = "<b>";
			_styleStack.push ("</b>");
			break;
		case document::Style::ITALIC: 
			style = "<i>";
			_styleStack.push ("</i>");
			break;
		case document::Style::MONOSPACE: 
			style = "<code>";
			_styleStack.push ("</code>");
			break;
		case document::Style::UNDERLINE: 
			style = "<u>";
			_styleStack.push ("</u>");
			break;
		default: 
			g_assert (FALSE);
		}
	
		std::cout << style;
	}
	void popStyle ()
	{
		g_assert (!_styleStack.empty ());
		std::cout << _styleStack.top ();
		_styleStack.pop ();
	}

	/*!
 	 * \todo implement
 	 */
	void pushLink (document::Link::Type type, 
		       gchar const *target,
		       gchar const *scroll)
	{

	}

	/*!
 	 * \todo implement
 	 */
	void popLink ()
	{

	}

	void pushBlock (document::Block::Type type)
	{
		switch (type) {
		case document::Block::BLOCKQUOTE: 
			std::cout << "\n<blockquote>";
			break;
		case document::Block::PARAGRAPH: 
			// the closing \n\n of the previous paragraph 
			// should suffice
			break;
		case document::Block::HEADING_1: 
			std::cout << "\n\\section s" << random () << " ";
			break;
		case document::Block::HEADING_2: 
			std::cout << "\n\\subsection s" << random () << " ";
			break;
		case document::Block::HEADING_3: 
			std::cout << "\n\\subsubsection s" << random () << " ";
			break;
		case document::Block::HEADING_4: 
			// TODO doxygen only supports 3 levels of headers
			g_assert (FALSE);
			break;
		default: 
			g_assert (FALSE);
		}
	}

	void popBlock ()
	{
		std::cout << std::endl << std::endl;
	}

private:
	std::stack<const gchar *> _styleStack;
};

int
main (int 	  argc, 
      char 	**argv)
{
	if (argc < 2) {
		std::cerr << "Need filename\n" << std::endl;
		return 1;
	}

	DoxConverter converter;
	Parser parser (argv[1], converter);
	parser.parse ();

	return 0;
}
