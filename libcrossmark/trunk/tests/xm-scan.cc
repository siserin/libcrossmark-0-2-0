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

#include <iostream>
#include <string>
#include <stdio.h>
#include <crossmark/scanner-private.hh>

using namespace crossmark;

int
main (int 	  argc, 
      char 	**argv)
{
	if (argc < 2) {
		std::cerr << "Need filename\n" << std::endl;
		return 1;
	}

	Scanner scanner (argv[1]);

	token::Token *token = NULL;
	do {
		if (token)
			delete token;
		token = scanner.fetchToken ();
		std::cout << token->toHtml ();

	} while (!dynamic_cast<token::End *> (token));
	delete token;

	return 0;
}
