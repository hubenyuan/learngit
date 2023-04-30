
APPNAME  = temp_cli
LIBPATH1 = `pwd`/include/
LIBPATH2 = `pwd`/lib/

CFLAGS+=-I${LIBPATH1}
LDFLAGS+=-L${LIBPATH2}

CC = gcc

all:
	make -C src
	${CC} ${CFLAGS} socket_client.c -o ${APPNAME} ${LDFLAGS} -llogger -lpackage_data -lmy_sqlite3 -l sqlite3

clean:
	rm -f ${APPNAME}

run:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:LIBPATH
	./${APPNAME} -p 9987 -i 192.168.68.129 -t 3
