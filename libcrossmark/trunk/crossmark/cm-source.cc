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

#include "crossmark.hh"

using namespace crossmark;

Source::Source (const std::string &file, 
		Reader &reader)
  : _reader (reader),
    _scanner (* new Scanner (file))
{

}

Source::Source (streams::Input &istream, 
		Reader &reader)
  : _reader (reader),
    _scanner (* new Scanner (istream))
{

}

Source::~Source ()
{
	delete &_scanner;
}

/*
# Scanner tokens:
sof eof pbreak newline indent text 
style-lb style-rb style-li style-ri style-lm style-rm style-lu style-ru

# Some quickie top-down grammar, flaws:
# + way incomplete
# + arbitrary nesting of styling allowed
# + ...

document   := sof newline* ( pbreak* (paragraph | blockquote) )* pbreak* eof
paragraph  := ( text | markup )* pbreak
blockquote := indent line ( newline indent line )* pbreak
line       := ( text | markup )*
markup     := bold | italic | monospace | underline
bold       := style-lb ( text | markup )* style-rb
italic     := style-li ( text | markup )* style-ri
monospace  := style-lm ( text | markup )* style-rm
underline  := style-lu ( text | markup )* style-ru
*/
gboolean 
Source::sputter ()
{
	parseDocument ();
}

void 
Source::parseDocument ()
{
	_reader.pushDocument ();

	// fetch token

	_reader.popDocument ();
}

void 
Source::parseParagraph ()
{

}

void 
Source::parseBlockquote ()
{

}

void 
Source::parseLine ()
{

}

void 
Source::parseMarkup ()
{

}

void 
Source::parseBold ()
{

}

void 
Source::parseItalic ()
{

}

void 
Source::parseMonospace ()
{

}

void 
Source::parseUnderline ()
{

}


