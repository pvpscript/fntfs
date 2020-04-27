#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	DIR *directory = opendir("./");

	struct dirent *data = readdir(directory);

	printf("File serial number %d\n", data->d_ino);
	printf("Name of entry %s\n", data->d_name);

	return 0;
}
