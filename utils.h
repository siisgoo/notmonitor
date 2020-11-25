#ifndef  _UTIL_H_
#define _UTIL_H_

#define LEN(x) (sizeof (x) / sizeof *(x))

#define MAX_PATH 64

const char *fmt_human(long int n);

const char *bprintf(const char *fmt, ...);
int pscanf(const char *path, const char *fmt, ...);

static void verr(const char *fmt, va_list ap);
void warn(const char *fmt, ...);
void die(const char *fmt, ...);

#endif
