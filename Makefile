CC=gcc
OBJECTS=nml2txt.o
TARGET=nml2txt
CC=gcc
LD=gcc
CFLAGS=-static
#CFLAGS=-g -ggdb -Wall -Wunused -Wshadow -Wmissing-declarations

LDFLAGS=
LIBS= -lexpat

all: $(TARGET)
clean:
	rm -rf *.o $(TARGET) *~ .depend

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

depend:
	$(CC) $(CFLAGS) -MM $(OBJECTS:%.o=%.c)> .depend

.c.o:
	$(CC) $(CFLAGS) -c $<


.depend:
	@touch .depend

include .depend
