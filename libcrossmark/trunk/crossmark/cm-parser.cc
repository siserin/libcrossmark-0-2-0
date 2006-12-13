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

bool 
Source::sputter ()
{

}
