GCC = gcc
flags = -g -Wall

head = -I lib/include/

lib = -L lib \
	  -l dingjingc

core_head = $(wildcard src/*/*.h)
core_src = $(wildcard src/*/*.c)
test_src = $(wildcard example/*.c)

test_obj = $(patsubst %.c, %.o, $(test_src))
core_obj = $(patsubst %.c, %.o, $(core_src))

test_target = $(patsubst %.c, %.run, $(test_src))

library = libdingjingc.so

all: reply library demo


demo:$(test_target)
	mv $^ -t "bin/"

library:$(core_obj)
	ar -crv $(library) $^
	mv $(library) -t "lib"

reply:
	mkdir -p "lib/include/"
	mkdir -p "bin/"
	cp $(core_head) -t "lib/include/"

%.run:%.o $(test_obj)
	$(GCC) -o $@ $< $(flags) $(head) $(lib)

%.o:%.c
	$(GCC) -o $@ -c $< $(flags) $(head)

.PHONY:library demo clean all reply

clean:
	rm -f $(test_target)
	rm -f $(test_obj)
	rm -f $(core_obj)
	rm -rf 'lib'
	rm -rf 'bin'

