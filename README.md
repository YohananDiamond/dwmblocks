# dwmblocks

A simple and modular status bar manager for
[`dwm`](https://dwm.suckless.org/), written in C.

# Modifying blocks

The status bar is made from blocks. Each block stores a command that is
ran on the start of dwmblocks and on a specific interval or by receiving
a specific signal.

These blocks and other options (yet to document) can be specified in the
`config.h` file.
