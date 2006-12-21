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

#include <glib.h>
#include "crossmark.hh"

using namespace crossmark;

/*!
 * \sa Document::text()
 */
void 
Writer::text (gchar const *str)
{
	g_assert (_ostream);

	fputs (str, _ostream);
}

/*!
 * \sa Document::pushStyle()
 */
void 
Writer::pushStyle (document::Style::Type type)
{
	g_assert (_ostream);

	switch (type) {
	case document::Style::BOLD:
		fputc ('*', _ostream);
		break;
	case document::Style::ITALIC:
		fputc ('/', _ostream);
		break;
	case document::Style::MONOSPACE:
		fputc ('`', _ostream);
		break;
	case document::Style::UNDERLINE:
		fputc ('_', _ostream);
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, type);
	}
}

/*!
 * \sa Document::popStyle()
 */
void 
Writer::popStyle (document::Style::Type type)
{
	g_assert (_ostream);

	switch (type) {
	case document::Style::BOLD:
		fputc ('*', _ostream);
		break;
	case document::Style::ITALIC:
		fputc ('/', _ostream);
		break;
	case document::Style::MONOSPACE:
		fputc ('`', _ostream);
		break;
	case document::Style::UNDERLINE:
		fputc ('_', _ostream);
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, type);
	}
}
