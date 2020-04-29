# Use config.mak to override any of the following variables.
# Do not make changes here.

.POSIX:

prefix = /usr/local
bindir = $(prefix)/bin
mandir = $(prefix)/share/man

OBJS = display.o \
       file.o \
       hexedit.o \
       interact.o \
       mark.o \
       misc.o \
       page.o \
       search.o

CFLAGS = -Wall -D_GNU_SOURCE -pipe

include config.mak

all: hexedit

$(OBJS): hexedit.h

install: hexedit hexedit.1
	install -D -m 755 hexedit $(DESTDIR)$(bindir)/hexedit
	install -D -m 644 hexedit.1 $(DESTDIR)$(mandir)/man1/hexedit.1

clean:
	rm -f hexedit
	rm -f $(OBJS)

hexedit: $(OBJS)
	$(CC) $(LDFLAGS) -o hexedit $(OBJS) -lncurses

.SUFFIXES:
.SUFFIXES: .o .c

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

.PHONY: all clean install
