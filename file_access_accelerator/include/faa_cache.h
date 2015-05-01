#ifndef _FAA_CACHE_
#define _FAA_CACHE_

#include "log.h"
#include "faa_data_structure.h"

#define DATA_FLUSHING_FROM_CACHE NULL


int faa_cache_init();
int faa_cache_deinit();
int faa_store_data_to_cache( void** data, int priority, FaaDataStruct* faaDataInfo );
int faa_get_data_from_cache( void** data );
int faa_remove_data_from_cache( void** data );
int faa_flush_data_from_cache( void** data, FaaDataStruct* faaDataInfo );

void* cache_compress( void* data, size_t * size ,size_t block_size, int compression_level);
void* cache_decomperess(void* data, size_t size ,size_t block_size, int compression_level);


#endif // _FAA_CACHE_