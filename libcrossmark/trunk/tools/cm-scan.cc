
#include <iostream>
#include <string>
#include <stdio.h>
#include <typeinfo>
#include <crossmark/cm-scanner.hh>

using namespace crossmark;

int
main (int 	  argc, 
      char 	**argv)
{
	if (argc < 2) {
		perror ("Need filename\n");
		return 1;
	}

	std::string file (argv[1]);
	Scanner scanner (file);

	tokens::Token *token;
	do {
		token = scanner.fetchToken ();
		std::cout << token->serialize ();

	} while (!dynamic_cast<tokens::Eof *> (token));

	return 0;
}
