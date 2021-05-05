PROGNAME=printraw
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
OBJECTS=main.o

all: $(PROGNAME)

debug: CFLAGS+=-g -O0 -v -Q
debug: $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

install: $(PROGNAME)
	install -m 0755 $(PROGNAME) $(BINDIR)

clean:
	rm -rf *.o
	rm -rf *.exe
	rm -rf $(PROGNAME)
