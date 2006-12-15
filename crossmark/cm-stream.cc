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

#include "config.h"
#include "cm-stream.hh"
#include "cm-stdio-stream-private.hh"
#if HAVE_LIBGSF
#include <gsf/gsf-utils.h>
#include "cm-gsf-stream-private.hh"
#endif

using namespace crossmark;

streams::Factory &
streams::Factory::instance ()
{
	static streams::Factory::Factory *factory = NULL;

	if (!factory) {
#if HAVE_LIBGSF
		gsf_init ();
#endif
		factory = new streams::Factory::Factory ();
	}

	return *factory;
}

streams::Input * 
streams::Factory::createInput (const std::string &file)
{
#if HAVE_LIBGSF
	return new streams::GsfInput (file);
#else
	return new streams::StdInput (file);
#endif
}

/*!
 * \todo Use libgsf.
 */
streams::Output * 
streams::Factory::createOutput (const std::string &file)
{
	return new streams::StdOutput (file);
}
