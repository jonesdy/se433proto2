CC=gcc
CFLAGS=-Wall -c -fpic
LIBFLAGS=-shared -o
LIBRARY=face_lib.so

all:face_io_api.o
	$(CC) $(LIBFLAGS) $(LIBRARY) $^

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *o $(LIBRARY)
