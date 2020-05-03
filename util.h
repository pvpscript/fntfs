#ifndef UTIL_H
#define UTIL_H

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

int is_directory(const char *path);
int o_strstr(const char *haystack, const char *needle, long *offset); 
char prompt();
void die(char *fmt, ...);

#endif /* UTIL_H */
