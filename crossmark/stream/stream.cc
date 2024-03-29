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

#include "stream-private.hh"
#include "stdio-stream-private.hh"
#ifdef LIBCROSSMARK_FEATURE_LIBGSF
#include <gsf/gsf-utils.h>
#include "gsf-stream-private.hh"
#endif

using namespace crossmark;
using namespace crossmark::stream;

/*!
 * Stream factory singleton getter.
 */
Factory &
Factory::instance ()
{
	static Factory *factory = NULL;

	if (!factory) {
#ifdef LIBCROSSMARK_FEATURE_LIBGSF
		gsf_init ();
#endif
		factory = new Factory ();
	}

	return *factory;
}

/*!
 * Create default input implementation.
 */
Input * 
Factory::createInput (gchar const *file)
{
#ifdef LIBCROSSMARK_FEATURE_LIBGSF
	return new GsfInput (file);
#else
	return new StdInput (file);
#endif
}

/*!
 * Create default output implementation.
 */
Output * 
Factory::createOutput (gchar const *file)
{
#ifdef LIBCROSSMARK_FEATURE_LIBGSF
	return new GsfOutput (file);
#else
	return new StdOutput (file);
#endif
}
