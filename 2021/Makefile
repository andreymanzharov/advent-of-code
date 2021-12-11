CXXFLAGS = $(shell cat compile_flags.txt)
CXXFLAGS += -O3
# CXXFLAGS += -g

DAYS = a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 \
       a11

.PHONY: clean

all: $(DAYS)

clean:
	rm -f $(DAYS)
