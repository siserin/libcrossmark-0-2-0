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

#include <fstream>
#include "crossmark.h"

crossmark::Sink::Sink (const std::string &file, 
		       Writer &writer)
  : _ostream (NULL)
{
	_ostream = new std::ofstream (file.c_str (), 
				      std::ios_base::out | std::ios_base::trunc);
	if (_ostream) {
		writer.setStream (*_ostream);
	}
}

crossmark::Sink::Sink (std::ostream stream, 
		       Writer &writer)
{

}

crossmark::Sink::~Sink ()
{

}

/*!
 * \todo use GError or exceptions?
 */
bool 
crossmark::Sink::open (const std::string &file)
{

}

bool 
crossmark::Sink::open (std::ostream stream)
{

}

/*!
 * \todo use GError or exceptions?
 */
bool 
crossmark::Sink::close ()
{

}
