/* delimeter string */
static const char *block_delim = "|";
static const char *block_prefix = " ";
static const char *block_postfix = " ";

/* other options */
static const uint signal_restart = 1;

/* blocks */
static const BlockPre blocks_pre[] = {
	/* command, update interval, update signal */
	{"echo Hello, World!", 0, 0},
};
