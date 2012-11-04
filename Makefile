CFLAGS:=-O2 -g -std=gnu99 -Wall -Werror -D_GNU_SOURCE -fPIC -I./include

.PHONY : clean

libcgoodies.so : ringbuffer/ringbuffer.o
	gcc -g -shared -Wl,-soname,libcgoodies.so -o libcgoodies.so */*.o

clean :
	rm -f */*.o libcgoodies.so
