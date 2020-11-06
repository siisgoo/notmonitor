#ifndef  _UTIL_H_
#define _UTIL_H_

#define LEN(x) (sizeof (x) / sizeof *(x))

#define MAX_PATH 64
#define IDENT_BAT "BAT0"

const char *fmt_human(long int n);

const char *bprintf(const char *fmt, ...);
int pscanf(const char *path, const char *fmt, ...);

static void verr(const char *fmt, va_list ap);
void warn(const char *fmt, ...);
void die(const char *fmt, ...);

/* battery handling */
int getBatPerc(const char *bat);
int isChurging(const char *bat);

/* disk handling */
unsigned long int diskPerc(const char *path);
unsigned long int diskFree(const char *path);

#endif
