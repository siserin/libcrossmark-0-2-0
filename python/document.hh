
#ifndef DOCUMENT_HH
#define DOCUMENT_HH

#include <crossmark/cm-document.hh>

class Document : public crossmark::Document
{
public:
	virtual ~Document () {}

	virtual void pushDocument () {}
	virtual void popDocument () {}

	virtual void text (gchar const *str) {}

	virtual void pushStyle (Document::Style::Type type) {}
	virtual void popStyle (Document::Style::Type type) {}

	virtual void pushBlock (Document::Block::Type type) {}
	virtual void popBlock () {}
};

#endif // DOCUMENT_HH
