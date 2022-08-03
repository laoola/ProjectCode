TARGET = app

PWD=$(shell pwd)
OBJ_PATH := ./obj

CC = g++

DARGS = -Wall
release: ARGS = $(DARGS) $(release) -O2
debug: ARGS = $(DARGS) -g -O0

INCLUDE = -I ./include -I./
LIBS = -ludev -lpthread

DIRS := $(shell ls ./ -R | grep '^\./.*:$$' | awk '{gsub(":","");print}')
#files = $(wildcard *.cpp)
files = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
objs = $(patsubst %.cpp, %.o, $(files))


all: debug

debug : $(objs)
	$(CC) $(ARGS) -o $(TARGET) $^ $(LIBS)

release : $(objs)
	$(CC) $(ARGS) -o $(TARGET) $^ $(LIBS)
	strip --strip-debug $(TARGET)

%.o: %.cpp
	$(CC) $(ARGS) $(INCLUDE) -c $<

clean:
	rm -f $(TARGET) $(objs)
