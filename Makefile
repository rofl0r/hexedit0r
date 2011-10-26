#
# Makefile for proxychains (requires GNU make), stolen from musl
#
# Use config.mak to override any of the following variables.
# Do not make changes here.
#

exec_prefix = /usr/local
bindir = $(exec_prefix)/bin

prefix = /usr/local/
includedir = $(prefix)/include
libdir = $(prefix)/lib

SRCS = $(sort $(wildcard *.c))
OBJS = $(SRCS:.c=.o)
LOBJS = $(OBJS:.o=.lo)

CFLAGS  += -Wall -O0 -g -std=c99 -D_GNU_SOURCE -pipe 
INC     = 
PIC     = -fPIC -O0
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib

-include config.mak

all: hexedit

install: 
	install -D -m 755 hexedit $(bindir)

clean:
	rm -f hexedit
	rm -f $(OBJS)

hexedit: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o hexedit -lncurses

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_MAIN) $(INC) -c -o $@ $<

.PHONY: all clean install
