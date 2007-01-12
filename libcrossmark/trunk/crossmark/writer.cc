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
#include "stream/stream-private.hh"

using namespace crossmark;

/*!
 * Create writer for file.
 */
Writer::Writer (gchar const *file)
  : _ostream (* stream::Factory::instance().createOutput (file)),
    _ownStream (TRUE)
{}

/*!
 * Create writer for stream.
 */
Writer::Writer (stream::Output &ostream)
  : _ostream (ostream),
    _ownStream (FALSE)
{}

/*!
 * Dtor.
 */
Writer::~Writer ()
{
	if (_ownStream) {
		delete &_ostream;
	}
}

void 
Writer::pushDocument ()
{}

void 
Writer::popDocument ()
{}

/*!
 * \sa Document::text()
 */
void 
Writer::text (gchar const *str)
{
	switch (_currentBlock) {
	case document::Block::HEADING_1:
	case document::Block::HEADING_2:
		_headingLength += g_utf8_strlen (str, -1);
	}

	_ostream.write (str);
}

/*!
 * \sa Document::pushStyle()
 */
void 
Writer::pushStyle (document::Style::Type type)
{
	switch (type) {
	case document::Style::BOLD:
		_ostream.write ('*');
		break;
	case document::Style::ITALIC:
		_ostream.write ('/');
		break;
	case document::Style::MONOSPACE:
		_ostream.write ('`');
		break;
	case document::Style::UNDERLINE:
		_ostream.write ('_');
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, type);
	}

	_styleStack.push (type);

	// keep track of heading length
	switch (_currentBlock) {
	case document::Block::HEADING_1:
	case document::Block::HEADING_2:
		++_headingLength;
	}
}

/*!
 * \sa Document::popStyle()
 */
void 
Writer::popStyle ()
{
	switch (_styleStack.top ()) {
	case document::Style::BOLD:
		_ostream.write ('*');
		break;
	case document::Style::ITALIC:
		_ostream.write ('/');
		break;
	case document::Style::MONOSPACE:
		_ostream.write ('`');
		break;
	case document::Style::UNDERLINE:
		_ostream.write ('_');
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, _styleStack.top ());
	}

	_styleStack.pop ();

	// keep track of heading length
	switch (_currentBlock) {
	case document::Block::HEADING_1:
	case document::Block::HEADING_2:
		++_headingLength;
	}
}

/*!
 * \todo implement
 */
void 
Writer::pushLink (document::Link::Type type, 
		  gchar const *target,
		  gchar const *scroll)
{

}

/*!
 * \todo implement
 */
void 
Writer::popLink ()
{

}

void 
Writer::pushBlock (document::Block::Type type)
{
	_currentBlock = type;

	switch (_currentBlock) {
	case document::Block::BLOCKQUOTE:
	case document::Block::PARAGRAPH:
	case document::Block::HEADING_1:
	case document::Block::HEADING_2:
		_headingLength = 0;
		break;
	case document::Block::HEADING_3:
		_ostream.write ("=== ");
		break;
	case document::Block::HEADING_4:
		_ostream.write ("==== ");
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, _currentBlock);
	}
}

/*!
 * \todo Count h1, h2 length.
 */
void 
Writer::popBlock ()
{
	gchar *underline;

	switch (_currentBlock) {
	case document::Block::BLOCKQUOTE:
	case document::Block::PARAGRAPH:
		_ostream.write ("\n\n");
		break;
	case document::Block::HEADING_1:
		underline = g_strnfill (_headingLength, '=');
		_ostream.write ("\n");
		_ostream.write (underline);
		_ostream.write ("\n\n");
		g_free (underline); underline = NULL;
		break;
	case document::Block::HEADING_2:
		underline = g_strnfill (_headingLength, '-');
		_ostream.write ("\n");
		_ostream.write (underline);
		_ostream.write ("\n\n");
		g_free (underline); underline = NULL;
		break;
	case document::Block::HEADING_3:
		_ostream.write (" ===\n\n");
		break;
	case document::Block::HEADING_4:
		_ostream.write (" ====\n\n");
		break;
	default:
		g_warning ("%s: type == %d", 
			   __FUNCTION__, _currentBlock);
	}
}
