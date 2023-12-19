JAVA_OPTS = --source 21 --enable-preview -Djava.net.useSystemProxies=true

DAYS = a01 a02 a03 a04 a05 a06 a07 a08 a09 a10 \
       a11 a12 a13 a14 a15 a16 a17 a18 a19

.PHONY: clean

all: $(DAYS)

a%: src/a%.java
	$(JAVA_HOME)/bin/java $(JAVA_OPTS) $^

clean:
	rm -f $(DAYS:=.in)
	rm -rf out/
