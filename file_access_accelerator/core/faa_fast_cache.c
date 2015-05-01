#include <stdlib.h>
#include "faa_internal.h"
#include <faa_data_structure.h>
#include <string.h>

#define FIRST_ELEMENT_IN_LIST -1ull
#define FAST_CACHE_END_OF_SPACE -1ull


typedef struct PiceInList{
    size_t anumber;
    struct PiceInList* prev;
    struct PiceInList* next;
}PiceInList;

typedef struct{
    int data_is_compressed;
    int priority;
    size_t size;
    PiceInList* lpice;
    void* data;
    void* buffer_for_flushing;
    size_t offset;
}PiceInArray;

typedef struct FreeSpaceStack{
    struct FreeSpaceStack* prev;
    size_t anumber;
} FreeSpaceStack;


typedef struct {
    size_t block_size;
    PiceInArray* apices;
    PiceInList* lpices_by_priorities;
    FreeSpaceStack* freSpaceLastElement;
    size_t number_of_places;
    int compression_level;
    size_t filesize;
} FastCacheMainStruct;

FastCacheMainStruct fastCacheMainInfo;






size_t fast_cache_get_free_apice_number();
int fast_cache_release_free_apice_number( size_t anumber );
int fast_cache_delete_lpice( PiceInList* lpice );
int fast_cache_prepare_free_space();
PiceInList* fast_cache_add_lpice(int priority);





int fast_cache_init( size_t block_size, size_t cache_size, int compression_level, size_t filesize ){
    
    size_t i;
    if ( (block_size == 0) || (cache_size<block_size) || ( compression_level < 0 || compression_level > 7 ) ){
        LOGMESG( LOG_ERROR, "Invalid arguments" );
        goto fast_cache_init_exit_error_0;
    }
    
    fastCacheMainInfo.compression_level = compression_level;
    fastCacheMainInfo.block_size = block_size;
    fastCacheMainInfo.filesize = filesize;
    
    fastCacheMainInfo.lpices_by_priorities = (PiceInList*) malloc( sizeof(PiceInList)* MAXIMAL_NUMBER_OF_PRIORITIES );
    if ( fastCacheMainInfo.lpices_by_priorities == NULL){
        LOGMESG( LOG_ERROR, "Error while malloc" );
        goto fast_cache_init_exit_error_0;
    }
    
    for( i = 0 ; i < MAXIMAL_NUMBER_OF_PRIORITIES; i++){
        fastCacheMainInfo.lpices_by_priorities[i].anumber = FIRST_ELEMENT_IN_LIST;
        fastCacheMainInfo.lpices_by_priorities[i].next = NULL;
        fastCacheMainInfo.lpices_by_priorities[i].prev = &fastCacheMainInfo.lpices_by_priorities[i];
    }
    
    fastCacheMainInfo.number_of_places = cache_size/block_size;
    fastCacheMainInfo.apices = (PiceInArray*) malloc( sizeof( PiceInArray)*fastCacheMainInfo.number_of_places );
    if ( fastCacheMainInfo.apices == NULL){
        LOGMESG( LOG_ERROR, "Error while malloc" );
        goto fast_cache_init_exit_error_1;
    }
    
    for( i = 0 ; i < fastCacheMainInfo.number_of_places; i++){
        if ( i == 0){
            fastCacheMainInfo.freSpaceLastElement = (FreeSpaceStack*) malloc(sizeof(FreeSpaceStack));
            fastCacheMainInfo.freSpaceLastElement->prev = NULL;
            fastCacheMainInfo.freSpaceLastElement->anumber = i;
        }else{
            FreeSpaceStack* temp = fastCacheMainInfo.freSpaceLastElement;
            fastCacheMainInfo.freSpaceLastElement = (FreeSpaceStack*) malloc(sizeof(FreeSpaceStack));
            fastCacheMainInfo.freSpaceLastElement->prev = temp;
            fastCacheMainInfo.freSpaceLastElement->anumber = i;
        }
        if (fastCacheMainInfo.freSpaceLastElement == NULL ){
            LOGMESG( LOG_ERROR, "Error while malloc" );
            goto fast_cache_init_exit_error_1;
        }
    }    
    fast_cache_init_exit_ok:
        return 0;
    fast_cache_init_exit_error_2:
        free(fastCacheMainInfo.apices);
    fast_cache_init_exit_error_1:
        free(fastCacheMainInfo.lpices_by_priorities);
    fast_cache_init_exit_error_0:
        return -1;
}

int fast_cache_deinit(){
    size_t i;
    if ( fastCacheMainInfo.lpices_by_priorities != NULL){
        for( i = 0 ; i < MAXIMAL_NUMBER_OF_PRIORITIES; i++){
            if (fastCacheMainInfo.lpices_by_priorities[i].next != NULL){
                PiceInList* next = fastCacheMainInfo.lpices_by_priorities[i].next; 
                while ( next != NULL){
                    PiceInList * temp_next;
                    temp_next = next;
                    next = next->next;
                    free( temp_next );
                }
            }
        }
        free(fastCacheMainInfo.lpices_by_priorities); 
        fastCacheMainInfo.lpices_by_priorities = NULL;
    }

    fastCacheMainInfo.apices = (PiceInArray*) malloc( sizeof( PiceInArray)*fastCacheMainInfo.number_of_places );
    if ( fastCacheMainInfo.apices != NULL){
        for ( i == 0; i < fastCacheMainInfo.number_of_places; i++){
            if ( fastCacheMainInfo.apices[i].data != NULL ){
                free(fastCacheMainInfo.apices[i].data);
            }
        }
        free( fastCacheMainInfo.apices );
        fastCacheMainInfo.apices = NULL;
    }
    
    FreeSpaceStack* lastElement = fastCacheMainInfo.freSpaceLastElement;
    while ( lastElement != NULL){
        FreeSpaceStack* temp_lastElement;
        temp_lastElement = lastElement;
        lastElement = lastElement->prev;
        free( temp_lastElement);
    }
    return 0;
}


int fast_store_data_to_cache( void** data, int priority, FaaDataStruct* faaDataInfo ){
    int rc;
    rc = fast_cache_prepare_free_space();
    size_t anumber = fast_cache_get_free_apice_number();
    PiceInArray* apice = &fastCacheMainInfo.apices[anumber];
    PiceInList* lpice = fast_cache_add_lpice(priority);
    lpice->anumber = anumber;
    apice->lpice=lpice;
    apice->priority=priority;
    apice->data_is_compressed = 1;
    apice->buffer_for_flushing = faaDataInfo->data;
    apice->offset = faaDataInfo->offset;
    void* temp_data = cache_compress(*data, &(apice->size) ,fastCacheMainInfo.block_size, fastCacheMainInfo.compression_level); // need to be compressed
    free(*data);
    *data = apice;
    apice->data = temp_data;
    return 0;
}
int fast_get_data_from_cache( void** data ){
    int rc;
    PiceInArray* apice = (PiceInArray*)(*data);
    size_t anumber = apice->lpice->anumber;
    rc = fast_cache_delete_lpice( apice->lpice );
    *data = cache_decomperess(apice->data, apice->size, fastCacheMainInfo.block_size, fastCacheMainInfo.compression_level );
    free( apice->data );
    fast_cache_release_free_apice_number(anumber);
    return 0;
}
int fast_remove_data_from_cache( void** data ){
    int rc;
    PiceInArray* apice = (PiceInArray*)(*data);
    size_t anumber = apice->lpice->anumber;
    rc = fast_cache_delete_lpice( apice->lpice );
    free( apice->data );
    fast_cache_release_free_apice_number(anumber);
    FaaDataStruct faaDataInfo;
    faaDataInfo.data = DATA_FLUSHING_FROM_CACHE;
    faaDataInfo.offset = apice->offset;
    //faa_data_dec_ref( &faaDataInfo );
    return 0;
}

int fast_flush_data_from_cache( void** data, FaaDataStruct* faaDataInfo ){
    int rc;
    PiceInArray* apice = (PiceInArray*)(*data);
    size_t anumber = apice->lpice->anumber;
    void* temp_data = cache_decomperess(apice->data, apice->size ,fastCacheMainInfo.block_size, fastCacheMainInfo.compression_level);
    memcpy( faaDataInfo->data ,temp_data,fastCacheMainInfo.block_size );
    free( temp_data );
    fast_cache_release_free_apice_number(anumber);
    return 0;
}


size_t fast_cache_get_free_apice_number(){
    FreeSpaceStack* lastElement = fastCacheMainInfo.freSpaceLastElement;
    if ( lastElement == NULL){
        return FAST_CACHE_END_OF_SPACE;
    }
    fastCacheMainInfo.freSpaceLastElement = lastElement->prev;
    size_t anumber = lastElement->anumber;
    free( lastElement );
    return anumber;
}
int fast_cache_release_free_apice_number( size_t anumber ){
    FreeSpaceStack* temp = (FreeSpaceStack*) malloc( sizeof(FreeSpaceStack));
    temp->prev = fastCacheMainInfo.freSpaceLastElement;
    fastCacheMainInfo.freSpaceLastElement = temp;
    fastCacheMainInfo.freSpaceLastElement->anumber = anumber;
    return 0;
}

PiceInList* fast_cache_add_lpice(int priority){
    if ( priority > MAXIMAL_NUMBER_OF_PRIORITIES){
        priority = MAXIMAL_NUMBER_OF_PRIORITIES-1;
    }
    PiceInList* list = &fastCacheMainInfo.lpices_by_priorities[priority];
    PiceInList* temp = ( PiceInList*) malloc( sizeof(PiceInList));
    temp->prev= list->prev;
    temp->next = list;
    list->prev = temp;
    if ( list->next == NULL){
       list->next = temp;
    }
    return temp;
}

int fast_cache_delete_lpice( PiceInList* lpice ){
    lpice->prev->next = lpice->next;
    lpice->next->prev = lpice->prev;
    free( lpice );
    return 0;
}

int fast_cache_prepare_free_space(){
    if ( fastCacheMainInfo.freSpaceLastElement != NULL ){
        return 0;
    }
    size_t i;
    PiceInList* lists = fastCacheMainInfo.lpices_by_priorities;
    for( i = 0; i<MAXIMAL_NUMBER_OF_PRIORITIES; i++ ){
        if ( lists[i].prev != &lists[i]  ){
            break;
        }
    }
    if ( i == MAXIMAL_NUMBER_OF_PRIORITIES ){
        return -1;
    }
    PiceInList* lpice = lists[i].prev;
    PiceInArray* apice = &fastCacheMainInfo.apices[lpice->anumber];
    FaaDataStruct faaDataInfo;
    faaDataInfo.data = apice->buffer_for_flushing;
    faa_flush_data_from_cache( (void**)&apice , &faaDataInfo );
    fast_remove_data_from_cache( (void**)&apice );
    return 0;
}
