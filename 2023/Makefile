CXX = clang++
CXXFLAGS += --stdlib=libc++
CXXFLAGS += $(shell cat compile_flags.txt)
CXXFLAGS += -O3 -g

DAYS = a01

.PHONY: clean

%.in:
	curl -L --cookie "session=${AOC_SESSION}" https://adventofcode.com/2023/day/$(basename $@)/input | tee input.txt

all: $(DAYS)

clean:
	rm -f $(DAYS)
