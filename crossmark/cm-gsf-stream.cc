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

#include <gsf/gsf-input-stdio.h>
#include <gsf/gsf-output-stdio.h>
#include "cm-gsf-stream-private.hh"
#include "cm-stream-private.hh"

using namespace crossmark;



/*!
 * Create input from a GsfInput.
 */
stream::Input * 
stream::createGsfInput (::GsfInput *input)
{
	return new stream::GsfInput (input);
}

/*!
 * Create output from a GsfOutput.
 */
stream::Output * 
stream::createGsfOutput (::GsfOutput *output)
{
	return new stream::GsfOutput (output);
}



/*!
 * Create input from file.
 *
 * \todo Use exceptions.
 */
stream::GsfInput::GsfInput (const std::string &file)
{
	GError	 *error;

	error = NULL;
	_input = gsf_input_stdio_new (file.c_str (), &error);
	if (error) {
		g_warning (error->message);
		g_error_free (error);
		return;
	}

	g_assert (_input);
}

/*!
 * Create input from gsf input.
 *
 * \todo Use exceptions.
 */
stream::GsfInput::GsfInput (::GsfInput *input)
  : _input (input)
{
	g_assert (_input);
	g_object_ref (G_OBJECT (input));
}

/*!
 * Dtor.
 */
stream::GsfInput::~GsfInput ()
{
	if (_input) {
		g_object_unref (G_OBJECT (_input));
	}
}

/*!
 * \sa stream::Input::getChar()
 * \todo Use g_utf8_get_char_validated() ?
 * \todo This needs a gsf-savvy hand and some error handling.
 */
gunichar
stream::GsfInput::getChar ()
{
	guint8        buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	int 	      mask;
	int           len;

	g_assert (_input);

	if (gsf_input_eof (_input)) {
		return (unsigned) EOF;
	}

	gsf_input_read (_input, 1, buf);
	mask = 0;
	UTF8_COMPUTE (buf[0], mask, len);
	if (len > 1) {
		// read remaining bytes
		gsf_input_read (_input, len - 1, buf + 1);
	}
	
	return g_utf8_get_char ((gchar const *) buf);
}



/*!
 * Create output to file.
 *
 * \todo Use exceptions.
 */
stream::GsfOutput::GsfOutput (const std::string &file)
{
	GError	 *error;

	error = NULL;
	_output = gsf_output_stdio_new (file.c_str (), &error);
	if (error) {
		g_warning (error->message);
		g_error_free (error);
		return;
	}

	g_assert (_output);
}

/*!
 * Create output to gsf output.
 *
 * \todo Use exceptions.
 */
stream::GsfOutput::GsfOutput (::GsfOutput *output)
  : _output (output)
{
	g_assert (_output);
	g_object_ref (G_OBJECT (_output));
}

/*!
 * Dtor.
 */
stream::GsfOutput::~GsfOutput ()
{
	if (_output) {
		g_object_unref (G_OBJECT (_output));
	}
}

/*!
 * \sa stream::Output::write(gunichar)
 */
gboolean 
stream::GsfOutput::write (gunichar c)
{
	gchar buf[] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	gint  n_bytes;

	n_bytes = g_unichar_to_utf8 (c, buf);
	if (n_bytes) {
		return gsf_output_puts (_output, buf);
	}
	return FALSE;
}

/*!
 * \sa stream::Output::write(gchar const *)
 */
gboolean 
stream::GsfOutput::write (gchar const *s)
{
	return gsf_output_puts (_output, s);
}
