CFLAGS=-Iuthash/src -I.
SRCDIR=src

concordance: $(SRCDIR)/main.c
	clang -o $@ $^ $(CFLAGS)

run: concordance
	./concordance
