#!/usr/bin/env python

import crossmark
import sys



class Document (crossmark.Document):

	def pushDocument (self):
		print "pushDocument"

	def popDocument (self):
		print "popDocument"

	def pushBlock (self, blockType):
		print "pushBlock ", blockType

	def popBlock (self):
		print "popBlock"

	def pushStyle (self, styleType):
		print "pushStyle ", styleType

	def popStyle (self, styleType):
		print "popStyle ", styleType

	def text (self, string):
		print string



if len (sys.argv) < 2: 
	sys.stderr.write ("Need filename\n")
	sys.exit (1)

document = Document ()
parser = crossmark.Parser (sys.argv[1], document)
parser.sputter ()
