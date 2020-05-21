CC = gcc
MAKE_STATIC_LIB = ar rv
LIB = cd ./lib &&
PROG=ImageServer
CFLAGS=-I.

.PHONY	:	all

all: daemon config

daemon : libhttpserver.a
	$(CC) -o ./bin/$(PROG) ./src/$(PROG).c -I./include ./lib/libhttpserver.a ./lib/libdatahandler.a ./lib/libprocesshandler.a -pthread

libhttpserver.a: httpserver.o
	$(LIB) $(MAKE_STATIC_LIB) libhttpserver.a httpserver.o

httpserver.o: libdatahandler.a libprocesshandler.a ./lib/httpserver.c
	$(LIB) $(CC) -c httpserver.c -I../include

libdatahandler.a: datahandler.o
	$(LIB) $(MAKE_STATIC_LIB) libdatahandler.a datahandler.o

datahandler.o: ./lib/datahandler.c
	$(LIB) $(CC) -c datahandler.c

libprocesshandler.a: processhandler.o
	$(LIB) $(MAKE_STATIC_LIB) libprocesshandler.a processhandler.o

processhandler.o: ./lib/processhandler.c
	$(LIB) $(CC) -c processhandler.c

config	:
		@echo "Moving bin file..."
		@cp ./bin/$(PROG)	/usr/bin
		@echo "Moving service file (ImageServer)..."
		@cp ./DaemonConfig/ImageServer.service /usr/lib/systemd/system
		@echo "Moving service file (ImageServer Stop(SO ShutDown))..."
		@cp ./DaemonConfig/ImageServerStopSO.service /usr/lib/systemd/system
		@echo "Moving Stop Service ShutDown bash script"
		@cp ./DaemonConfig/ImageServerStopSO.sh /usr/bin
		@echo "Moving config file..."
		@cp ./DaemonConfig/config.conf /etc/server/
		@echo "Set Service on Boot"
		@systemctl enable ImageServer.service
		@systemctl enable ImageServerStopSO.service
