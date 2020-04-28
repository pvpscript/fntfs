#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fntfs.h"
#include "config.h"

typedef struct {
	char *old_name;
	char *new_name;
} Reserved;

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

void testing()
{
	int i;

/*	puts("Reserved chars");
	for (i = 0; i < COUNT_OF(r_chars); i++)
		printf("%c\n", r_chars[i]);

	puts("Reserved names");
	for (i = 0; i < COUNT_OF(r_names); i++)
		printf("%s\n", r_names[i]);
*/
	printf(COUNT_OF(r_chars));
	printf(COUNT_OF(r_names));
}

char *depth_first(DIR *directory, char *path)
{
	struct dirent *data;
	char *full_path;

	while ((data = readdir(directory)) != NULL) {
		if (strcmp(data->d_name, ".") != 0
				&& strcmp(data->d_name, "..") != 0) {
			full_path = cat_path(path, data->d_name);

			printf("File serial number %lu\n", data->d_ino);
			printf("Name of entry \"%s\"\n\n", data->d_name);

			printf("FULLPATH: \"%s\"\n", full_path);
	/*		getchar(); */
			if (is_directory(full_path))
				depth_first(opendir(full_path), full_path);

			
		}
	}

	return NULL;
}
