//#include "file_access_accelerator.h"
#include "faa.h"
#include "faa_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAA_HASH_LAST_LEVEL_NUMBER_OF_PICES_COUNT 1024

typedef struct{
    int maxReferenceCounter;
    int referenceCounter;
    void* data; // CacheID* if rC = 0;
} LastLevelHashPiceStruct;

typedef struct{
    LastLevelHashPiceStruct* pices; 
    size_t numberOfPices;
    size_t numberOfUsedPices;
} LastLevelHashHeaderStruct;

/*
typedef struct{
    LastLevelHashHeaderStruct* lastLevelHashHeaderInfo;
} FirstLevelHashPiceStruct;

typedef struct{
    FirstLevelHashPiceStruct* pices; 
    size_t numberOfPices;
    size_t numberOfUsedPices;
} FirstLevelHashHeaderStruct;
*/

typedef struct{
    LastLevelHashHeaderStruct * lastLevelHashHeaderInfo;
    size_t block_size;
    size_t file_size;  
} HashMainStruct;

HashMainStruct hashMainInfo ;


void* last_level_hash_inc_ref( LastLevelHashHeaderStruct* lastLevelHashHeaderInfo, FaaDataStruct* faaDataInfo);
LastLevelHashHeaderStruct * last_level_hash_header_init( size_t block_size , size_t size );

int faa_hash_init( size_t block_size, size_t file_size){
    LOGMESG(LOG_INFO ,"FAA HASH init");
    hashMainInfo.file_size = file_size;
    hashMainInfo.block_size = block_size;
    hashMainInfo.lastLevelHashHeaderInfo = last_level_hash_header_init( block_size, file_size );
    if ( hashMainInfo.lastLevelHashHeaderInfo == NULL){
        LOGMESG(LOG_ERROR, "hashMainInfo.lastLevelHashHeaderInfo == NULL");
        return -1;
    }
    return 0;
}


LastLevelHashHeaderStruct * last_level_hash_header_init( size_t block_size , size_t size )
{
    LOGMESG(LOG_INFO, "last_level_hash_header_init");
    size_t i = 0;
    size_t number_of_pices = size/block_size + 1;
    LastLevelHashHeaderStruct* lastLevelHashHeaderInfo;
    LOGMESG(LOG_INFO, "Trying to malloc %lld bytes", (long long int) sizeof(LastLevelHashHeaderStruct) + sizeof( LastLevelHashPiceStruct ) * ( number_of_pices ) );
    lastLevelHashHeaderInfo = (LastLevelHashHeaderStruct*) malloc( sizeof(LastLevelHashHeaderStruct) + sizeof( LastLevelHashPiceStruct ) * ( number_of_pices ) );
    if ( lastLevelHashHeaderInfo == NULL){
        LOGMESG(LOG_ERROR, "Malloc");
        return NULL;
    }
    lastLevelHashHeaderInfo->pices = (LastLevelHashPiceStruct*) &lastLevelHashHeaderInfo[1];
    lastLevelHashHeaderInfo->numberOfUsedPices = 0;
    lastLevelHashHeaderInfo->numberOfPices = number_of_pices;
    for ( i = 0; i < number_of_pices; i++){
        lastLevelHashHeaderInfo->pices[i].data = NULL;
        lastLevelHashHeaderInfo->pices[i].referenceCounter = -1;
        lastLevelHashHeaderInfo->pices[i].maxReferenceCounter = 0;
    }
    LOGMESG(LOG_INFO ,"FAA HASH initialised successefully");
    
    return lastLevelHashHeaderInfo;
}

void* faa_hash_inc_ref(FaaDataStruct* faaDataInfo){
    return last_level_hash_inc_ref( hashMainInfo.lastLevelHashHeaderInfo, faaDataInfo);
}

int faa_hash_dec_ref(FaaDataStruct* faaDataInfo){
    return last_level_hash_dec_ref( hashMainInfo.lastLevelHashHeaderInfo , faaDataInfo);
}

int faa_hash_deinit( FaaDataStruct* faaDataInfo ){
    return last_level_hash_deinit(hashMainInfo.lastLevelHashHeaderInfo, faaDataInfo);
}

int faa_hash_flush( FaaDataStruct* faaDataInfo ){
    return last_level_hash_flush( hashMainInfo.lastLevelHashHeaderInfo , faaDataInfo);
}

void* last_level_hash_inc_ref( LastLevelHashHeaderStruct* lastLevelHashHeaderInfo, FaaDataStruct* faaDataInfo){
    LOGMESG(LOG_INFO ,"FAA HASH inc ref");
    size_t number = faaDataInfo->offset / hashMainInfo.block_size;
    LastLevelHashPiceStruct* pice = lastLevelHashHeaderInfo->pices + number;
    if ( pice->referenceCounter < 0 ){
        pice->data = malloc(hashMainInfo.block_size);
        if ( pice->data == NULL ) {
            LOGMESG(LOG_ERROR, "Malloc");
            return NULL;
        }
        memcpy( pice->data , faaDataInfo->data , hashMainInfo.block_size);
        pice->referenceCounter = 1;
    } else if( pice->referenceCounter == 0){ // cached
        faa_get_data_from_cache( &pice->data);
        pice->referenceCounter = 1;
         goto last_level_hash_inc_ref_exit_ok;
    } else if( pice->referenceCounter < 100500 ){//1234 ip3ojr0 eujrf9qejrf     j0rfj0  3jr0f   jef0j230jr0f2j3f023j0rj23dj
        pice->referenceCounter = pice->referenceCounter + 1;
        goto last_level_hash_inc_ref_exit_ok;
    } else{
        LOGMESG( LOG_ERROR, "Reference counter is too big!");
    }
    last_level_hash_inc_ref_exit_ok:
        return pice->data;
}




int last_level_hash_dec_ref( LastLevelHashHeaderStruct* lastLevelHashHeaderInfo, FaaDataStruct* faaDataInfo){
    LOGMESG(LOG_INFO ,"FAA HASH dec ref");
    size_t number = faaDataInfo->offset / hashMainInfo.block_size;
    LastLevelHashPiceStruct* pice = lastLevelHashHeaderInfo->pices + number;
    
    if ( faaDataInfo->data == DATA_FLUSHING_FROM_CACHE ){
        if ( pice->referenceCounter != 0 ){
            LOGMESG( LOG_ERROR , "REmoving from cache when ref count != 0 ");
            goto last_level_hash_dec_ref_exit_ok;
        }
        pice->referenceCounter = -1;
        free(pice->data);
        goto last_level_hash_dec_ref_exit_ok;
    }
    
    if ( pice->referenceCounter < 0 ){
        goto last_level_hash_dec_ref_exit_error_0;
    } else if( pice->referenceCounter == 0){ // cached
        goto last_level_hash_dec_ref_exit_error_0;
    }
    if ( pice->maxReferenceCounter < pice->referenceCounter){
        pice->maxReferenceCounter = pice->referenceCounter;
    }
    if ( pice->referenceCounter == 1){
        faa_store_data_to_cache( &pice->data, pice->maxReferenceCounter, faaDataInfo );
    }
    pice->referenceCounter =  pice->referenceCounter - 1;
    last_level_hash_dec_ref_exit_ok:
        return 0;
    last_level_hash_dec_ref_exit_error_0:
        return -1;
}
int last_level_hash_deinit( LastLevelHashHeaderStruct* lastLevelHashHeaderInfo , FaaDataStruct* faaDataInfo_){
    LOGMESG(LOG_INFO ,"FAA HASH deinit");
    size_t i = 0;
    FaaDataStruct faaDataInfo;
    faaDataInfo.offset = FAA_FLUSH_ALL; 
    faaDataInfo.data = faaDataInfo_->data;
    int rc = last_level_hash_flush( lastLevelHashHeaderInfo, &faaDataInfo);
    for (i = 0; i < lastLevelHashHeaderInfo->numberOfPices; i++ ){
        LastLevelHashPiceStruct * pice = &lastLevelHashHeaderInfo->pices[i];
        if ( pice->referenceCounter == 0 ){
            faa_remove_data_from_cache( &pice->data);
        } else if ( pice->referenceCounter > 0 ){
            free( pice->data);
        }
    }
    free(lastLevelHashHeaderInfo);
    
}
int last_level_hash_flush( LastLevelHashHeaderStruct* lastLevelHashHeaderInfo, FaaDataStruct* faaDataInfo){
    LOGMESG(LOG_INFO ,"FAA HASH flush, offset = %llx", faaDataInfo->offset );
    int rc;
    if ( faaDataInfo->offset == FAA_FLUSH_ALL ){
        size_t i = 0;
        for (i = 0; i < lastLevelHashHeaderInfo->numberOfPices; i++ ){
            FaaDataStruct faaDataInfoTemp;
            faaDataInfoTemp.data = (void*)((char*) faaDataInfo->data + i*hashMainInfo.block_size );
            faaDataInfoTemp.offset = i*hashMainInfo.block_size ;
            LastLevelHashPiceStruct* pice = lastLevelHashHeaderInfo->pices + i;
            rc = last_level_hash_flush_pice( pice, &faaDataInfoTemp);
            if ( rc < 0){
                return rc;
            }
        }
        
    } else {
        size_t number = faaDataInfo->offset / hashMainInfo.block_size;
        LastLevelHashPiceStruct* pice = lastLevelHashHeaderInfo->pices + number;
        return last_level_hash_flush_pice( pice , faaDataInfo );
    }
}

int last_level_hash_flush_pice( LastLevelHashPiceStruct* pice, FaaDataStruct* faaDataInfo ){
    LOGMESG(LOG_INFO ,"FAA HASH flush_pice , offset = %llx", faaDataInfo->offset );
    if ( pice->referenceCounter == 0){
        return faa_flush_data_from_cache(&pice->data, faaDataInfo );
    }else if( pice->referenceCounter > 0 ){
        memcpy( faaDataInfo->data, pice->data, hashMainInfo.block_size );
    }
}


/*
FirstLevelHashHeaderStruct * first_level_hash_header_init(  size_t block_size,  size_t * file_size  )
{
    size_t number_of_pices = file_size/(block_size* FAA_HASH_LAST_LEVEL_NUMBER_OF_PICES_COUNT) +1;
    size_t i = 0;
    FirstLevelHashHeaderInfo* firstLevelHashHeaderInfo;
    
    firstLevelHashHeaderInfo = (FirstLevelHashHeaderInfo*) malloc ( sizeof(FirstLevelHashHeaderInfo) + sizeof( FirstLevelHashPiceStruct ) * ( number_of_pices ) );
    if ( firstLevelHashHeaderInfo == NULL){
        LOGMESG(LOG_ERROR, "Malloc");
        return NULL;
    }
    firstLevelHashHeaderInfo->pices = &firstLevelHashHeaderInfo[1];
    firstLevelHashHeaderInfo->numberOfPices = number_of_pices;
    firstLevelHashHeaderInfo->numberOfUsedPices = 0;
    for ( i = 0; i < number_of_pices; i++){
        firstLevelHashHeaderInfo->pices[i].LastLevelHashHeaderInfo = NULL;
    }
    return firstLevelHashHeaderInfo;
}*/




