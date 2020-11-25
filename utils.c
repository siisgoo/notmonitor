#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/statvfs.h>

#include <string.h>
#include <stdarg.h>

#include "utils.h"

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

