#ifndef _BIZZ_BAZZ_H_
#define _BIZZ_BAZZ_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <limits.h>

struct bb_info{
    char* temp_string;          // Buffer for some object needs.
    char separators[257];       // String with any of valid separators.
                                //  You can put it into bb_create.
    char last_separator;
    int temp_string_len;
    int temp_string_max_len;
    ssize_t (*write)(char*, size_t);    // Std out is using for output by default.
                                        // You can put your own method to bb_create.
                                        // It must WRITE buf of size anyhow and return 0
                                        // if all is right.
};

struct bb_info* bb_create( char* separators_ , ssize_t (*default_write)(char*, size_t) );
void bb_destroy( struct bb_info* info);

    // Bytes of compressing string
    // going here one by one.
    // Output is generating automaticly with (*write) func
    // as fast as it becomes clear what to print.
int bb_give_byte( char byte, struct bb_info* info);


#endif //_BIZZ_BAZZ_H_

