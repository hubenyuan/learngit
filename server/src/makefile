

PREFIX = `pwd`/../


LIBNAME = server_sqlite3

all: static_lib

static_lib:
	@gcc -c *.c
	ar -rcs lib${LIBNAME}.a *.o


clean:
	rm -f *.o

distclean:clean
	rm -f *.so *.a

install:
	mkdir -p ${PREFIX}/lib
	mkdir -p ${PREFIX}/include
	cp lib*.a ${PREFIX}/lib
	cp *.h ${PREFIX}/include

uninstall:
	rm -f ${PREFIX}/lib/lib${LIBNAME}.*
	rm -f ${PREFIX}/include/logger.h
