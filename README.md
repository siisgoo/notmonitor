# NOTMON
System monitor and notifier.

## Table of content
1. [ Configuration. ](#conf)
	+ [ Syntax. ](#syntax)
2. [ Availible monitors. ](#mons)
3. [ In development capabilities. ](#dev)

<a name="conf"></a>
## Configuration
All configuration applaing in config.h((suckless)[suckless.org] like configuration)
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
+ Archlinux pacman updates check

<a name="dev"></a>
## In development capabilities
- [ ] Functions
	- [ ] Ignoring some notifications on the fly
