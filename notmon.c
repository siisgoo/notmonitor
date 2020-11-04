/* this program exist for alarm user */
/* about low battery charge using notifycation daemon */
/* made with love */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

struct arg {
	int (*func)();
	char *args;
};

#include "notmon.h"
#include "battery.h"
#include "disk.h"
#include "util.h"
#include "config.h"

static volatile sig_atomic_t done = 0;
static char notify_path[MAX_PATH]; /* path to notifycator */

int
main()
{
	/* make more beutifull */
	setTermHandler();
	
	/* get path to notifycator program */
	snprintf(notify_path, sizeof(notify_path), "/bin/%s", NOTIFYCATOR);

	while(!done)
	{
		/* threads hendling vars */
		int stat, ret = 0;
		pid_t pid = fork();

		if (pid == 0)
		{
			/* uncomplited!!! */
			unsigned i, len;
			for (i = len = 0; i < LEN(notificator); i++)
			{
				int statc;
				pid_t pidc = fork();
				if (pidc == 0)
				{
					ret += notificator[i].func(notificator[i].args);
					exit(EXIT_SUCCESS);
				}
				if (pidc > 0)
					wait(&statc);
				else
					fprintf(stderr, "EROREWRWER\n");
			}

			/* check for status sended */
			if (ret != 0)
			       exit(EXIT_FAILURE);
			exit(EXIT_SUCCESS);
		}
		else if (pid > 0)
		{
			/* wait for exit child process */
			wait(&stat);
			/* if corectly exited then continue */
			if (WIFEXITED(stat))
				sleep(SYNC_TIME);
			else
			{
				fprintf(stderr, "Error: pid-%d exited with error code %d\n", pid, stat);
				exit(EXIT_FAILURE);
			}
		}
		else {
			fprintf(stderr, "Fattal Error: Cant fork\n");
			return CANT_FORK;
		}
	}

	return 0;
}

/* terminate command */
static void term(){ done = 1; }

static void setTermHandler()
{
	/* set hendler of SIGTERM and SIGINT to term() */
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = term;
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGINT, &action, NULL);
}

int
sendNotify(char **argv, char *msg, int level)
{
	switch(level)
	{
		case L_LOW:
			break;
		case L_NORMAL:
			argv[ARG_MSG] = msg;
			execv(notify_path, argv);
			break;
		case L_CRITICAL:
			argv[ARG_MSG] = msg;
			argv[ARG_LEVEL] = "critical";
			execv(notify_path, argv);
			break;
		default:
			return -1;
	}

	return 0;
}

int
battery()
{
	int perc;
	int ret = 0;

	gArgv[ARG_APP] = "battery";
	if (isChurging(IDENT_BAT) == 0)
	{
		perc = getBatPerc(IDENT_BAT);
		if (perc < B_LOW)
			ret = sendNotify(gArgv, M_LOW, L_NORMAL);
		else if(perc < B_CRIT)
			ret = sendNotify(gArgv, M_CRIT, L_CRITICAL);
	}
	return ret;
}

int
disk(char *fs)
{
	int ret = 0;
	int free_perc;
	/* char msg[MAX_LINE] = DISK_MSG_LOW; */
	/* strcat(msg, fs); */
	char msg[MAX_LINE];
	sprintf(msg, "%s \"%s\". %s left", DISK_MSG_LOW, fs, fmt_human(diskFree(fs)));

	
	gArgv[ARG_APP] = "disk";
	free_perc = (100 - diskPerc(fs));
	if (free_perc < D_LOW)
		ret = sendNotify(gArgv, msg, L_NORMAL);
	else if (free_perc < D_CRIT)
		ret = sendNotify(gArgv, msg, L_CRITICAL);

	return ret;
}
