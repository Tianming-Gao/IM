CC=g++
INCLUDE=../include
CFLAGS=-I$(INCLUDE) -g -Wall -c
LDFLAGS=-lpthread
TARGET=thread_pool_server

SRCFILE=$(wildcard *.cpp)
DESFILE=$(patsubst %.cpp,%.o,$(SRCFILE))

$(TARGET):$(DESFILE)
	$(CC) $^ $(LDFLAGS) -o $@

%./o:%.cpp
	$(CC) $(CFLAGS) $<

clean:
	-rm -rf $(DESFILE)
	-rm -rf $(TARGET)
