/* settings for the edges of the root name */
static const char all_prefix[]      = " ";
static const char all_postfix[]     = " ";

/* settings for each individual block */
static const char block_delimeter[] = " | ";
static const char block_prefix[]    = "<";
static const char block_postfix[]   = ">";

/* extra signals */
static const uint signal_all        = 1;

/* blocks to load */
static const BlockPre blocks_pre[] = {
	/* command, update interval, update signal */
	{"dwmblocks-ram", 5, 0},
	{"dwmblocks-cpu", 10, 0},
	{"dwmblocks-battery", 5, 0},
	{"dwmblocks-volume", 5, 10},
	{"dwmblocks-clock", 5, 0},
};
