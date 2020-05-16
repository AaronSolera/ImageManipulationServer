CC = gcc
MAKE_STATIC_LIB = ar rv
LIB = cd ./lib &&

server: libhttpserver.a
	$(CC) -o server ./src/server.c -I./include ./lib/libhttpserver.a ./lib/libjsonhandler.a -pthread

libhttpserver.a: httpserver.o
	$(LIB) $(MAKE_STATIC_LIB) libhttpserver.a httpserver.o

httpserver.o: libjsonhandler.a ./lib/httpserver.c
	$(LIB) $(CC) -c httpserver.c -I../include

libjsonhandler.a: jsonhandler.o
	$(LIB) $(MAKE_STATIC_LIB) libjsonhandler.a jsonhandler.o

jsonhandler.o: ./lib/jsonhandler.c
	$(LIB) $(CC) -c jsonhandler.c
