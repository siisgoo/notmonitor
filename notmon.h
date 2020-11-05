#define MAX_LINE 1000

/* notifycation urgency level */
#define L_LOW      0
#define L_NORMAL   1
#define L_CRITICAL 2

/* exit code for hendling fork() failure */
#define CANT_FORK 2

/* #define MAX_ARG 9 /1* args_amount+1 *1/ */

void term();
int sendNotify(char **argv, char *msg, int level);
int battery();
int disk(char *fs);
