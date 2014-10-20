CC=gcc
CFLAGS=-Wall -c -fpic
LIBFLAGS=-shared -o
LIBRARY=face_io_lib
EXECUTABLE=face_pss.out
all:lib exe

exe:face_pss.o
	LD_PATH=. $(CC) -o $(EXECUTABLE) $^ -Wl,-rpath,. lib$(LIBRARY).so

lib:face_io_api.o config_parser.o
	$(CC) $(LIBFLAGS) lib$(LIBRARY).so $^

%.o:%.c
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *o $(LIBRARY) $(EXECUTABLE) testrepl

repltest: repl.c
	$(CC) -DCOLOR -DTESTREPL repl.c -o repltest
