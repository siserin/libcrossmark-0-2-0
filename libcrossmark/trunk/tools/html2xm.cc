
#include <crossmark/crossmark.hh>
#include <glib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace crossmark;

static void
start (GMarkupParseContext *context,
       const gchar         *element,
       const gchar        **attrs,
       const gchar        **vals,
       gpointer             writer_,
       GError             **error)
{
	Writer *writer = static_cast<Writer *> (writer_);
	
	// skip these
	if (0 == strcmp (element, "html") || 
	    0 == strcmp (element, "body")) {
		return;
	}

	// block
	if (0 == strcmp (element, "blockquote")) {
		writer->pushBlock (document::Block::PARAGRAPH);
	} else if (0 == strcmp (element, "p")) {
		writer->pushBlock (document::Block::PARAGRAPH);
	} else if (0 == strcmp (element, "h1")) {
		writer->pushBlock (document::Block::HEADING_1);
	} else if (0 == strcmp (element, "h2")) {
		writer->pushBlock (document::Block::HEADING_2);
	} else if (0 == strcmp (element, "h3")) {
		writer->pushBlock (document::Block::HEADING_3);
	} else if (0 == strcmp (element, "h4")) {
		writer->pushBlock (document::Block::HEADING_4);
	} 
	// inline
	else if (0 == strcmp (element, "b")) {
		writer->pushStyle (document::Style::BOLD);
	} else if (0 == strcmp (element, "i")) {
		writer->pushStyle (document::Style::ITALIC);
	} else if (0 == strcmp (element, "code")) {
		writer->pushStyle (document::Style::MONOSPACE);
	} else if (0 == strcmp (element, "u")) {
		writer->pushStyle (document::Style::UNDERLINE);
	} else {
		g_assert (FALSE);
	}
}

static void
end (GMarkupParseContext *context,
     const gchar         *element,
     gpointer             writer_,
     GError             **error)
{
	Writer *writer = static_cast<Writer *> (writer_);

	// skip these
	if (0 == strcmp (element, "html") || 
	    0 == strcmp (element, "body")) {
		return;
	}

	// block
	if (0 == strcmp (element, "blockquote")) {
		writer->popBlock ();
	} else if (0 == strcmp (element, "p")) {
		writer->popBlock ();
	} else if (0 == strcmp (element, "h1")) {
		writer->popBlock ();
	} else if (0 == strcmp (element, "h2")) {
		writer->popBlock ();
	} else if (0 == strcmp (element, "h3")) {
		writer->popBlock ();
	} else if (0 == strcmp (element, "h4")) {
		writer->popBlock ();
	} 
	// inline
	else if (0 == strcmp (element, "b")) {
		writer->popStyle ();
	} else if (0 == strcmp (element, "i")) {
		writer->popStyle ();
	} else if (0 == strcmp (element, "code")) {
		writer->popStyle ();
	} else if (0 == strcmp (element, "u")) {
		writer->popStyle ();
	} else {
		g_assert (FALSE);
	}
}

static void
text (GMarkupParseContext *context,
      const gchar         *text,
      gsize                length,  
      gpointer             writer_,
      GError             **error)
{
	Writer *writer = static_cast<Writer *> (writer_);

	if (*text == '\n') {
		return;
	}

	writer->text (text);
}

int
main (int 	  argc, 
      char 	**argv)
{
	GMarkupParseContext 	*context;
	GMarkupParser		 parser;
	GError			*error;
	gboolean		 ret;
	gchar			*buffer;
	gsize			 length;

	if (argc < 2) {
		std::cerr << "Need filename\n" << std::endl;
		return 1;
	}

	parser.start_element = start;
	parser.end_element = end;
	parser.text = text;

	stream::Output *output = stream::createStdOutput (stdout);
	Writer writer (*output);
	context = g_markup_parse_context_new (&parser, (GMarkupParseFlags) 0, 
					      &writer, NULL);

	buffer = NULL;
	error = NULL;
	ret = g_file_get_contents (argv[1], &buffer, &length, &error);
	if (!ret) {
		g_warning (error->message);
		g_error_free (error);
		return 1;
	}

	error = NULL;
	ret = g_markup_parse_context_parse (context, buffer, length, &error);
	if (!ret) {
		g_warning (error->message);
		g_error_free (error);
		return 1;
	}

	delete output;
	g_free (buffer);

	return 0;
}
