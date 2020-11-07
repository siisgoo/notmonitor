#define MAX_LINE 1000

static void term();
static int battery();
static int disk(char *fs);
static int sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency);
