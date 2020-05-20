CC=gcc
CFLAGS=-I.
PROG=ImageServer

.PHONY	:	all

all	: build	config

build	:
		$(CC) -o ./bin/$(PROG) ./src/$(PROG).c

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
