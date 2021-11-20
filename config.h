#define APP "notmon" /* app that will be displaied in notification msg */
#define HELLO_MSG "You'r welcome to " APP

/* disk */
/* disk free space in perc to alarm */
static const int diskLowValue  = 95;
static const int diskCritValue = 99;

/* battery */
/* battery low and critical values for warning */
static const int batteryLowValue  = 30;
static const int batteryCritValue = 20;

/* checkupdates */
/* number of packages with available update to notify */
static const int checkupdatesVarnPkgs = 40;

/* sync time in ms */
static const unsigned int g_interval = 60000;

/***********************************
 * func				     argument
 * battery			     battery name
 * disk				     mount poing path
 * checkupdates          NULL
 * checkupdates_aur      NULL
 ***********************************/
static const struct arg notificator[] = {
   /* function | update frequency(ms) |  argument */
    { battery,	                   0,   "BAT0" },
    { disk,		                   0,    "/" },
    { checkupdates,           360000,    NULL },
};
