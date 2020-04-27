#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_directory(const char *path)
{
	struct stat buf;
	if (stat(path, &buf) != 0)
		return 0;
	return S_ISDIR(buf.st_mode);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		exit(1);

	DIR *directory = opendir(argv[1]);
	if (directory == NULL) {
		fprintf(stderr, "Couldn't open directory\n");
		exit(1);
	}

	printf("Current location of dir stream: %d\n", telldir(directory));

	struct dirent *data;
	int files = 0;
	char full_path[200];
	char new_name[200];

	while ((data = readdir(directory)) != NULL) {
		strcat(full_path, argv[1]);
		strcat(full_path, data->d_name);

		printf("File serial number %d\n", data->d_ino);
		printf("Name of entry \"%s\" -> (dir: %s)\n\n",
				data->d_name,
				(is_directory(full_path))
				 ? "True" : "False");

		if (strcmp(data->d_name, "testing") == 0) {
			new_name[0] = 0;
			strcat(new_name, argv[1]);
			strcat(new_name, "file_renamed_by_c_code");
			rename(full_path, new_name);
		}

		if (strcmp(data->d_name, "dir_test") == 0) {
			new_name[0] = 0;
			strcat(new_name, argv[1]);
			strcat(new_name, "dir_renamed_by_c_code");
			rename(full_path, new_name);
		}

		files++;
		full_path[0] = 0;
	}

	printf("\nTotal files: %d\n", files);

	return 0;
}
