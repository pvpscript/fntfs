#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fntfs.h"
#include "defs.h"

static char *cat_path(char *first, char *final)
{
	size_t first_len = strlen(first);
	size_t final_len = strlen(final);
	char *cat;

	if (first[first_len-1] == '/')
		first[first_len-1] = 0;
	else
		first_len++;

	cat = malloc(sizeof(char) * first_len + final_len + 1);
	sprintf(cat, "%s/%s", first, final);

	return cat;
}

static int is_directory(const char *path)
{
	struct stat buf;

	if (path == NULL)
		return 0;
	if (stat(path, &buf) != 0)
		return 0;
	return S_ISDIR(buf.st_mode);
}

char *depth_first(DIR *directory, char *path)
{
	struct dirent *data;
	char *full_path;

	while ((data = readdir(directory)) != NULL) {
		full_path = (strcmp(data->d_name, ".") == 0 
				|| strcmp(data->d_name, "..") == 0)
			? NULL
			: cat_path(path, data->d_name);

		printf("File serial number %lu\n", data->d_ino);
		printf("Name of entry \"%s\"\n\n", data->d_name);

		printf("FULLPATH: \"%s\"\n", full_path);
/*		getchar(); */
		if (is_directory(full_path))
			depth_first(opendir(full_path), full_path);
	}

	return NULL;
}
