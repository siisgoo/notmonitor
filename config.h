#define HELLO_MSG "You'r welcome to " APP

#define NOTIFYCATOR "notify-send" /* notifycation program */
#define APP "notmon"		  /* app that will be displaied in notification msg */
/* #define TIMEOUT (10 * 1000)       /1* expire time of msg *1/ */

#define MAX_ARG   9 /* args amount+1 */
#define ARG_MSG   (MAX_ARG-2)
#define ARG_LEVEL (ARG_MSG-3)
#define ARG_APP   (ARG_LEVEL-2)

/* low and critical values for warning */
#define B_LOW  25
#define B_CRIT 15

/* disk */
/* disk free space in perc to alarm */
#define D_LOW  5
#define D_CRIT 1

#define DISK_MSG_LOW  "Running out of disk space "
#define DISK_MSG_CRIT DISK_MSG_LOW

/* battery */
/* messeges per level */
#define M_LOW "Low battery charge"
#define M_CRIT "Critical battery charge"

/* sync time */
#define SYNC_TIME 5

/* notifycator arguments */
char *gArgv[MAX_ARG] = { NOTIFYCATOR,
						"-a", APP,
						"-u", "normal",
						"-t", "10000", /* (seconds * 1000) */
						HELLO_MSG };


static const struct arg notificator[] = {
	/* monitor	argument */
	{ battery,	NULL },
	{ disk,		"/"  },
	{ disk,		"/home" },
	{ disk,		"/tmp" },
};
