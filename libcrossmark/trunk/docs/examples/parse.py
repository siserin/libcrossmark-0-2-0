#!/usr/bin/env python

##
# \example parse.py
# \brief Basic crossmark parsing example.

import crossmark
import sys

class Listener (crossmark.Document):

	def pushDocument (self):
		print "pushDocument"

	def popDocument (self):
		print "popDocument"

	def pushBlock (self, block):
		if block == crossmark.BLOCKQUOTE:
			typename = 'BLOCKQUOTE'
		elif block == crossmark.PARAGRAPH:
			typename = 'PARAGRAPH'
		elif block == crossmark.HEADING_1:
			typename = 'HEADING_1'
		elif block == crossmark.HEADING_2:
			typename = 'HEADING_2'
		elif block == crossmark.HEADING_3:
			typename = 'HEADING_3'
		elif block == crossmark.HEADING_4:
			typename = 'HEADING_4'
		else:
			typename = 'unknown'
		print "pushBlock (", typename, ")"

	def popBlock (self):
		print "popBlock"

	def pushStyle (self, style):
		if style == crossmark.BOLD: 
			typename = 'BOLD'
		elif style == crossmark.ITALIC: 
			typename = 'ITALIC'
		elif style == crossmark.MONOSPACE: 
			typename = 'MONOSPACE'
		elif style == crossmark.UNDERLINE: 
			typename = 'UNDERLINE'
		print "pushStyle (", typename, ")"

	def popStyle (self, style):
		print "popStyle"

	def text (self, string):
		print '"' + string + '"'

if len (sys.argv) < 2: 
	sys.stderr.write ("Need filename\n")
	sys.exit (1)

listener = Listener ()
parser = crossmark.Parser (sys.argv[1], listener)
parser.parse ()
