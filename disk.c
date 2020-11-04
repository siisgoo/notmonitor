#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/types.h>

#include "disk.h"

unsigned long int
diskPerc(const char *path)
{
	struct statvfs fs;

	if (statvfs(path, &fs) < 0) {
		fprintf(stderr, "Error: statvfs '%s':", path);
		return -1;
	}

	/* returns fs size in KB */
	return (unsigned long int)(100 * (1.0f - ((float)fs.f_bavail / (float)fs.f_blocks)));

}

unsigned long int
diskFree(const char *path)
{
	struct statvfs fs;

	if (statvfs(path, &fs) < 0) {
		fprintf(stderr, "Error: statvfs '%s':", path);
		return -1;
	}

	return fs.f_frsize * fs.f_bavail;
}

