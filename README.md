# NOTMON
System monitor and notifier.

## Table of content
1. [ Configuration. ](#conf)
	+ [ Syntax. ](#syntax)
2. [ Availible monitors. ](#mons)
3. [ In development capabilities. ](#dev)

<a name="conf"></a>
## Configuration
3rd party config file not supported.
All configuration applaing in config.h and after program recompiling.
Inspired by suckless [dwm](https://dwm.suckless.org/)
<a name="syntax"></a>
### Syntax
**Example:**
```C
static const struct arg notifications[] = {
   /* function	argument */
	{ battery,	"BAT0" },
	{ disk,		"/" },
	{ disk,		"/home },
	...
};
```

<a name="mons"></a>
## Availible monitors
+ Battery percentage
+ Disk free space

<a name="dev"></a>
## In development capabilities
- [ ] Notificators
	- [ ] MPD notifications
- [ ] Functions
	- [ ] Ignoring some notifications on the fly

## TODO
- Unify error hendling
