#ifndef _FAA_DATA_STRUCTURE_
#define _FAA_DATA_STRUCTURE_

#include "faa_internal.h"

typedef struct{
    size_t offset;
    void* data;
} FaaDataStruct;




int faa_data_init( size_t block_size,  size_t file_size );


void* faa_data_inc_ref(FaaDataStruct* faaDataInfo);

int faa_data_dec_ref(FaaDataStruct* faaDataInfo);

int faa_data_deinit();

int faa_data_flush( FaaDataStruct* faaDataInfo );

#endif // _FAA_DATA_STRUCTURE_
