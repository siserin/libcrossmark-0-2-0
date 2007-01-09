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

	def pushBlock (self, blockType):
		if blockType == crossmark.BLOCKQUOTE:
			typename = 'BLOCKQUOTE'
		elif blockType == crossmark.PARAGRAPH:
			typename = 'PARAGRAPH'
		elif blockType == crossmark.HEADING_1:
			typename = 'HEADING_1'
		elif blockType == crossmark.HEADING_2:
			typename = 'HEADING_2'
		elif blockType == crossmark.HEADING_3:
			typename = 'HEADING_3'
		elif blockType == crossmark.HEADING_4:
			typename = 'HEADING_4'
		else:
			typename = 'unknown'
		print "pushBlock (", typename, ")"

	def popBlock (self):
		print "popBlock"

	def pushStyle (self, styleType):
		if styleType == crossmark.BOLD: 
			typename = 'BOLD'
		if styleType == crossmark.ITALIC: 
			typename = 'ITALIC'
		if styleType == crossmark.MONOSPACE: 
			typename = 'MONOSPACE'
		if styleType == crossmark.UNDERLINE: 
			typename = 'UNDERLINE'
		print "pushStyle (", typename, ")"

	def popStyle (self, styleType):
		print "popStyle"

	def text (self, string):
		print '"' + string + '"'

if len (sys.argv) < 2: 
	sys.stderr.write ("Need filename\n")
	sys.exit (1)

listener = Listener ()
parser = crossmark.Parser (sys.argv[1], listener)
parser.parse ()
