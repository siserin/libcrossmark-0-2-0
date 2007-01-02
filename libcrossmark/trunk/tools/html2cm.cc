
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

	writer->popBlock ();
}

static void
text (GMarkupParseContext *context,
      const gchar         *text,
      gsize                length,  
      gpointer             writer_,
      GError             **error)
{
	Writer *writer = static_cast<Writer *> (writer_);

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

	Writer writer (*stream::createStdOutput (stdout));
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

	g_free (buffer);

	return 0;
}
