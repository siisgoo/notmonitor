#define APP "notmon" /* app that will be displaied in notification msg */
#define HELLO_MSG "You'r welcome to " APP

/* disk */
/* disk free space in perc to alarm */
static const signed int diskLowValue = 100;
static const signed int diskCritValue = 1;

#define DISK_MSG_LOW  "Running out of disk space "
#define DISK_MSG_CRIT DISK_MSG_LOW

/* battery */
/* battery low and critical values for warning */
static const signed int batteryLowValue = 100;
static const signed int batteryCritValue = 1;

/* messeges per level */
#define BATTERY_MSG_LOW "Low battery charge"
#define BATTERY_MSG_CRIT "Critical battery charge"

/* sync time */
static const unsigned int interval = 10000;

static const char *bat = "BAT0";

static const struct arg notificator[] = {
	/* monitor	argument */
	{ battery,	NULL },
	{ disk,		"/"  },
	{ disk,		"/home" },
	{ disk,		"/tmp" },
};
