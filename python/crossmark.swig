
/*
 * Hi Dom. 
 * Think i have fixed the problem I've been bugging you about.
 * Thanks.
 */

%module crossmark
%{
#include "document.hh"
%}

class Document
{
public:
	virtual ~Document ();

	virtual void pushDocument ();
	virtual void popDocument ();

	virtual void text (gchar const *str);

	virtual void pushStyle (Document::Style::Type type);
	virtual void popStyle (Document::Style::Type type);

	virtual void pushBlock (Document::Block::Type type);
	virtual void popBlock ();
};
