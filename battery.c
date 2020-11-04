#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "battery.h"
#include "util.h"

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
