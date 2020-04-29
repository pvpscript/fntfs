#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
	char *old_name;
	char *new_name;
} Reserved;

#include "config.h"

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

static int replace_substr(char **entry, char **location,
		const Reserved replace)
{
	size_t entry_len = strlen(*entry);
	size_t oname_len = strlen(replace.old_name);
	size_t nname_len = strlen(replace.new_name);
	size_t loc_offset = *location - *entry;

	if (nname_len - oname_len > 0) {
		*entry = realloc(*entry, sizeof(char) 
				* (entry_len + nname_len - oname_len + 1));
		if (*entry == NULL)
			return 0;
	}

	*location += oname_len;

	memcpy(*entry+loc_offset+nname_len, *location, strlen(*location)+1);
	memcpy(*entry+loc_offset, replace.new_name, nname_len);

	*location += (nname_len - oname_len);

	return 1;
}

static void testing()
{
	int i;

/*	puts("Reserved chars");
	for (i = 0; i < COUNT_OF(r_chars); i++)
		printf("%c\n", r_chars[i]);

	puts("Reserved names");
	for (i = 0; i < COUNT_OF(r_names); i++)
		printf("%s\n", r_names[i]);
*/
	printf("%ld\n", COUNT_OF(r_chars));
	printf("%ld\n", COUNT_OF(r_names));
}

static char *replace_chars(char *name)
{
	char *new_name;
	char *tmp;
	int i;

	new_name = malloc(sizeof(char) * (strlen(name) + 1));
	if (new_name == NULL)
		return NULL;
	strcpy(new_name, name);

	for (i = 0; i < COUNT_OF(r_chars); i++) {
		tmp = new_name;
		while((tmp = strstr(tmp, r_chars[i].old_name)) != NULL)
			if (!replace_substr(&new_name, &tmp, r_chars[i]))
				return NULL;
	}

	return new_name;
}

static char *replace_names(char *name)
{
	char *new_name;
	char *offset;
	int i;

	for (i = 0; i < COUNT_OF(r_names); i++) {
		offset = strstr(name, r_names[i].old_name);
		if (offset != NULL	&& offset - name == 0) {
			new_name = malloc((strlen(name) + 1) * sizeof(*new_name));
			if (new_name == NULL)
				return NULL;
			strcpy(new_name, name);

			offset = new_name + (offset - name);
			if (!replace_substr(&new_name, &offset, r_names[i]))
				return NULL;

			return new_name;
		}
	}

	return NULL;
}

static char *depth_first(DIR *directory, char *path)
{
	struct dirent *data;
	char *full_path;
	char *new_name;

	while ((data = readdir(directory)) != NULL) {
		if (strcmp(data->d_name, ".") != 0
				&& strcmp(data->d_name, "..") != 0) {
			full_path = cat_path(path, data->d_name);

/*			printf("File serial number %lu\n", data->d_ino);
			printf("Name of entry \"%s\"\n\n", data->d_name);

			printf("FULLPATH: \"%s\"\n", full_path);*/
	/*		getchar(); */
			if (is_directory(full_path))
				depth_first(opendir(full_path), full_path);
				
			new_name = replace_names(data->d_name);
			new_name = (new_name == NULL)
				? replace_chars(data->d_name)
				: replace_chars(new_name);
			if (new_name != NULL &&
				strcmp(new_name, data->d_name) != 0) {
				printf("%s -> %s\n", data->d_name, new_name);
			}	
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{	
/*	testing();
	exit(0);*/

	DIR *directory = opendir(argv[1]);	

	depth_first(directory, argv[1]);

	return 0;
}
