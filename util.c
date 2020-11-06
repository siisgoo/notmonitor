#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/statvfs.h>

#include <string.h>
#include <stdarg.h>

#include "util.h"

char buf[1024];

const char *
fmt_human(long int num)
{
	double scaled;
	size_t i, prefixlen;
	const char *prefix[] = { "", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei",
				"Zi", "Yi" };

	prefixlen = LEN(prefix);

	scaled = num;
	for (i = 0; i < prefixlen && scaled >= 1024; i++) {
		scaled /= 1024;
	}

	return bprintf("%.1f %s", scaled, prefix[i]);
}

int
pscanf(const char *path, const char *fmt, ...)
{
	FILE *fp;
	va_list ap;
	int n;

	if (!(fp = fopen(path, "r"))) {
		fprintf(stderr, "fopen: cant open file\n");
		return -1;
	}
	va_start(ap, fmt);
	n = vfscanf(fp, fmt, ap);
	va_end(ap);
	fclose(fp);

	return (n == EOF) ? -1 : n;
}

const char *
bprintf(const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
	ret = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	return (ret < 0) ? NULL : buf;
}

static void
verr(const char *fmt, va_list ap)
{
	vfprintf(stderr, fmt, ap);

	if (fmt[0] && fmt[strlen(fmt) - 1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
}

void
warn(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verr(fmt, ap);
	va_end(ap);
}

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	verr(fmt, ap);
	va_end(ap);

	exit(1);
}

int
getBatPerc(const char* bat)
{
	char perc;
	char path[MAX_PATH];

	if (sprintf(path,
		"/sys/class/power_supply/%s/capacity", bat) < 0) {
		return -1;
	}
	
	pscanf(path, "%d", &perc);

	return (perc > 0) ? perc : -1;
}

int
isChurging(const char *bat)
{
	static struct {
		char *state;
		int symbol;
	} map[] = {
		{ "Charging",    1 },
		{ "Discharging", 0 },
	};
	size_t i;
	char path[MAX_PATH], state[12];

	if (sprintf(path,
			"/sys/class/power_supply/%s/status", bat) < 0) {
		return -1;
	}
	if (pscanf(path, "%12s", state) != 1)
		return -1;

	for (i = 0; i < LEN(map); i++) {
		if (!strcmp(map[i].state, state)) {
			break;
		}
	}
	return (i == LEN(map)) ? -1 : map[i].symbol;
}

unsigned long int
diskPerc(const char *path)
{
	struct statvfs fs;

	if (statvfs(path, &fs) < 0) {
		fprintf(stderr, "Error: statvfs '%s':\n", path);
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
		fprintf(stderr, "Error: statvfs '%s':\n", path);
		return -1;
	}

	return fs.f_frsize * fs.f_bavail;
}
