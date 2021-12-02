# make -f c-examples.makefile

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wunused-parameter -pedantic
LDLIBS = -ljpeg -L$(HOME)/vp/bin/qt/opencv/
LDFLAGS="-Wl,-rpath, ./../qt/opencv"

capturesrc := capture.h capture.cpp
srcdir := c-examples
mains := $(wildcard $(srcdir)/*.cpp)
targets := $(patsubst $(srcdir)/%.cpp,%,$(mains))

all: $(targets)

$(targets): %: $(srcdir)/%.cpp $(capturesrc)
	$(CC) $(CFLAGS) $(filter %.cpp, $^) $(LDLIBS) -o $@

clean:
	rm -f $(targets)
