#include "faa_internal.h"
#include <faa_data_structure.h>
#include <string.h>
#include <stdlib.h>
#include "zlib/zlib.h"

int faa_cache_init(size_t block_size, size_t cache_size, int compression_level, size_t filesize ){
    return fast_cache_init(block_size, cache_size,  compression_level,filesize );
}
int faa_cache_deinit( ){
    return fast_cache_deinit();
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
    int rc;
    unsigned long dest_len = block_size*2+1000;
    void* temp_buffer = malloc( dest_len ) ;
    rc = compress2( temp_buffer,  &dest_len, data, block_size,compression_level);
    return realloc( temp_buffer, dest_len);
}

void* cache_decomperess(void* data, size_t size ,size_t block_size, int compression_level){
    int rc;
    unsigned long dest_len = block_size*2+1000;
    void* temp_buffer = malloc( dest_len ) ;
    rc = uncompress( temp_buffer,  &dest_len, data, size);
    return realloc( temp_buffer, dest_len);
}


//ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
//                                   const Bytef *source, uLong sourceLen));

/*
     Decompresses the source buffer into the destination buffer.  sourceLen is
   the byte length of the source buffer.  Upon entry, destLen is the total size
   of the destination buffer, which must be large enough to hold the entire
   uncompressed data.  (The size of the uncompressed data must have been saved
   previously by the compressor and transmitted to the decompressor by some
   mechanism outside the scope of this compression library.) Upon exit, destLen
   is the actual size of the uncompressed buffer.

     uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
   enough memory, Z_BUF_ERROR if there was not enough room in the output
   buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.  In
   the case where there is not enough room, uncompress() will fill the output
   buffer with the uncompressed data up to that point.
*/

                        /* gzip file access functions */

/*
     This library supports reading and writing files in gzip (.gz) format with
   an interface similar to that of stdio, using the functions that start with
   "gz".  The gzip format is different from the zlib format.  gzip is a gzip
   wrapper, documented in RFC 1952, wrapped around a deflate stream.
*/