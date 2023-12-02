JAVA_OPTS = --source 21 --enable-preview -Djava.net.useSystemProxies=true

DAYS = a01 a02

.PHONY: clean

all: $(DAYS)

a%: src/a%.java
	$(JAVA_HOME)/bin/java $(JAVA_OPTS) $^

clean:
	rm -f $(DAYS:=.in)
	rm -rf out/
