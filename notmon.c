/* this program exist for alarm user */
/* about low battery charge using notifycation daemon */
/* made with love */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <libnotify/notify.h>

#include "notmon.h"
#include "utils.h"

struct arg {
	int (*func)();
	char *args;
};

#include "config.h"

static volatile sig_atomic_t done = 0;

/* terminate command */
static void term() { done = 1; }

static void
difftimespec(struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
	res->tv_nsec = a->tv_nsec - b->tv_nsec +
	               (a->tv_nsec < b->tv_nsec) * 1E9;
}

int
getBatPerc(const char* bat)
{
	char perc;
	char path[MAX_PATH];

	if (sprintf(path,
		"/sys/class/power_supply/%s/capacity", bat) < 0) {
		return -1;
	}
	
	pscanf(path, "%d", &perc);

	return (perc > 0) ? perc : -1;
}

int
isChurging(const char *bat)
{
	static struct {
		char *state;
		int symbol;
	} map[] = {
		{ "Charging",    1 },
		{ "Discharging", 0 },
	};
	size_t i;
	char path[MAX_PATH], state[12];

	if (sprintf(path,
			"/sys/class/power_supply/%s/status", bat) < 0) {
		return -1;
	}
	if (pscanf(path, "%12s", state) != 1)
		return -1;

	for (i = 0; i < LEN(map); i++) {
		if (!strcmp(map[i].state, state)) {
			break;
		}
	}
	return (i == LEN(map)) ? -1 : map[i].symbol;
}

unsigned long int
diskPerc(const char *path)
{
	struct statvfs fs;

	if (statvfs(path, &fs) < 0) {
		fprintf(stderr, "Error: statvfs '%s':\n", path);
		return -1;
	}

	/* returns fs size in KB */
	return (unsigned long int)(100 * (1.0f - ((float)fs.f_bavail / (float)fs.f_blocks)));

}

unsigned long int
diskFree(const char *path)
{
	struct statvfs fs;

	if (statvfs(path, &fs) < 0) {
		fprintf(stderr, "Error: statvfs '%s':\n", path);
		return -1;
	}

	return fs.f_frsize * fs.f_bavail;
}

static int
battery(const char* bat)
{
	int perc;
	int ret = 0;

	if (isChurging(bat) == 0)
	{
		perc = getBatPerc(bat);
		if (perc < batteryLowValue) {
			ret = sendNotify("battery", BATTERY_MSG_LOW, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
		} else if(perc < batteryCritValue) {
			ret = sendNotify("battery", BATTERY_MSG_CRIT, 10000, "monitor", NOTIFY_URGENCY_CRITICAL);
		}
	}

	return ret;
}

static int
disk(char *fs)
{
	int ret = 0;
	int free_perc;
	char msg[MAX_LINE];
	sprintf(msg, "%s \"%s\". %s left", DISK_MSG_LOW, fs, fmt_human(diskFree(fs)));

	free_perc = (100 - diskPerc(fs));
	if (free_perc < diskLowValue) {
		ret = sendNotify("disk", msg, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
	} else if (free_perc < diskCritValue) {
		ret = sendNotify("disk", msg, 10000, "category", NOTIFY_URGENCY_CRITICAL);
	}

	return ret;
}

static int
sendNotify(const char *appName, const char *msg, gint timeout, const char *category, NotifyUrgency urgency)
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
main(void)
{
	struct timespec start, current, diff, intspec, wait;

	/* set hendler of SIGTERM and SIGINT to term() */
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGINT, &action, NULL);

	while (!done)
	{
			if (clock_gettime(CLOCK_MONOTONIC, &start) < 0) {
				die("clock_gettime:");
			}

			unsigned i, len;
			/* run monitors */
			for (i = len = 0; i < LEN(notificator); i++)
			{
				notificator[i].func(notificator[i].args);
			}
				
			notify_uninit();

			if (!done) {
				if (clock_gettime(CLOCK_MONOTONIC, &current) < 0) {
					die("clock_gettime:");
				}
				difftimespec(&diff, &current, &start);

				intspec.tv_sec = interval / 1000;
				intspec.tv_nsec = (interval % 1000) * 1E6;
				difftimespec(&wait, &intspec, &diff);

				if (wait.tv_sec >= 0) {
					if (nanosleep(&wait, NULL) < 0 &&
						errno != EINTR) {
						die("nanosleep:");
					}
				}
			}
	}


}
