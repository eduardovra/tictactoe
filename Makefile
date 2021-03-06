CC=gcc
CFLAGS=-Wall -g -fPIC -Werror -Wfatal-errors
LDFLAGS=`sdl-config --static-libs` -lrt
INCLUDE=`sdl-config --cflags`
SRCS=tictactoe.c draw.c
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.dep)
TARGET=tictactoe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

$(OBJS): %.o: %.c %.dep
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

$(DEPS): %.dep: %.c Makefile
	$(CC) $(CFLAGS) $(INCLUDE) -MM $< > $@

clean:
	-rm -f *~ *.o *.dep $(TARGET)
