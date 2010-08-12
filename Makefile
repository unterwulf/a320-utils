OBJS = hxfpack hxfunpack bmf2text text2bmf dlx2text text2dlx
CFLAGS = -Wall
PREFIX = /usr/local
DESTDIR =

all: $(OBJS)

install: $(OBJS)
	strip $(OBJS)
	install -t $(DESTDIR)/$(PREFIX)/bin $(OBJS)

clean:
	$(RM) *.o $(OBJS)

hxfpack: hxf_common.o hxfpack.o xstdio.o xintio.o
	$(CC) -o $@ $^

hxfunpack: hxf_common.o hxfunpack.o xstdio.o xintio.o
	$(CC) -o $@ $^

dlx2text: dlx2text.o xstdio.o xintio.o
	$(CC) -o $@ $^

text2dlx: text2dlx.o xstdio.o xintio.o
	$(CC) -o $@ $^

bmf2text: bmf2text.o xstdio.o xintio.o
	$(CC) -o $@ $^

text2bmf: text2bmf.o xstdio.o xintio.o
	$(CC) -o $@ $^
