#define MAX_LINE 1000

void term();
int battery();
int disk(char *fs);
int sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency);
