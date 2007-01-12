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

#ifndef BLOCK_HH
#define BLOCK_HH

#include <crossmark/crossmark.hh>

enum BlockType {
	BLOCKQUOTE = crossmark::document::Block::BLOCKQUOTE,
	PARAGRAPH = crossmark::document::Block::PARAGRAPH,
	HEADING_1 = crossmark::document::Block::HEADING_1,
	HEADING_2 = crossmark::document::Block::HEADING_2,
	HEADING_3 = crossmark::document::Block::HEADING_3,
	HEADING_4 = crossmark::document::Block::HEADING_4
};

#endif // BLOCK_HH
