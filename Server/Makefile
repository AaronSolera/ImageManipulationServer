CC = gcc
MAKE_STATIC_LIB = ar rv
LIB = cd ./lib &&

server: libhttpserver.a
	$(CC) -o server ./src/server.c -I./include ./lib/libhttpserver.a ./lib/libdatahandler.a -pthread

libhttpserver.a: httpserver.o
	$(LIB) $(MAKE_STATIC_LIB) libhttpserver.a httpserver.o

httpserver.o: libdatahandler.a ./lib/httpserver.c
	$(LIB) $(CC) -c httpserver.c -I../include

libdatahandler.a: datahandler.o
	$(LIB) $(MAKE_STATIC_LIB) libdatahandler.a datahandler.o

datahandler.o: ./lib/datahandler.c
	$(LIB) $(CC) -c datahandler.c
