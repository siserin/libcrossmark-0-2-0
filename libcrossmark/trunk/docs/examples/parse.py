#!/usr/bin/env python

##
# \example parse.py
# \brief Basic crossmark parsing example.

import crossmark
from crossmark import block
from crossmark import style
import sys

class Listener (crossmark.Document):

	def pushDocument (self):
		print "pushDocument"

	def popDocument (self):
		print "popDocument"

	def pushBlock (self, klass):
		if klass == block.BLOCKQUOTE:
			typename = 'BLOCKQUOTE'
		elif klass == block.PARAGRAPH:
			typename = 'PARAGRAPH'
		elif klass == block.HEADING_1:
			typename = 'HEADING_1'
		elif klass == block.HEADING_2:
			typename = 'HEADING_2'
		elif klass == block.HEADING_3:
			typename = 'HEADING_3'
		elif klass == block.HEADING_4:
			typename = 'HEADING_4'
		else:
			typename = 'unknown'
		print "pushBlock (", typename, ")"

	def popBlock (self):
		print "popBlock"

	def pushStyle (self, klass):
		if klass == style.BOLD: 
			typename = 'BOLD'
		elif klass == style.ITALIC: 
			typename = 'ITALIC'
		elif klass == style.MONOSPACE: 
			typename = 'MONOSPACE'
		elif klass == style.UNDERLINE: 
			typename = 'UNDERLINE'
		print "pushStyle (", typename, ")"

	def popStyle (self):
		print "popStyle"

	def text (self, string):
		print '"' + string + '"'

if len (sys.argv) < 2: 
	sys.stderr.write ("Need filename\n")
	sys.exit (1)

listener = Listener ()
parser = crossmark.Parser (sys.argv[1], listener)
parser.parse ()
