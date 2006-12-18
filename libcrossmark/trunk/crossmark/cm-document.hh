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

/*!
 * \file cm-document.hh
 * \brief Crossmark document interfaces.
 */

#ifndef CM_DOCUMENT_HH
#define CM_DOCUMENT_HH

#include <list>
#include <map>
#include <stack>
#include <string>

namespace crossmark {

/*!
 * \brief Interface to crossmark documents.
 * 
 * Each module provides an interface for a specific aspect of the crossmark format.
 */
namespace document {

/*!
 * \brief Text interface for reader and writer implementations.
 *
 * This is the only way to pass text data.
 */
class Text 
{
public:
	virtual ~Text () {}

	virtual void text (const std::string &text) = 0;
};

/*!
 * \brief Note and annotation interface.
 */
class Note
{
public:
	enum Type {
		STANDARD,
		ANNOTATION
	};

	virtual ~Note () {}

	virtual void blockNote (Type type, 
				const std::map<char const *, char const *> *attribs, 
				const std::string &text) = 0;

	virtual void inlineNote (Type type, 
				 const std::string &text) = 0;
};

/*!
 * \brief Hyperlink interface.
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

	virtual ~Link () {}

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
 * \brief Text styling interface.
 */
class Style
{
public:
	// don't change order
	// see tokens::Style::Type
	enum Type {
		BOLD, 
		ITALIC,
		MONOSPACE,
		UNDERLINE
	};

	virtual ~Style () {}

	virtual void pushStyle (Type type) = 0;
	virtual void popStyle (Type type) = 0;
};

/*!
 * \brief Image interface for reader and writer implementations.
 *
 * \todo Pre-parse optional attributes like geo?
 * \todo If the "details" files contains exif, how will it be marked? Custom macro?
 */
class Image
{
public:	
	virtual ~Image () {}

	virtual void image (const std::string &uri, 
			    const std::string &caption, 
			    const std::map<char const *, char const *> &attribs);

	// helpers
	std::list<char const *> * parseCamera (const std::string &camera);
	
};

/*!
 * \brief Document outline interface.
 */
class Block 
{
public:
	enum Type {
		BLOCKQUOTE,
		PARAGRAPH,
		HEADING
	};

	virtual ~Block () {}

	virtual void pushBlock (Type type) = 0;
	virtual void pushHeading (int level) = 0;
	virtual void popBlock () = 0;
};

/*!
 * \brief List interface.
 *
 * \todo Need to pass numbering/bullet type, maybe other params for lists? 
 */
class List 
{
public:
	enum Type {
		ORDERED,
		UNORDERED
	};

	virtual ~List () {}

	virtual void pushList (Type type) = 0;
	virtual void popList () = 0;

	virtual void pushListItem () = 0;
	virtual void popListItem () = 0;
};

/*!
 * \brief Table interface.
 */
class Table 
{
public:
	virtual ~Table () {}

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
 * \brief Math interface.
 *
 * \todo Possible to use itex?
 * \todo Make compile-time optional
 */
class Math
{
public:
	virtual ~Math () {}
	
};

/*!
 * \brief Generic macro interface.
 */
class Macro 
{
public:
	virtual ~Macro () {}

	virtual void blockMacro (const std::string &name, 
				 const std::list<char const *> &params, 
				 const std::map<char const *, char const *> &attribs, 
				 const std::string &text) = 0;

	virtual void inlineMacro (const std::string &text) = 0;
};

}; // namespace document

/*!
 * \brief Document must implement all specific document interfaces.
 *
 * \note Maybe later we'll split mandatory and optional ones.
 */
class Document : public document::Text, 
		 //public document::Note,
		 //public document::Link,
		 public document::Style,
		 //public document::Image,
		 public document::Block
		 //public document::List,
		 //public document::Table,
		 //public document::Math, 
		 //public document::Macro
{
public:
	virtual ~Document () {}

	// document interface, 
	// TODO maybe pull out, but this doesn't have a 
	// crossmark counterpart
	virtual void pushDocument () = 0;
	virtual void popDocument () = 0;

	// text interface
	virtual void text (const std::string &text) = 0;

	// style interface
	virtual void pushStyle (document::Style::Type type) = 0;
	virtual void popStyle (document::Style::Type type) = 0;

	// document structure interface
	virtual void pushBlock (document::Block::Type type) = 0;
	virtual void pushHeading (int level) = 0;
	virtual void popBlock () = 0;
};

};  // namespace crossmark

#endif /* CM_DOCUMENT_HH */
