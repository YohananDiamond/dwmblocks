// modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/* Icon, Command, Update Interval, Update Signal */
	/* {"", "cat ~/.pacupdate | sed /📦0/d", */
            /* 0, 9}, */
	
	// {"☀", "xbacklight | sed 's/\\..*//'",
	//    0, 11},

	/* {"🌡", "sensors | awk '/^temp1:/{print $2}'", */
	/*     5, 0}, */

	{"mem ", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g",
            30, 0},

	{"", "~/.local/bin/status-dwm/battery",
	    10, 0},

	{"", "~/.local/bin/status-dwm/volume",
	    0, 10},

	{"", "~/.local/bin/status-dwm/clock",
	    30, 0},
};

// sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char *delim = " | ";
