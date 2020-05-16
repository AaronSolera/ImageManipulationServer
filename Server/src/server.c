#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <httpserver.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	// Starting HTTPserver
    startServer();
    restartServer();

    // Here goes Nacho's Code, it needs to be an endless while or another thread
    while(1){
    	sleep(1);
    }
    
    return 0;
}
