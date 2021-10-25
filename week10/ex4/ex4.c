#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define BUFSIZE 200

#define WORKING_DIR "./tmp/"

DIR * opendir_wrap(const char * dirpath) {
	DIR * dirp = opendir(dirpath);
	if(dirp == NULL) {
		fprintf(stderr, "error opening directory %s, check if it exists and has proper read permissions\n", dirpath);
		exit(EXIT_FAILURE);
	}
	return dirp;
}

typedef struct {
	ino_t inode;
	char name[BUFSIZE];
} file_entry;

int main() {
	struct dirent * de;
	struct stat statbuf;
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);
	DIR * dirp = opendir_wrap(WORKING_DIR);
	strncat(buf, WORKING_DIR, BUFSIZE);
	//int n_files = 10, 
	int last_ind = 0;
	file_entry files[50]; //= malloc(sizeof(struct stat) * n_files);
	while((de = readdir(dirp)) != NULL) {
		if(de->d_type != DT_DIR) { 
			strcat(buf, de->d_name);
			stat(buf, &statbuf);
			fprintf(stderr, "INFO: file: %s\tinode:%lu\n", buf, statbuf.st_ino);
			memset(buf, 0, BUFSIZE); strncat(buf, WORKING_DIR, BUFSIZE);
			if (statbuf.st_nlink >= 2) {
				files[last_ind].inode = statbuf.st_ino;
				strncpy(files[last_ind].name, de->d_name, BUFSIZE);
				last_ind++;
			}
		}
	}
	printf("================================\nfile\t\thardlinks\n================================\n");
	for(int i = 0; i < last_ind; i++) {
		printf("%s\t\t%s ", files[i].name, files[i].name);
		for(int j = 0; j < last_ind; j++) {
			if (i==j) continue;
			if (files[j].inode == files[i].inode) {
				printf("%s ", files[j].name);
			}
		}
		printf("\n");
	}
	return 0;
}
