#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <setjmp.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

typedef struct {
	char *old_name;
	char *new_name;
} Reserved;

#include "config.h"

enum params {
	INTERACTIVE 	= (1u << 0),
	VERBOSE 	= (1u << 1)
};

static jmp_buf err_buf; /* error handling buffer */

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
	if (!cat)
		longjmp(err_buf, errno);

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

static void replace_substr(char **entry, long *offset,
		const Reserved replace)
{
	size_t entry_len = strlen(*entry);
	size_t oname_len = strlen(replace.old_name);
	size_t nname_len = strlen(replace.new_name);

	if (nname_len - oname_len > 0) {
		*entry = realloc(*entry, sizeof(char) 
				* (entry_len + nname_len - oname_len + 1));
		if (!*entry)
			longjmp(err_buf, errno);
	}

	memcpy(*entry+*offset+nname_len, *entry+*offset+oname_len,
			entry_len-*offset-oname_len+1);
	memcpy(*entry+*offset, replace.new_name, nname_len);
	*offset += nname_len;
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

static void replace_chars(char **dst, char *name)
{
	long offset;
	int i;

	for (i = 0; i < COUNT_OF(r_chars); i++) {
		for (offset = 0; o_strstr(*dst, r_chars[i].old_name, &offset);
		     replace_substr(&(*dst), &offset, r_chars[i]));
	}
}

static void replace_names(char **dst, const char *name)
{
	long offset = 0;
	int i;

	for (i = 0; i < COUNT_OF(r_names); i++) {
		if (o_strstr(name, r_names[i].old_name, &offset) && !offset) {
			*dst = realloc(*dst, (strlen(name)+1) * sizeof(**dst));
			if (!*dst)
				longjmp(err_buf, errno);

			replace_substr(&(*dst), &offset, r_names[i]);
			break;
		}
	}
}

static char *replace_reserved(char *name)
{
	char *new_name = malloc((strlen(name)+1) * sizeof(*new_name));
	if (!new_name)
		longjmp(err_buf, errno);

	strcpy(new_name, name);

	replace_names(&new_name, name);
	replace_chars(&new_name, new_name);

	return new_name;
}

static char prompt()
{
	char c;
	char buf;

	while (scanf(" %c", &c) && c != 'N' && c != 'n' && c != 'Y' && c != 'y')
		;
	while ((buf = getchar()) && buf != '\n' && buf != EOF);

	return tolower(c);
}

static int ren_entry(const char *old, const char *new, unsigned param_mask)
{
	int ret = -1;

	if (param_mask & INTERACTIVE &&
			access(new, F_OK) == 0) {
		printf("fntfs: overwrite '%s'?", new);
		if (prompt() == 'y')
			ret = rename(old, new);
	} else {
		ret = rename(old, new);
	}
	
	if (param_mask & VERBOSE &&
			ret == 0)
		printf("renamed '%s' -> '%s'\n",
				old, new);

	return (ret == 0);
}

static void depth_first(char *path, unsigned param_mask)
{
	struct dirent *data;
	DIR *directory;
	char *full_path;
	char *new_name;
	char *new_path;

	directory = opendir(path);
	if (!directory)
		longjmp(err_buf, errno);

	while ((data = readdir(directory))) {
		if (strcmp(data->d_name, ".") != 0
				&& strcmp(data->d_name, "..") != 0) {
			full_path = cat_path(path, data->d_name);

			if (is_directory(full_path))
				depth_first(full_path, param_mask);
				
			new_name = replace_reserved(data->d_name);
			if (strcmp(new_name, data->d_name) != 0) {
				new_path = cat_path(path, new_name);

				if (!ren_entry(full_path, new_path,
							param_mask))
					longjmp(err_buf, errno);

				free(new_path);
			}

			free(new_name);
			free(full_path);
		}
	}

	free(directory);
}

void die(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{	
	/* Parameters:
	 * 	-v: verbose (explain what is being done). E.g.: fntfs: renamed 'file_1' -> 'file_2'
	 * 	-i: interactive (prompt before rename). E.g.: 
	 */

	unsigned param_mask = 0;
	int opt;
	int i;

	while((opt = getopt(argc, argv, "hiv")) != -1) {
		switch (opt) {
			case 'h':
				printf("Usage: %s [OPTION...] dir1 [dir2 ...]\n\n",
						argv[0]);
				printf("-h (help): show this menu\n");
				printf("-i (interactive): prompt before rename\n");
				printf("-v (verbose): explain what is being done\n");

				exit(EXIT_SUCCESS);
			case 'i':
				param_mask |= INTERACTIVE;
				break;
			case 'v':
				param_mask |= VERBOSE;
				break;
			default: /* '?' */
				die("Usage: %s [OPTION...] dir1 [dir2 ...]\n",
						argv[0]);
		}
	}

	if (optind >= argc)
		die("Missing directory");

	switch(setjmp(err_buf)) {
		case 0:
			for (i = optind; i < argc; i++)
				depth_first(argv[i], param_mask); 
			break;
		case ENOMEM: /* Memory error */
			die("Memory error: %s\n", strerror(ENOMEM));
		case ENOENT: /* Missing entry error */
			die("%s: %s\n", argv[i], strerror(ENOENT));
		case EACCES: /* Access error */
			die("'%s': %s\n", argv[i], strerror(EACCES));
		default:
			die("Unknown error: %s\n", strerror(errno));
	}

	exit(EXIT_SUCCESS);
}
