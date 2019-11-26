CC=gcc
INCLUDE=../include
CFLAGS=-I$(INCLUDE) -g -Wall -c
LDFLAGS=-lpthread
TARGET=thread_pool_server

SRCFILE=$(wildcard *.c)
DESFILE=$(patsubst %.c,%.o,$(SRCFILE))

$(TARGET):$(DESFILE)
	$(CC) $^ $(LDFLAGS) -o $@

%./o:%.c
	$(CC) $(CFLAGS) $<

clean:
	-rm -rf $(DESFILE)
	-rm -rf $(TARGET)
