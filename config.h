#define BINBLOCK(bin, interval, signal) {"~/.local/bin/status-dwm/" bin, interval, signal}

/* delimeter string */
static const char *block_delim = " | ";
static const char *block_prefix = " ";
static const char *block_postfix = " ";

/* other options */
static const uint signal_restart = 1;

/* blocks */
static const BlockPre blocks_pre[] = {
	/* command, update interval, update signal */
	/* BINBLOCK("swap", 10, 0), */
	BINBLOCK("ram", 5, 0),
	BINBLOCK("cpu", 10, 0),
	BINBLOCK("battery", 5, 0),
	BINBLOCK("volume", 5, 10),
	BINBLOCK("clock", 5, 0),
};
