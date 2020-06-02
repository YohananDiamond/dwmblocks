/* blocks to load */
static const Block blocks[] = {
	/* Icon, Command, Update Interval, Update Signal */
	// {"", "cat ~/.pacupdate | sed /📦0/d", 0, 9},
	// {"☀", "xbacklight | sed 's/\\..*//'", 0, 11},
	// {"🌡", "sensors | awk '/^temp1:/{print $2}'" 5, 0},
	// {"mem ", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0},

	{"", "~/.local/bin/status-dwm/swap", 10, 0},
	{"", "~/.local/bin/status-dwm/ram", 10, 0},
	{"", "~/.local/bin/status-dwm/cpu", 10, 0},
	{"", "~/.local/bin/status-dwm/battery", 10, 0},
	{"", "~/.local/bin/status-dwm/volume", 30, 10},
	{"", "~/.local/bin/status-dwm/clock", 10, 0},
};

/* delimeters, prefixes and postfixes */
static char *delim = " | ";
static char *prefix = " ";
static char *postfix = " ";
