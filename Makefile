PROGRAMS = hxfpack hxfunpack bmf2text text2bmf dlx2text text2dlx
CFLAGS = -Wall
PREFIX = /usr/local
DESTDIR =

all: $(PROGRAMS)

install: $(PROGRAMS)
	strip $(PROGRAMS)
	install -t $(DESTDIR)/$(PREFIX)/bin $(PROGRAMS)

clean:
	$(RM) *.o $(PROGRAMS)

hxfpack: hxfpack.o hxf_common.o xstdio.o xintio.o
hxfunpack: hxfunpack.o hxf_common.o xstdio.o xintio.o
dlx2text: dlx2text.o xstdio.o xintio.o
text2dlx: text2dlx.o xstdio.o xintio.o
bmf2text: bmf2text.o xstdio.o xintio.o
text2bmf: text2bmf.o xstdio.o xintio.o

$(PROGRAMS):
	$(CC) -o $@ $^
