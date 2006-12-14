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

#include <iostream>
#include <string>
#include <stdio.h>
#include <crossmark/crossmark.hh>

using namespace crossmark;

class HtmlConverter : public crossmark::Reader
{
public:
	~HtmlConverter () {}

	// document interface
	void pushDocument () 
	{
		std::cout << "<html><body>" << std::endl;
	}
	void popDocument ()
	{
		std::cout << std::endl << "</body></html>" << std::endl;
	}

	// text interface
	void text (const std::string &text)
	{
		std::cout << text.c_str ();
	}

	// style interface
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

	// document structure interface
	void pushStructure (document::Structure::Type type)
	{
		const gchar *structure;

		switch (type) {
		case document::Structure::BLOCKQUOTE: 
			structure = "<blockquote>";
			_structureStack.push ("</blockquote>");
			break;
		case document::Structure::PARAGRAPH: 
			structure = "<p>";
			_structureStack.push ("</p>");
			break;
		default: 
			g_assert (FALSE);
		}

		std::cout << structure;
	}
	void pushHeadingStructure (int level)
	{
		g_assert (FALSE);
	}
	void popStructure ()
	{
		g_assert (!_structureStack.empty ());
		std::cout << _structureStack.top ();
		_structureStack.pop ();
	}

private:
	std::stack<const gchar *> _styleStack;
	std::stack<const gchar *> _structureStack;
};

int
main (int 	  argc, 
      char 	**argv)
{
	if (argc < 2) {
		std::cerr << "Need filename\n" << std::endl;
		return 1;
	}

	HtmlConverter converter;

	std::string file (argv[1]);
	Source source (file, converter);
	source.sputter ();

	return 0;
}