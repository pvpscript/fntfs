#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

int is_directory(const char *path)
{
	struct stat buf;

	if (path == NULL)
		return 0;
	if (stat(path, &buf) != 0)
		return 0;
	return S_ISDIR(buf.st_mode);
}

int o_strstr(const char *haystack, const char *needle, long *offset)
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

char prompt()
{
	char c;
	char buf;

	while (scanf(" %c", &c) && c != 'N' && c != 'n' && c != 'Y' && c != 'y')
		;
	while ((buf = getchar()) && buf != '\n' && buf != EOF);

	return tolower(c);
}

void die(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	vfprintf(stderr, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

