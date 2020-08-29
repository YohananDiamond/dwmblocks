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
	{"echo Hello, World!", 5, 0},
};
