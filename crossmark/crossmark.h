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

#ifndef CROSSMARK_H
#define CROSSMARK_H

#include <list>
#include <map>
#include <stack>
#include <string>

/*

Initial poking at grammar, just to wrap my head around crossmark.
Will not be implemente like that, need to work out scanner before.

Some obvious flaws, notes:
+ incomplete
+ arbitrary nesting of styling allowed
+ many more

# Some quickie top-down grammar, to wrap my head around the problem
document   := [ paragraph | blockquote ]*
paragraph  := [ text | markup ]* '\n' '\n'
markup     := bold | italic | monospace | underline
bold       := " *" [ {charset} | markup ]* "* "
italic     := " /" [ {charset} | markup ]* "/ "
monospace  := " `" [ {charset} | markup ]* "` "
underline  := " _" [ {charset} | markup ]* "_ "
blockquote := [ ' '* line '\n' ]*
line       := [ text | markup ]*
text       := {charset}*
*/

/*!
 * Crossmark implementation.
 * \todo Maybe use glib::ustring instead of std::string and glib::TimeVal?
 * \todo Pass errors to the Reader, return them from Source::sputter() or even both?
 * \todo Not quite happy with citations, e.g. the sample from the v4 spec. Since crossmark
	 parsers must not handle macros parsing would be up to the application. Or does
	 that not hold for builtin macros?
 * \todo Helper functions
	 Timestamp * parseTimestamp (std::string &time); // or find a widely used c++ time class
 * \author Robert Staudinger <robert.staudinger@gmail.com>
 */
namespace crossmark {

/*!
 * Each module provides an interface for a specific aspect of the crossmark format.
 * The goal ist to re-use 
 */
namespace modules {

/*!
 * Content interface for reader and writer implementations.
 * This is the only way to pass text data.
 */
class Text 
{
public:
	virtual ~Text ();

	virtual void text (const std::string &text) = 0;
};

/*!
 * Note and annotation interface.
 */
class Note
{
public:
	enum Type {
		STANDARD,
		ANNOTATION
	};

	virtual ~Note ();

	virtual void blockNote (Type type, 
				const std::map<char const *, char const *> *attribs, 
				const std::string &text) = 0;

	virtual void inlineNote (Type type, 
				 const std::string &text) = 0;
};

/*!
 * Hyperlink interface for reader and writer implementations.
 */
class Link
{
public:
	enum Type {
		INTERNAL,		//< Link leads to the specified wiki-internal page.
		INTERNAL_HEADING,	//< Link leads to the specified heading on the specified page.
		INTERNAL_SECTION,	//< Link leads to the specified numbered section on the specified page.
		INTERNAL_LABEL,		//< Link with custom label, leads to specified wiki-internal page.

		EXTERNAL,		//< External link.
		EXTERNAL_LABEL		//< External link with custom label.
	};

	virtual ~Link ();

	virtual void internalLink (Type type, 
				   const std::string &anchor) = 0;

	virtual void internalCustomLink (Type type, 
					 const std::string &anchor, 
					 const std::string &param) = 0;
	
	virtual void externalLink (Type type, 
				   const std::string &uri) = 0;

	virtual void externalCustomLink (Type type, 
					 const std::string &uri, 
					 const std::string &param) = 0;
};

/*!
 * Text styling interface for reader and writer implementations.
 */
class Style
{
public:
	enum Type {
		BOLD, 
		ITALIC,
		MONOSPACE,
		UNDERLINE
	};

	virtual ~Style ();

	virtual void pushStyle (Type type) = 0;
	virtual void popStyle () = 0;
};

/*!
 * Image interface for reader and writer implementations.
 * \todo Are images always stored externally, like in HTML?
 * \todo Pre-parse optional attributes like geo?
 * \todo If the "details" files contains exif, how will it be marked? Custom macro?
 */
class Image
{
public:	
	virtual ~Image ();

	virtual void image (const std::string &uri, 
			    const std::string &caption, 
			    const std::map<char const *, char const *> &attribs);

	// helpers
	std::list<char const *> * parseCamera (const std::string &camera);
	
};

/*!
 * Document outline interface for reader and writer implementations.
 */
class Structure 
{
public:
	enum Type {
		BLOCKQUOTE,
		PARAGRAPH,
		HEADING
	};

	virtual ~Structure ();

	/*!
	 * \todo Need to pass numbering/bullet type, maybe other params for lists? 
		 -- Hmm, maybe that kind of styling should be separated anyways.
	 */
	virtual void pushStructure (Type type) = 0;
	virtual void pushHeadingStructure (int level) = 0;
	virtual void popStructure () = 0;
};

/*!
 * List interface.
 */
class List 
{
public:
	enum Type {
		ORDERED,
		UNORDERED
	};

	virtual ~List ();

	virtual void pushList (Type type) = 0;
	virtual void popList () = 0;

	virtual void pushListItem () = 0;
	virtual void popListItem () = 0;
};

/*!
 * Table interface.
 */
class Table 
{
public:
	virtual ~Table ();

	virtual void pushTable (const std::string &caption) = 0;
	virtual void popTable () = 0;

	virtual void pushHeader () = 0;
	virtual void popHeader () = 0;

	virtual void pushRow () = 0;
	virtual void popRow () = 0;

	virtual void pushCell () = 0;
	virtual void popCell () = 0;
};

/*!
 * Math interface.
 * \todo Possible to use itex?
 * \todo Make compile-time optional
 */
class Math
{
public:
	virtual ~Math ();
	
};

/*!
 * Generic macro interface.
 */
class Macro 
{
public:
	virtual ~Macro ();

	virtual void blockMacro (const std::string &name, 
				 const std::list<char const *> &params, 
				 const std::map<char const *, char const *> &attribs, 
				 const std::string &text) = 0;

	virtual void inlineMacro (const std::string &text) = 0;
};

}; // namespace modules

/*!
 * The ``editor'' namespace specialises modules so they can be used for
 * automatic input conversion. 
 *
 * E.g. after input of "*foo" it should
 * automatically switch to boldface, but if the next "*" doesn't match
 * word boundaries boldface would have to be cancelled and reverted.
 * Blockquotes are similar, e.g. 3 times <enter> ends blockquote mode.
 * 
 * It will be important to (1) keep the input parser in sync when formatting 
 * is done using the toolbar and (2) resync when the cursor is moved using
 * mouse or keyboard navigation. 
 * For (2) we may just re-parse from the beginning of the block.
 * \see Tracker::resume()
 * 
 * \todo Figure out a better name. Calling it trackers for now because 
	 it keeps track of keystrokes.
 */
namespace trackers {

class Style : public modules::Style
{
public:
	virtual ~Style ();

	virtual void pushStyle (Type type) = 0;
	virtual void cancelStyle (Type type) = 0;
	virtual void popStyle () = 0;
};

}; // namespace trackers

/*!
 * Reader must implement all specific interfaces.
 * Maybe later we'll split mandatory and optional ones.
 */
class Reader : public modules::Text, 
	       public modules::Note,
	       public modules::Link,
	       public modules::Style,
	       public modules::Image,
	       public modules::Structure, 
	       public modules::List,
	       public modules::Table,
	       public modules::Math, 
	       public modules::Macro
{
public:
	virtual ~Reader ();

	// TODO implement modules
};

/*
 * \todo Figure out a better name. Calling it trackers for now because 
	 it keeps track of keystrokes.
 * \todo Should this be in the namespace of the same name?
 */
class Tracker : public trackers::Style 
{
public:
	virtual ~Tracker ();

	/*!
	 * This sets the tracker to initial state. 
	 * \see resume
 	 */
	virtual void reset ();

	/*!
	 * Resume tracking and making formatting predictions.
	 *
	 * Between reset() and resume() all content between the 
	 * start of the block and the cursor position has to be 
	 * passed to the tracker, so it'll settle in a consistent 
	 * state before editing is resumed.
	 *
	 * While reparsing no predictions will be issued.
	 *
	 * The reset, reparse, resume chain has to be run when
	 * editing is resumed after the caret has been moved.
	 */
	virtual void resume ();


	// TODO implement trackers	
};

/*!
 * Source is mainly a wrapper for opening and closing files or streams to read from.
 */
class Source 
{
public:
	Source (const std::string &file, 
		Reader &reader);

	/* maybe optionally: 
	Source (GsfInput *input, 
		Reader &reader);
	*/

	virtual ~Source ();

	/*!
	 * \todo use GError or exceptions?
	 */
	bool open (const std::string &file);
	/* maybe optionally: 
	bool open (GsfInput *input);
	*/

	/*!
	 * \todo use GError or exceptions?
	 */
	bool sputter ();

	/*!
	 * \todo use GError or exceptions?
	 */
	bool close ();
};

/*!
 * Writer must implement all specific interfaces.
 * Maybe later we'll split mandatory and optional ones.
 * \todo Does the writer validate?
 * \todo Implement state machine.
 */
class Writer : public modules::Text, 
	       public modules::Note,
	       public modules::Link,
	       public modules::Style,
	       public modules::Image,
	       public modules::Structure, 
	       public modules::List,
	       public modules::Table,
	       public modules::Math, 
	       public modules::Macro
{
friend class Sink;
public:
	virtual ~Writer ();

	virtual void text (const std::string &text);

	virtual void pushStyle (modules::Style::Type type);
	virtual void popStyle ();

	// TODO implement

private:
	void setStream (FILE *ostream);

	FILE *_ostream;
	std::stack<modules::Style::Type> _styleStack;
};

/*!
 * Sink is mainly a wrapper for opening and closing files or streams to write to.
 */
class Sink
{
public:
	Sink (const std::string &file, 
	      Writer &writer);

	/* maybe optionally: 
	Sink (GsfOutput *output, 
	      Writer &writer);
	*/

	virtual ~Sink ();

	/*!
	 * \todo use GError or exceptions?
	 */
	bool open (const std::string &file);
	// maybe optionally: bool open (GsfOutput *output);

	/*!
	 * \todo use GError or exceptions?
	 */
	bool close ();
};

};  // namespace crossmark

#endif /* CROSSMARK_H */
