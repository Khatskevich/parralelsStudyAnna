#define DEBUG
#include "archivator.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/ipc.h>
#include <linux/msg.h>



int main(int argc, char** argv){
    logInit(LOG_INFO, LOG_PRINT_TIME | LOG_PRINT_LEVEL_DESCRIPTION | LOG_PRINT_FILE | LOG_PRINT_LINE , NULL);
    if ( argc == 3)
    {
        LOGMESG(LOG_INFO, "Decompression...");
        dataRestore(argv[2]);
    }else
    {        
        LOGMESG(LOG_INFO, "Compression...");
        char* files[2];
        files[0] = argv[1];
        files[1] = NULL; 
        dataPresentationControllerInit("compressed", 1, 1, files, argv[0] );
        compressionPerform();
    }
    logClose();
}
 
