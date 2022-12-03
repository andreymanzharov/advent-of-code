CXX = clang++
CXXFLAGS += --stdlib=libc++
CXXFLAGS += $(shell cat compile_flags.txt)
CXXFLAGS += -O3 -g

DAYS = a01 a02 a03

.PHONY: clean

all: $(DAYS)

clean:
	rm -f $(DAYS)
