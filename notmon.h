#define MAX_LINE 1000

/* messeges per level */
static const char *batteryMsgLow  = "Low battery charge";
static const char *batteryMsgCrit = "Critical battery charge";

static const char *diskMsgLow  = "Disk space remaining";
static const char *diskMsgCrit = "Disk space remaining";
/**********************/

/* time */
static struct timespec now();
static void difftimespec(struct timespec *res, struct timespec *a, struct timespec *b);
static struct timespec timer_get_wait(struct timespec start, long interval);

/* battery handling */
int getBatPerc(const char *bat);
int isChurging(const char *bat);

/* disk handling */
unsigned long int diskPerc(const char *path);
unsigned long int diskFree(const char *path);

static void term();
static int sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency);

/* monitors */
static int battery(const char *bat);
static int disk(char *fs);
static int checkupdates(void);
