#include "faa_internal.h"
#include "faa.h"

int faa_data_init( size_t block_size,  size_t file_size ){
    return faa_hash_init(block_size, file_size);
}

void* faa_data_inc_ref(FaaDataStruct* faaDataInfo){ 
    return faa_hash_inc_ref(faaDataInfo);
}

int faa_data_dec_ref(FaaDataStruct* faaDataInfo){
    return faa_hash_dec_ref(faaDataInfo);
}

int faa_data_deinit(FaaDataStruct* faaDataInfo ){
    return faa_hash_deinit( faaDataInfo );
}

int faa_data_flush( FaaDataStruct* faaDataInfo ){
    return faa_hash_flush(faaDataInfo);
}