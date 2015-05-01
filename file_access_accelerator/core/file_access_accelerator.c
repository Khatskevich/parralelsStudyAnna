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
    int initialised;
} FaaMainStruct;

FaaMainStruct faaMainInfo = {.initialised = 0};

int faa_init( const char* file_name, size_t block_size, size_t cache_size, int compression_level, size_t * file_size)
{
    logInit(LOG_INFO , LOG_PRINT_TIME | LOG_PRINT_LEVEL_DESCRIPTION | LOG_PRINT_FILE | LOG_PRINT_LINE , NULL);
    int fd,rc;
    if ( faaMainInfo.initialised == 1){
        LOGMESG( LOG_ERROR, "FAA is currently initialised");
        goto faa_init_error_0;
    }
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
    if ( file_size == NULL ){
        LOGMESG( LOG_ERROR, "Wrong file_size argument");
        goto faa_init_error_0;
    }
    
    faaMainInfo.block_size = block_size;
    faaMainInfo.cache_size = cache_size;
    
    if ( (fd = open( file_name, O_RDWR )) == -1){
        LOGMESG( LOG_ERROR, "Opening file error : %s", file_name);
        perror("");
        goto faa_init_error_0;
    }
    struct stat sb;
    if ( fstat(fd , & sb) == -1 ){
        LOGMESG(LOG_ERROR ,"Error while fstat");
        goto faa_init_error_1;         
    }
    if ( sb.st_size % block_size != 0){
         sb.st_size = sb.st_size - (sb.st_size % block_size) + block_size;
        rc = ftruncate(fd,sb.st_size);
    }
    faaMainInfo.file_size = sb.st_size;
    *file_size = sb.st_size;
    
    faaMainInfo.mmap_start = mmap(NULL, sb.st_size , PROT_READ | PROT_WRITE ,
         MAP_SHARED, fd, 0);
    if ( faaMainInfo.mmap_start == MAP_FAILED ){
        LOGMESG(LOG_ERROR ,"Error while creating mmap");
        goto faa_init_error_1;                          
    }
    rc = faa_cache_init( block_size, cache_size, compression_level, faaMainInfo.file_size  );
    if ( rc != 0){
        LOGMESG(LOG_ERROR ,"Error while creating archived cache");
        goto faa_init_error_2;
    }
    rc = faa_data_init(block_size, faaMainInfo.file_size);
    if (rc < 0){
        LOGMESG(LOG_ERROR ,"Error during data initialisation");
        goto faa_init_error_3;       
    }
    faaMainInfo.initialised = 1;
    LOGMESG(LOG_INFO ,"FAA initialised successefully, \nfile size = %lld", (long long int) faaMainInfo.file_size );
    return 0;
    faa_init_error_3:
        
    faa_init_error_2:
        munmap(faaMainInfo.mmap_start, faaMainInfo.file_size );
    faa_init_error_1:
        close(fd);
    faa_init_error_0:
        return -1;
}


void* faa_inc_ref(size_t offset){
    if ( faaMainInfo.initialised == 0 ){
        LOGMESG( LOG_ERROR, "FAA is currently deinitialised");
        return NULL;
    }
    if ( offset % faaMainInfo.block_size != 0 || offset > faaMainInfo.file_size ){
        LOGMESG( LOG_ERROR ,"WRONG offset" );
        return NULL;
    }
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    faaDataInfo.data = faaMainInfo.mmap_start + offset;
    return faa_data_inc_ref(&faaDataInfo);
}

int faa_dec_ref(size_t offset){
    if ( faaMainInfo.initialised == 0 ){
        LOGMESG( LOG_ERROR, "FAA is currently deinitialised");
        return -1;
    }
    if ( offset % faaMainInfo.block_size != 0 || offset > faaMainInfo.file_size ){
        LOGMESG( LOG_ERROR ,"WRONG offset" );
        return -1;
    }
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    faaDataInfo.data = faaMainInfo.mmap_start + offset;
    return faa_data_dec_ref(&faaDataInfo);
}

int faa_deinit(){
    LOGMESG( LOG_INFO, "FAA deinitialisation...");
    int rc;
    if ( faaMainInfo.initialised == 0){
        LOGMESG( LOG_ERROR, "FAA is currently deinitialised");
        return -1;
    }
    FaaDataStruct faaDataInfo;
    faaDataInfo.data = faaMainInfo.mmap_start;
    rc = faa_data_deinit(&faaDataInfo);
    if ( rc == 0 ){
        faaMainInfo.initialised = 0;
        return 0;
    }
    faa_cache_deinit();
}

int faa_flush(size_t offset){
    if ( faaMainInfo.initialised == 0 ){
        LOGMESG( LOG_ERROR, "FAA is currently deinitialised");
        return -1;
    }
    if ( (offset % faaMainInfo.block_size != 0 || offset > faaMainInfo.file_size) && offset != FAA_FLUSH_ALL ){
        LOGMESG( LOG_ERROR ,"WRONG offset" );
        return -1;
    }
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = offset;
    return faa_data_flush(&faaDataInfo);
}