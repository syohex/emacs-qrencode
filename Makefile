EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS)
LDFLAGS =

.PHONY : test

all: qrencode-core.so

qrencode-core.so: qrencode-core.o
	$(LD) -shared $(LDFLAGS) -o $@ $^ -lqrencode

qrencode-core.o: qrencode-core.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f qrencode-core.so qrencode-core.o

test:
	$(EMACS) -Q -batch -L . $(LOADPATH) \
		-l test/test-qrencode.el \
		-f ert-run-tests-batch-and-exit
