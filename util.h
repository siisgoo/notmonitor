#ifndef  _UTIL_H_
#define _UTIL_H_

#define LEN(x) (sizeof (x) / sizeof *(x))

const char *fmt_human(long int n);
const char *bprintf(const char *fmt, ...);
int pscanf(const char *path, const char *fmt, ...);

#endif
