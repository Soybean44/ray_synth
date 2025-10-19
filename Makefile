CC := gcc
CFLAGS := -Wall -Werror --std=c99 $(shell pkg-config -cflags raylib)
LDFLAGS := -lm $(shell pkg-config -libs raylib) 

all: build/ray_synth

build/ray_synth: build/main.o
	$(CC) $? -o $@ $(LDFLAGS)

build/main.o: src/main.c src/gen.h src/common.h | build/
	$(CC) $(CFLAGS) -c $< -o $@

src/gen.h: build/gen
	./build/gen

build/gen: gen.c | build/
	$(CC) -Wall -Werror --std=c99 $? -o $@ -lm	

build/:
	mkdir -p ./build

clean:
	rm -r build/
