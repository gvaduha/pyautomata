CC=gcc
CFLAGS:=$(shell python3-config --cflags)
LDFLAGS:=$(shell python3-config --ldflags)

.PHONY: build

build:
	#@echo $(CFLAGS)
	$(CC) -o pycallback *.c $(CFLAGS) $(LDFLAGS)
    
