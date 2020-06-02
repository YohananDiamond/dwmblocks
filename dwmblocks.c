/**
 * This program is not tested on OpenBSD, but I think the macros I moved still work. I hope.
 * TODO: make a man page
 * TODO: replace all int i = 0 with uint, I guess
 * TODO: a signal to refresh the entire bar
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <X11/Xlib.h>

#define LENGTH(X) (sizeof (X) / sizeof (X[0]))
#define STR_EQ(str1, str2) !strcmp((str1), (str2))
#define OUTPUT_SIZE 64
#define STATUS_SIZE 256

/* types */
typedef struct {
	char *command;
	uint interval;
	uint signal;
} BlockPre;
typedef struct {
	char output[OUTPUT_SIZE];
	char *command;
	uint interval;
	uint signal;
} Block;
typedef enum {
	BLOCK_FIRST = 0,
	BLOCK_NORMAL = 1,
	BLOCK_LAST = 2,
} EBlockOrder;

#include "config.h"

/* internal functions */
void main(int argc, char **argv);
void blocks_update(int time);
void block_getcmd(Block *block, EBlockOrder order);
EBlockOrder block_order(uint pos, uint length);
void find_signal_command(int signal);
void get_status(char *str, uint maxsize);
void handler_signal(int signum);
void handler_term(int signal);
int strcat_safe(char *string_mod, const char *string_addto, const uint maxsize);
void str_remove_char(char *str, char c);
void update_status(void);
void write_setroot(char *str);
void write_stdout(char *str);

/* globals */
static void (*write_fn)(char *str) = write_setroot;
static int status_continue = 1;
static uint blocks_len;
static Block *blocks;
static char status_str[STATUS_SIZE];
static char status_str_old[STATUS_SIZE];

void main(int argc, char **argv) {
	int timer = 0;

	for (int i = 1; i < argc; i++) {
		if (STR_EQ("-h", argv[i]) || STR_EQ("--help", argv[i])) {
			fprintf(stderr, "Usage: %s [{ -h | --help }]\n", argv[0]);
			fprintf(stderr, "There are no arguments for now.\n");
		} else {
			fprintf(stderr, "Invalid argument: %s\n", argv[i]);
			exit(1);
		}
	}

	/* convert blocks_pre into blocks */
	blocks_len = LENGTH(blocks_pre);
	blocks = malloc((sizeof *blocks) * blocks_len);
	for (int i = 0; i < blocks_len; i++) {
		const BlockPre *old = &blocks_pre[i];
		Block *new = &blocks[i];

		new->output[0] = '\0'; /* to assert the string length is 0 if there's junk memory there (idk if this is correct) */
		new->command = old->command;
		new->interval = old->interval;
		new->signal = old->signal;
	}

	/* set up standard signals */
	signal(SIGTERM, handler_term);
	signal(SIGINT, handler_term);

#ifndef __OpenBSD__
	/* set up custom signals */
	for (int i = 0; i < blocks_len; i++) {
		Block *current = &blocks[i];
		int sig = current->signal;
		if (sig > 0) {
			signal(SIGRTMIN + sig, handler_signal);
		}
	}
#endif

	blocks_update(-1);
	while (status_continue) {
		blocks_update(timer);

		/* put old string to status_str_old */
		status_str_old[0] = '\0';
		strcat_safe(status_str_old, status_str, STATUS_SIZE);

		update_status();
		sleep(1.0);
		timer++;
	}
}

void blocks_update(int time) {
	for (int i = 0; i < blocks_len; i++) {
		Block *current = &blocks[i];
		if ((current->interval != 0 && time % current->interval == 0)
				|| (time == -1)) {
			EBlockOrder order = block_order(i, blocks_len);
			block_getcmd(current, order);
		}
	}
}

void block_getcmd(Block *block, EBlockOrder order) {
	char str[OUTPUT_SIZE]; /* idk why I had to make this here but ok */
	FILE *cmdf;

	/* clean output string */
	for (uint i = 0; i < OUTPUT_SIZE; i++) {
		str[i] = '\0';
	}

	/* prefix */
	if (order == BLOCK_FIRST) {
		strcat_safe(str, block_prefix, OUTPUT_SIZE);
	}

	/* command output */
	char cmd_str[OUTPUT_SIZE];
	cmdf = popen(block->command, "r");
	if (cmdf) {
		fgets(cmd_str, OUTPUT_SIZE - strlen(block->output) - 1, cmdf);
		pclose(cmdf);
		strcat_safe(str, cmd_str, OUTPUT_SIZE);
	} else {
		strcat_safe(str, "<FAIL>", OUTPUT_SIZE);
	}

	/* delimeter or postfix */
	if (order == BLOCK_LAST) {
		strcat_safe(str, block_postfix, OUTPUT_SIZE);
	} else {
		strcat_safe(str, block_delim, OUTPUT_SIZE);
	}

	str_remove_char(str, '\n');
	strcpy(block->output, str);
}

EBlockOrder block_order(uint pos, uint length) {
	if (pos == 0) {
		return BLOCK_FIRST;
	} else if (pos < length - 1) {
		return BLOCK_NORMAL;
	} else {
		return BLOCK_LAST;
	}
}

void find_signal_command(int signal) {
	for (uint i = 0; i < blocks_len; i++) {
		Block *current = &blocks[i];
		if (current->signal == signal) {
			EBlockOrder order = block_order(i, blocks_len);
			block_getcmd(current, order);
			update_status();
		}
	}
}

void get_status(char *str, uint maxsize) {
	for (int i = 0; i < blocks_len; i++) {
		Block *current = &blocks[i];
		strcat_safe(str, current->output, maxsize);
	}
}

void handler_signal(int signum) {
#ifndef __OpenBSD__
	find_signal_command(signum);
	update_status();
#endif
}

void handler_term(int signal) {
	/* free resources */
	free(blocks);

	status_continue = 0;
	exit(0);
}

/* Returns zero if size has exceeded, nonzero if there's still free space */
int strcat_safe(char *string_mod, const char *string_addto, const uint maxsize) {
	uint strpos = strlen(string_mod);
	uint i;

	for (i = 0; string_addto[i]; i++) {
		if (strpos + i == maxsize - 1) break;
		string_mod[strpos + i] = string_addto[i];
	}

	string_mod[strpos + i] = '\0';
	return (strpos + i == maxsize - 1) ? 0 : 1;
}

void str_remove_char(char *str, char c) {
	/* stole and improved this one from Luke Smith */
	char *read = str;
	char *write = str;

	while (*read) {
		while (*read == c) {
			read++;
		}
		*write = *read;
		read++;
		write++;
	}
	*write = 0;
}

void update_status(void) {
		/* get status */
		status_str[0] = '\0';
		get_status(status_str, STATUS_SIZE);

		/* only write if status has changed */
		if (!STR_EQ(status_str, status_str_old)) {
			write_fn(status_str);
		}
}

void write_setroot(char *str) {
	Display *display;
	Window root;
	int screen;

	display = XOpenDisplay(NULL);
	if (!display) return;

	screen = DefaultScreen(display);
	root = RootWindow(display, screen);
	XStoreName(display, root, str);
	XCloseDisplay(display);
}

void write_stdout(char *str) {
	printf("%s\n", str);
	fflush(stdout);
}
