CXX = clang++
CXXFLAGS += --stdlib=libc++
CXXFLAGS += $(shell cat compile_flags.txt)
CXXFLAGS += -O3 -g

DAYS = a01 a02 a03 a04

.PHONY: clean

%.in:
	curl -Lo input.txt --cookie "session=${AOC_SESSION}" https://adventofcode.com/2022/day/$(basename $@)/input

all: $(DAYS)

clean:
	rm -f $(DAYS)
