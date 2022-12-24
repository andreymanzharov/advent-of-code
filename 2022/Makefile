CXX = clang++
CXXFLAGS += --stdlib=libc++
CXXFLAGS += $(shell cat compile_flags.txt)
CXXFLAGS += -O3 -g

DAYS = a01 a02 a03 a04 a05 a06 a07 a08 \
       a17 a20 \
       a21 a22 a23 a24

.PHONY: clean

%.in:
	curl -L --cookie "session=${AOC_SESSION}" https://adventofcode.com/2022/day/$(basename $@)/input | tee input.txt

all: $(DAYS)

clean:
	rm -f $(DAYS)
