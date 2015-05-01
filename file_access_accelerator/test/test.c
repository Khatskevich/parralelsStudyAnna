#include "faa.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> 
#include <assert.h> 
#include <errno.h>
#include <sys/mman.h>

#define TEST_FILE_NAME_1 "test_file_1_temp"
#define TEST_FILE_NAME_2 "test_file_2_temp"
#define TEST_FILE_NAME_3 "test_file_3_temp"
#define TEST_FILE_NAME_4 "test_file_4_temp"
#define TEST_FILE_SIZE_1 100l
#define TEST_FILE_SIZE_2 10000l
#define TEST_FILE_SIZE_3 1000000l
#define TEST_FILE_SIZE_4 5000000000l
#define STANDARD_BLOCK_SIZE 4096
#define STANDARD_CACHE_SIZE 1000000000
#define STANDARD_COMPRESSION_LEVEL 1

int create_file_for_tests(char* name, size_t size);
int create_files_for_tests();
    
int main( int argc, char** argv ){
    int rc;
    /*struct rusage usage_start;
    struct rusage usage_end;
    int rc;
    rc = getrusage( RUSAGE_SELF , &usage_start);
    long long int i;
    printf("Time %lld\n", usage_start.ru_utime.tv_usec );*/
    printf("Test started!\n");
    rc = create_files_for_tests();
    rc = test_load_full_file(TEST_FILE_NAME_3);
    size_t operations_number = 1000000;
    rc = test_random_access(TEST_FILE_NAME_3, operations_number );
    rc = test_wrong(TEST_FILE_NAME_3);
    rc = test_consistency(TEST_FILE_NAME_3);
    printf("Test passed!\n");
    return 0;
}

int test_consistency( char * name ){
    printf("Test consistency\n");
    int rc;
    size_t i;
    size_t file_size;
    rc = faa_init(name, STANDARD_BLOCK_SIZE, STANDARD_CACHE_SIZE, STANDARD_COMPRESSION_LEVEL,  &file_size );
    if ( rc < 0){
        return rc;
    }
    for( i = 0; i < file_size - STANDARD_BLOCK_SIZE; i+=STANDARD_BLOCK_SIZE ){
        char *block = faa_inc_ref(i);
        size_t j;
        for(j = 0; j < STANDARD_BLOCK_SIZE; j ++){
            if ( abs(block[j]) != (j+ i*STANDARD_BLOCK_SIZE)%10 ){
                faa_dec_ref(i);
                printf("File is unconsistent: # %llu = %d\n", j+ i*STANDARD_BLOCK_SIZE, (int)  block[j] );
                fflush( stdout );
                goto test_consistency_exit_error_0;
            }
        }
        faa_dec_ref(i);
    }
    
    return faa_deinit();
    test_consistency_exit_error_0:
        faa_deinit();
        return -1;
}

int test_load_full_file( char * name ){
    printf("Test load full file\n");
    int rc;
    size_t i;
    size_t file_size;
    rc = faa_init(name, STANDARD_BLOCK_SIZE, STANDARD_CACHE_SIZE, STANDARD_COMPRESSION_LEVEL,  &file_size );
    assert( rc >= 0);
    for( i = 0; i < file_size ; i+=STANDARD_BLOCK_SIZE ){
        faa_inc_ref(i);
    }
    return faa_deinit();
}

size_t get_random_value(){
    return (size_t) ((long long)rand() << 32) | rand();
}

int test_random_access( char * name , size_t operations_number ){
    printf("Test random access\n");
    int rc;
    size_t i;
    size_t file_size;
    size_t * allocations;
    allocations = (size_t*) malloc( sizeof( size_t) * operations_number);
    
    rc = faa_init(name, STANDARD_BLOCK_SIZE, STANDARD_CACHE_SIZE, STANDARD_COMPRESSION_LEVEL,  &file_size );
    assert( rc == 0);
    for( i = 0; i < operations_number ; i++ ){
        size_t next_offset = get_random_value() % (file_size / STANDARD_BLOCK_SIZE );
        allocations[i] = next_offset;
        void* ptr = faa_inc_ref(next_offset);
        if ( ptr == NULL ){
            goto test_random_access_exit_error_0;
        }
    }
    for( i = 0; i < operations_number ; i++ ){
        rc = faa_deinit(allocations[i]);
        if ( rc < 0){
            goto test_random_access_exit_error_0;
        }
    }
    return faa_deinit();
    test_random_access_exit_error_0:
        faa_deinit();
        return -1;
}

int test_wrong( char * name ){
    printf("Test wrong\n");
    int rc;
    void * ptr;
    ptr = faa_inc_ref(100500);
    assert( ptr == NULL);
    rc = faa_deinit();
    assert( rc < 0);
    rc = faa_flush(100500);
    assert( rc < 0);
    rc = faa_flush(FAA_FLUSH_ALL);
    assert( rc < 0);
    rc = faa_dec_ref(100500);
    assert( rc < 0);
    size_t i;
    size_t file_size;
    rc = faa_init(name, STANDARD_BLOCK_SIZE, STANDARD_CACHE_SIZE, STANDARD_COMPRESSION_LEVEL,  &file_size );
    ptr = faa_inc_ref(file_size + 1);
    assert( ptr == NULL );
    rc = faa_dec_ref(file_size + 1);
    assert( rc < 0);
    rc = faa_init(name, STANDARD_BLOCK_SIZE, STANDARD_CACHE_SIZE, STANDARD_COMPRESSION_LEVEL,  &file_size );
    assert( rc < 0);
    rc = faa_deinit();
    return 0;
}



int create_files_for_tests(){
    printf("Creating files:\n");
    create_file_for_tests( TEST_FILE_NAME_1, TEST_FILE_SIZE_1 );
    create_file_for_tests( TEST_FILE_NAME_2, TEST_FILE_SIZE_2 );
    create_file_for_tests( TEST_FILE_NAME_3, TEST_FILE_SIZE_3 );
    //create_file_for_tests( TEST_FILE_NAME_4, TEST_FILE_SIZE_4 );
    return 0;
}
int create_file_for_tests(char* name, size_t size){
    printf("Creating file %s, of size = %lld\n", name, (long long int) size );
    size_t i;
    int rc;
    int fd;
    void* mmap_start;
    fd = open( name, O_CREAT | O_RDWR, 0666);
    assert( fd > 0 );
    rc = ftruncate( fd, size);
    assert( rc == 0 );
    mmap_start = mmap(NULL, size, PROT_READ | PROT_WRITE ,
                    MAP_SHARED, fd, 0);
    assert(mmap_start != NULL );
    char *file_bytes = (char*) mmap_start;
    for ( i = 0; i < size; i++){
        file_bytes[i] = (char) (i% 10);
    }
    close( fd);
    munmap( mmap_start, size );
    return 0;
}