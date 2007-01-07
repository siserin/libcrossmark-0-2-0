
%module crossmark
%{
#include "document.hh"
#include "parser.hh"
%}

class Document
{
public:
	virtual ~Document ();

	virtual void pushDocument ();
	virtual void popDocument ();

	virtual void text (const char *str);

	virtual void pushStyle (Document::Style::Type type);
	virtual void popStyle (Document::Style::Type type);

	virtual void pushBlock (Document::Block::Type type);
	virtual void popBlock ();
};

class Parser
{
public:
	Parser (const char *file, Document *reader);

	virtual ~Parser ();

	bool sputter ();
};
