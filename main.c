#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "fntfs.h"
#include "main.h"

int main(int argc, char **argv)
{	
	testing();
	exit(0);

	DIR *directory = opendir(argv[1]);	

	depth_first(directory, argv[1]);

	return 0;
}
