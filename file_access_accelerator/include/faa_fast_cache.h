#ifndef _FAA_FAST_CACHE_
#define _FAA_FAST_CACHE_

#include "log.h"
#include "faa_data_structure.h"
#include "faa_cache.h"
#include "faa_hash.h"

#define MAXIMAL_NUMBER_OF_PRIORITIES 100

int fast_cache_init( size_t block_size, size_t cache_size, int compression_level, size_t filesize );
int fast_cache_deinit();
int fast_store_data_to_cache( void** data, int priority, FaaDataStruct* faaDataInfo );
int fast_get_data_from_cache( void** data );
int fast_remove_data_from_cache( void** data );

int fast_flush_data_from_cache( void** data, FaaDataStruct* faaDataInfo );


#endif // _FAA_FAST_CACHE_
