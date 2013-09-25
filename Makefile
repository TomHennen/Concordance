CFLAGS=-g -Iuthash/src -I.
SRCDIR=src

all: concordance

run: concordance
	./$^ data/test1.txt

state.o: $(SRCDIR)/state.c $(SRCDIR)/state.h
	clang -o $@ -c $< $(CFLAGS)

process.o: $(SRCDIR)/process.c $(SRCDIR)/process.h $(SRCDIR)/state.h
	clang -o $@ -c $< $(CFLAGS)

display.o: $(SRCDIR)/display.c $(SRCDIR)/display.h $(SRCDIR)/state.h
	clang -o $@ -c $< $(CFLAGS)

main.o: $(SRCDIR)/main.c $(SRCDIR)/display.h $(SRCDIR)/process.h $(SRCDIR)/state.h
	clang -o $@ -c $< $(CFLAGS)

concordance: main.o state.o display.o process.o
	clang -o $@ $^ $(CFLAGS)


clean:
	rm *.o
	rm concordance
