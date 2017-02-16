# Makefile for dummy serial reader
OPTIM = -O2

CFLAGS_EXTRA = `pkg-config --cflags libpcsclite` -Wextra -pedantic
CFLAGS = $(OPTIM) -Wall -std=c99 $(CFLAGS_EXTRA)

LINKER_FLAGS =
LIBS =

SRCS = dummy_ifdh.c
HDRS =

OBJS = $(SRCS:.c=.o)

TARGET = libdummyrdr.so

all: $(TARGET)

$(OBJS): %.o: %.c
		$(CC) -c $(CFLAGS) -fPIC $< -o $@

$(TARGET): $(OBJS) $(HDRS) Makefile
		$(CC) $(CFLAGS) -shared  $(OBJS) $(LIBS) $(LINKER_FLAGS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

#libdummyrdr.so: $(OBJ)
#$(CC) -shared $(OBJ) -o libdummyrdr.so

#CFLAGS = -Wall -O2 -fPIC -c -DG_UNIX -DHAVE_PTHREAD_H $(CFLAGS_EXTRA) -Wall -Wextra -pedantic
#CFLAGS_EXTRA=`pkg-config libpcsclite --cflags` -I.
#
#OBJ	= ifdhandler.o
#
#all:
#ifdhandler.o: ifdhandler.c Makefile
#${CC} ${CFLAGS} $(CFLAGS_EXTRA) ifdhandler.c
#
