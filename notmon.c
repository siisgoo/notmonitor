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
    unsigned int interval;
    char *args;
};

#include "config.h"

char buf[1024];
static volatile sig_atomic_t done = 0;

/* terminate command */
static void term() { done = 1; }

static struct timespec
now() {
    struct timespec current;
    if (clock_gettime(CLOCK_MONOTONIC, &current) < 0) {
        die("clock_gettime:");
    }
    return current;
}

static void
difftimespec(struct timespec *res, struct timespec *a, struct timespec *b)
{
    res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
    res->tv_nsec = a->tv_nsec - b->tv_nsec +
                   (a->tv_nsec < b->tv_nsec) * 1E9;
}

static struct timespec
timer_get_wait(struct timespec start, long interval)
{
    struct timespec diff, current, wait, intspec;
    current = now();
    difftimespec(&diff, &current, &start);

    intspec.tv_sec = interval / 1000;
    intspec.tv_nsec = (interval % 1000) * 1E6;
    difftimespec(&wait, &intspec, &diff);

    return wait;
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
            ret = sendNotify("battery", bprintf("%s %d%%", batteryMsgLow, perc), 10000, "monitor", NOTIFY_URGENCY_NORMAL);
        } else if(perc < batteryCritValue) {
            ret = sendNotify("battery", bprintf("%s %d%%", batteryMsgCrit, perc), 10000, "monitor", NOTIFY_URGENCY_CRITICAL);
        }
    }

    return ret;
}

static int
disk(char *fs)
{
    int ret = 0;
    int used_perc;
    char msg[MAX_LINE];

    //rewrite this
    used_perc = diskPerc(fs);
    sprintf(msg,
            "%s \"%s\". %s left",
            diskMsgLow, fs, fmt_human(used_perc));

    if (used_perc > diskLowValue) {
        ret = sendNotify("disk", msg, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
    } else if (used_perc > diskCritValue) {
        ret = sendNotify("disk", msg, 10000, "category", NOTIFY_URGENCY_CRITICAL);
    }

    return ret;
}

static int
checkupdates(void)
{
    int ret = 0;
    FILE *fd;
    fd = popen("checkupdates | wc -l", "r");
    if (!fd) return 1;
     
    char   buffer[256];
    size_t chread;
    /* String to store entire command contents in */
    size_t comalloc = 256;
    size_t comlen   = 0;
    char  *comout   = malloc(comalloc);
     
    /* Use fread so binary data is dealt with correctly */
    while ((chread = fread(buffer, 1, sizeof(buffer), fd)) != 0) {
        if (comlen + chread >= comalloc) {
            comalloc *= 2;
            comout = realloc(comout, comalloc);
        }
        memmove(comout + comlen, buffer, chread);
        comlen += chread;
    }

    /* We can now work with the output as we please. Just print
     * out to confirm output is as expected */

    if (atoi(comout) > checkupdatesVarnPkgs) {
        char msg[MAX_LINE];
        sprintf(msg, "%d updates available", atoi(comout));
        ret = sendNotify("System Update", msg, 10000, "monitor", NOTIFY_URGENCY_NORMAL);
    }

    free(comout);
    pclose(fd);

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
    struct timespec start, wait;

    struct timespec notificators_start[LEN(notificator)];
    for (int i = 0; i < LEN(notificator); i++) {
        /* notificators_start[i] = now(); */
        notificators_start[i].tv_sec  = 0;
        notificators_start[i].tv_nsec = 0;
    }

    /* set hendler of SIGTERM and SIGINT to term() */
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);

    while (!done)
    {
        start = now();

        int i;
        /* run monitors */
        for (i = 0; i < LEN(notificator); i++)
        {
            if (timer_get_wait(notificators_start[i], notificator[i].interval).tv_sec <= 0) {
                notificators_start[i] = now();
                notificator[i].func(notificator[i].args);
            }
        }
            
        notify_uninit();

        /* sleep */
        if (!done) {
            wait = timer_get_wait(start, g_interval);

            if (wait.tv_sec >= 0) {
                if (nanosleep(&wait, NULL) < 0 &&
                    errno != EINTR) {
                    die("nanosleep:");
                }
            }
        }
    }

    return 0;
}
