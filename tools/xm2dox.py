#!/usr/bin/env python

# Copyright (C) 2006, Robert Staudinger <robert.staudinger@gmail.com>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#

import crossmark
from crossmark import block
from crossmark import style
import random
import sys

class Converter (crossmark.Document): 

	def _randId (self): 

		return str (random.randint(0, sys.maxint))

	def pushBlock (self, klass):

		if klass == block.BLOCKQUOTE: 
			pass
			# TODO
		elif klass == block.PARAGRAPH:
			pass
			# the closing \n\n of the previous paragraph should suffice
		elif klass == block.HEADING_1:
			sys.stdout.write ("\n\\section s" + self._randId () + " ")
		elif klass == block.HEADING_2:
			sys.stdout.write ("\n\\subsection s" + self._randId () + " ")
		elif klass == block.HEADING_3:
			sys.stdout.write ("\n\\subsubsection s" + self._randId () + " ")
		elif klass == block.HEADING_4: 
			pass
			# TODO

	def popBlock (self):

		sys.stdout.write ("\n\n")

	def pushStyle (self, klass):

		if klass == style.BOLD:
			sys.stdout.write ("<b>")
		elif klass == style.ITALIC:
			sys.stdout.write ("<i>")
		elif klass == style.MONOSPACE:
			sys.stdout.write ("<code>")
		elif klass == style.UNDERLINE:
			sys.stdout.write ("<u>")

	def popStyle (self, klass):

		if klass == style.BOLD:
			sys.stdout.write ("</b>")
		elif klass == style.ITALIC:
			sys.stdout.write ("</i>")
		elif klass == style.MONOSPACE:
			sys.stdout.write ("</code>")
		elif klass == style.UNDERLINE:
			sys.stdout.write ("</u>")

	def text (self, string):
		
		sys.stdout.write (string)



if len (sys.argv) < 2: 
        sys.stderr.write ("Need filename\n")
        sys.exit (1)

converter = Converter ()
parser = crossmark.Parser (sys.argv[1], converter)
parser.parse ()
