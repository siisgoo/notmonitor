#define MAX_LINE 1000

#define LEN(x) (sizeof (x) / sizeof *(x))

#define MAX_PATH 64

/* battery handling */
int getBatPerc(const char *bat);
int isChurging(const char *bat);

/* disk handling */
unsigned long int diskPerc(const char *path);
unsigned long int diskFree(const char *path);

static void term();
static int sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency);

/* monitors */
static int battery();
static int disk(char *fs);
