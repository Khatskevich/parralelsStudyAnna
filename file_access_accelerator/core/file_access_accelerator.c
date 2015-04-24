#include "faa_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct {
    size_t file_size;
    size_t cache_size;
    size_t block_size;
    void* mmap_start;
} FaaMainStruct;

FaaMainStruct faaMainInfo;

int faa_init( const char* file_name, size_t block_size, size_t cache_size, int compression_level, size_t * file_size)
{
    int fd;
    if ( file_name == NULL ){
        LOGMESG( LOG_ERROR, "Wrong filname");
        goto faa_init_error_0;
    }
    if ( block_size < 1 ){
        LOGMESG( LOG_ERROR, "Wrong block_size");
        goto faa_init_error_0;
    }
    if ( cache_size < 0 ){
        LOGMESG( LOG_ERROR, "Wrong cache_size");
        goto faa_init_error_0;
    }
    if ( compression_level < 1 || compression_level > 7 ){
        LOGMESG( LOG_ERROR, "Wrong compression_level");
        goto faa_init_error_0;
    }
    if ( file_size == NULL ){
        LOGMESG( LOG_ERROR, "Wrong file_size argument");
        goto faa_init_error_0;
    }
    if ( (fd = open( file_name, O_RDWR )) == -1){
        LOGMESG( LOG_ERROR, "Opening file error");
        goto faa_init_error_0;
    }
    struct stat sb;
    if ( fstat(fd , & sb) == -1 ){
        LOGMESG(LOG_ERROR ,"Error while fstat");
        goto faa_init_error_1;         
    }
    faaMainInfo.file_size = sb.st_size;
    *file_size = sb.st_size;
    
    faaMainInfo.mmap_start = mmap(NULL, sb.st_size , PROT_READ | PROT_WRITE ,
         MAP_SHARED, fd, 0);
    if ( faaMainInfo.mmap_start == MAP_FAILED ){
        LOGMESG(LOG_ERROR ,"Error while creating mmap");
        goto faa_init_error_1;                          
    }
    
    
    
    faa_init_error_2:
        
    faa_init_error_1:
        close(fd);
    faa_init_error_0:
        return -1;
}


void* faa_inc_ref(size_t offset){ 
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    faaDataInfo.data = faaMainInfo.mmap_start + offset;
    return faa_data_inc_ref(&faaDataInfo);
}

int faa_dec_ref(size_t offset){
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    faaDataInfo.data = faaMainInfo.mmap_start + offset;
    return faa_data_dec_ref(&faaDataInfo);
}

int faa_deinit(){
    return faa_data_deinit();
}

int faa_flush(size_t offset){
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    return faa_data_flush(&faaDataInfo);
}