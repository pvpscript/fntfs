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

static int replace_substr(char **entry, long *offset,
		const Reserved replace)
{
	size_t entry_len = strlen(*entry);
	size_t oname_len = strlen(replace.old_name);
	size_t nname_len = strlen(replace.new_name);

	if (nname_len - oname_len > 0) {
		*entry = realloc(*entry, sizeof(char) 
				* (entry_len + nname_len - oname_len + 1));
		if (!*entry)
			return 0;
	}

	memcpy(*entry+*offset+nname_len, *entry+*offset+oname_len,
			entry_len-*offset-oname_len+1);
	memcpy(*entry+*offset, replace.new_name, nname_len);
	*offset += nname_len;

	return 1;
}

static int o_strstr(const char *haystack, const char *needle, long *offset)
{
    long h_offset = 0;
    const char *h;
    const char *n;
    
    for (haystack += *offset; *haystack; haystack++, h_offset++) {
        for (h = haystack, n = needle; *h && *n && *h == *n; h++, n++);
        if (!*n) {
            *offset += h_offset;
            return 1;
        }
    }
    
    return 0;
}

static int replace_chars(char **dst, char *name)
{
	long offset;
	int i;

/*	new_name = malloc(sizeof(char) * (strlen(name) + 1));
	if (!new_name)
		return NULL;
	strcpy(new_name, name); */

	for (i = 0; i < COUNT_OF(r_chars); i++) {
		offset = 0;
		while(o_strstr(*dst, r_chars[i].old_name, &offset)) {
			if (!replace_substr(&(*dst), &offset, r_chars[i]))
				return 0;
		}
	}

	return 1;
}

static int replace_names(char **dst, const char *name)
{
	long offset = 0;
	int i;

	for (i = 0; i < COUNT_OF(r_names); i++) {
		if (o_strstr(name, r_names[i].old_name, &offset) && !offset) {
			*dst = realloc(*dst, (strlen(name)+1) * sizeof(**dst));
			if (!*dst)
				return 0;

			if (!replace_substr(&(*dst), &offset, r_names[i]))
				return 0;

			return 1;
		}
	}

	return 0;
}

static char *replace_reserved(char *name)
{
	char *new_name = malloc((strlen(name)+1) * sizeof(*new_name));
	if (!new_name)
		return NULL;

	strcpy(new_name, name);

	/* TODO: Error control */
	replace_names(&new_name, name);
	replace_chars(&new_name, new_name);

	return new_name;
}

static void depth_first(DIR *directory, char *path)
{
	struct dirent *data;
	char *full_path;
	char *new_name = NULL;

	while ((data = readdir(directory))) {
		if (strcmp(data->d_name, ".") != 0
				&& strcmp(data->d_name, "..") != 0) {
			full_path = cat_path(path, data->d_name);

			if (is_directory(full_path))
				depth_first(opendir(full_path), full_path);
				
			new_name = replace_reserved(data->d_name);
			if (new_name && strcmp(new_name, data->d_name) != 0) {
				printf("%s -> %s\n", data->d_name, new_name);
			}

			free(new_name);
			free(full_path);
		}
	}

	free(directory);
}

int main(int argc, char **argv)
{	
/*	testing();
	exit(0);*/

	/* TODO: parameters.
	 * 	-v: verbose (explain what is being done). E.g.: fntfs: renamed 'file_1' -> 'file_2'
	 * 	-i: interactive (prompt before rename). E.g.: 
	 */

	DIR *directory = opendir(argv[1]);	

	depth_first(directory, argv[1]);

	return 0;
}
