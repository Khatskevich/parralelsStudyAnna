#ifndef _FAA_HASH_
#define _FAA_HASH_

#include "faa_internal.h"
#include "faa.h"

int faa_hash_init( size_t block_size, size_t file_size);

void* faa_hash_inc_ref(FaaDataStruct* faaDataInfo);

int faa_hash_dec_ref(FaaDataStruct* faaDataInfo);

int faa_hash_deinit(FaaDataStruct* faaDataInfo );

int faa_hash_flush( FaaDataStruct* faaDataInfo );


#endif // _FAA_HASH_
