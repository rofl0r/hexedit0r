# Use config.mak to override any of the following variables.
# Do not make changes here.

prefix = /usr/local
bindir = $(prefix)/bin
mandir = $(prefix)/share/man

SRCS := $(sort $(wildcard *.c))
OBJS := $(SRCS:.c=.o)

CFLAGS  += -Wall -D_GNU_SOURCE -pipe 

-include config.mak

all: hexedit

install: hexedit hexedit.1
	install -D -m 755 hexedit $(DESTDIR)$(bindir)/hexedit
	install -D -m 644 hexedit.1 $(DESTDIR)$(mandir)/man1/hexedit.1

clean:
	rm -f hexedit
	rm -f $(OBJS)

hexedit: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o hexedit -lncurses

%.o: %.c hexedit.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: all clean install
