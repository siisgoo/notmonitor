#define APP "notmon" /* app that will be displaied in notification msg */
#define HELLO_MSG "You'r welcome to " APP


/* disk */
/* disk free space in perc to alarm */
#define D_LOW  100
#define D_CRIT 1

#define DISK_MSG_LOW  "Running out of disk space "
#define DISK_MSG_CRIT DISK_MSG_LOW

/* battery */
/* battery low and critical values for warning */
#define B_LOW  100
#define B_CRIT 15

/* messeges per level */
#define M_LOW "Low battery charge"
#define M_CRIT "Critical battery charge"

/* sync time */
#define SYNC_TIME 5

static const struct arg notificator[] = {
	/* monitor	argument */
	{ battery,	NULL },
	{ disk,		"/"  },
	{ disk,		"/home" },
	{ disk,		"/tmp" },
};
