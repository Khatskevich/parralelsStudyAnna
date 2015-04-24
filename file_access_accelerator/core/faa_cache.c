#include "faa_internal.h"
#include <faa_data_structure.h>
#include <string.h>
#include <stdlib.h>

int faa_cache_init(size_t block_size, size_t cache_size, int compression_level, size_t filesize ){
    return fast_cache_init(block_size, cache_size,  compression_level,filesize );
}
int faa_store_data_to_cache( void** data, int priority, FaaDataStruct* faaDataInfo ){
    return fast_store_data_to_cache( data, priority, faaDataInfo);
}
int faa_get_data_from_cache( void** data ){
    return fast_get_data_from_cache( data);
}
int faa_remove_data_from_cache( void** data ){
    return fast_remove_data_from_cache(data);
}
int faa_flush_data_from_cache( void** data, FaaDataStruct* faaDataInfo ){
    return fast_flush_data_from_cache( data , faaDataInfo );
}


void* cache_compress( void* data, size_t * size ,size_t block_size, int compression_level){
}

void* cache_decomperess(void* data, size_t size ,size_t block_size, int compression_level){
}

