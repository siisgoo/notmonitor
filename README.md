# NOTMON
System monitor and notifier.

## Content
- Configuration
	- Syntax
- Availible monitors
- In development capabilities

## Configuration
3rd party config file not supported.
All configuration applaing in config.h and after program recompiling.
Inspired by suckless [dwm](https://dwm.suckless.org/)
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

## Availible monitors
+ Battery percentage
+ Disk free space

## In development capabilities
- [ ] Notificators
	- [ ] MPD notifications
- [ ] Functions
	- [ ] Ignoring some notifications on the fly
