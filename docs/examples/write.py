#!/usr/bin/env python

##
# \example write.py
# \brief Basic crossmark writing example.
# This example writes a hardcoded crossmark file.

import crossmark
from crossmark import block
from crossmark import style
import sys

if len (sys.argv) < 2: 
	sys.stderr.write ("Need filename\n")
	sys.exit (1)

writer = crossmark.Writer (sys.argv[1])
writer.pushDocument ()

writer.pushBlock (block.HEADING_1)
writer.text ("heading 1")
writer.popBlock ()

writer.pushBlock (block.PARAGRAPH)
writer.text ("foo bar baz")
writer.popBlock ()

writer.popDocument ()
