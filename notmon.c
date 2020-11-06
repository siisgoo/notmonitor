/* this program exist for alarm user */
/* about low battery charge using notifycation daemon */
/* made with love */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <libnotify/notify.h>

struct arg {
	int (*func)();
	char *args;
};

#include "notmon.h"
#include "util.h"
#include "config.h"

static volatile sig_atomic_t done = 0;

int
battery()
{
	int perc;
	int ret = 0;

	if (isChurging(bat) == 0)
	{
		perc = getBatPerc(bat);
		if (perc < B_LOW) {
			ret = sendNotify("battery", BATTERY_MSG_LOW, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
		} else if(perc < B_CRIT) {
			ret = sendNotify("battery", BATTERY_MSG_CRIT, 10000, "monitor", NOTIFY_URGENCY_CRITICAL);
		}
	}

	return ret;
}

int
disk(char *fs)
{
	int ret = 0;
	int free_perc;
	char msg[MAX_LINE];
	sprintf(msg, "%s \"%s\". %s left", DISK_MSG_LOW, fs, fmt_human(diskFree(fs)));

	free_perc = (100 - diskPerc(fs));
	if (free_perc < D_LOW) {
		ret = sendNotify("disk", msg, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
	} else if (free_perc < D_CRIT) {
		ret = sendNotify("disk", msg, 10000, "category", NOTIFY_URGENCY_CRITICAL);
	}

	return ret;
}

int sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency)
{
	if (!notify_init(APP)) {
		die("Notify init:");
	}

	NotifyNotification *notification = notify_notification_new(appName, msg, NULL);
	notify_notification_set_timeout(notification, timeout);
	notify_notification_set_category(notification, category);
	notify_notification_set_urgency(notification, urgency);

	if (!notify_notification_show(notification, NULL))
	{
		fprintf(stderr, "Cant show notification:");
		return 1;
	}

	return 0;
}

int
main()
{
	/* set hendler of SIGTERM and SIGINT to term() */
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGINT, &action, NULL);

	while(!done)
	{
		unsigned i, len;
		for (i = len = 0; i < LEN(notificator); i++)
		{
			notificator[i].func(notificator[i].args);
		}
			
		notify_uninit();
		sleep(SYNC_TIME);
	}

	return 0;
}

/* terminate command */
void term() { done = 1; }
