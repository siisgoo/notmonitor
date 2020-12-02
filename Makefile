CC=gcc

GLIBINC=\
	-I/usr/include/glib-2.0\
	-I/usr/lib/glib-2.0/include

NOTIFINC=\
	-I/usr/include/gdk-pixbuf-2.0\
	-I/usr/include/glib-2.0\
	-I/usr/lib/glib-2.0/include\
	-I/usr/include/libmount\
	-I/usr/include/blkid\
	-pthread

INC=$(GLIBINC) $(NOTIFINC)
LDFLAGS=
LDLIBS=\
	-lglib-2.0\
	-lnotify\
	-lgdk_pixbuf-2.0\
	-lgio-2.0\
	-lgobject-2.0

CFLAGS= -c -Wall -xc -O2 $(INC)

SOURCES=notmon.c utils.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=$(shell basename `pwd`)

all: options $(SOURCES) $(EXECUTABLE)

remake: clean all

options:
	@echo build options:
	@echo "CFLAGS	= $(CFLAGS)"
	@echo "LDFLAGS	= $(LDFLAGS)"
	@echo "CC	= $(CC)"
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

.c.o:
		$(CC) $(CFLAGS) $< -o $@
		
install: all
	cp -f $(EXECUTABLE) /usr/local/bin/$(EXECUTABLE)

deinstall:
	rm -f /usr/local/bin/$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
