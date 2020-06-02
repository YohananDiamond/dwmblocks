#define BINBLOCK(bin, interval, signal) {"~/.local/bin/status-dwm/" bin, interval, signal}

/* delimeter string */
static char *block_delim = " | ";
static char *block_prefix = " ";
static char *block_postfix = " ";

/* blocks */
static const BlockPre blocks_pre[] = {
	/* command, update interval, update signal */
	BINBLOCK("swap", 10, 0),
	BINBLOCK("ram", 10, 0),
	BINBLOCK("cpu", 10, 0),
	BINBLOCK("battery", 10, 0),
	BINBLOCK("volume", 10, 0),
	BINBLOCK("clock", 10, 0),
};
