#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(1);

	DIR *directory = opendir(argv[1]);
	if (directory == NULL) {
		fprintf(stderr, "Couldn't open directory\n");
		exit(1);
	}

	struct dirent *data;
	int files = 0;

	while ((data = readdir(directory)) != NULL) {
		printf("File serial number %d\n", data->d_ino);
		printf("Name of entry \"%s\"\n\n", data->d_name);

		files++;
	}

	printf("\nTotal files: %d\n", files);

	return 0;
}
