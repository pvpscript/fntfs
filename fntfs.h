#ifndef FRIENDLY_NTFS_H
#define FRIENDLY_NTFS_H

#define MAX_PATH 260

char r_chars[] = {'<', '>', ':', '"', '/', '\\', '|', '?', '*'};
char *r_names[] = {"CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4",
     "COM5", "COM6", "COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4",
     "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"};


/* Check wether the given path is a directory */
int is_directory(const char *path);

#endif /* FRIENDLY_NTFS_H */
