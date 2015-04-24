//#include "faa.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>

int main( int argc, char** argv ){
    struct rusage usage_start;
    struct rusage usage_end;
    int rc;
    rc = getrusage( RUSAGE_SELF , &usage_start);
    long long int i;
    for( i = 0; i<100000000; i++){}
    printf("Time %lld\n", usage_start.ru_utime.tv_usec );
    return 0;
}
