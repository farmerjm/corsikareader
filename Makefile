HEADERS = $(wildcard include/*.h) $(wildcard common/*.h)
OBJS=$(patsubst include/%.h, src/%.o, $(wildcard include/*.h))

LDLIBS = -I include -I common `root-config --glibs


CFLAGS  = -g -DLINUX  -g `root-config --cflags` -std=c++11
LDFLAGS = -m32 `root-config --ldflags`
LDLIBS = -I include -I common `root-config --glibs`

CXX = g++


TARGET= read
PROG = read
PROGOBJ = read.o
#PROGS=$(patsubst %.cc, %, $(strip $(wildcard src/*.cc)))
#PROGOBJ=$(patsubst %, %.o, $(PROGS))


all: $(TARGET)

$(TARGET): %: %.o $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(PROGOBJ): %.o: %.cc
	$(CXX) -c $< -o $@ $(CFLAGS) $(LDLIBS)

$(OBJS): %.o: %.cc $(HEADERS)
	$(CXX) -c $< -o $@ $(CFLAGS) $(LDLIBS)


.PHONY: clean
clean:
	rm *.o
	rm src/*.o
