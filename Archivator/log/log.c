#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>

#include "log.h"

LOGMAININFO logMainInfo;

void * threadWriter( void* param){
    char buf[WRITER_ATOM_SIZE];
    ssize_t len;
    ssize_t writtenlen;
    while( len = read( logMainInfo.readBufDes, buf, WRITER_ATOM_SIZE )){
        writtenlen = write( logMainInfo.logDes, buf, len );
        if( writtenlen != len );
    }
}

int logInit(unsigned logLevel, const char * filename){
    int des;
    int rc;
    int pipefd[2];
    
    if ( logLevel >= LOG_LEVELS_COUNT || logLevel < 0 ){
        goto log_exit_0;
    }

    if (filename == NULL){
        des = 2; // stderr
    } else{
        des = open(filename, O_APPEND | O_CREAT | O_WRONLY, 0666);
        if ( des == -1 )
            goto log_exit_0;
    }
    rc = pipe( pipefd );
    if ( rc == -1 ){
        goto log_exit_1;
    }
    rc = pthread_create(&logMainInfo.writerThreadId, NULL, &threadWriter, NULL);
    if ( rc !=0 ){
        goto log_exit_2;
    }
    logMainInfo.readBufDes = pipefd[0] ; 
    logMainInfo.writeBufDes = pipefd[1] ;
    logMainInfo.logLevel = logLevel ; 
    logMainInfo.logDes = des ; 
    
    return 0;

    log_exit_2:
        close( pipefd[0]);
        close( pipefd[1]);
    log_exit_1:
        close( des);
    log_exit_0:
        return -1;
}   

int logMesg( char* group, int priority ,const char* str,...)
{
    char buf[MAX_MESG_SIZE];
    int len_group;
    int len_mesg; 
    ssize_t len;
    va_list argptr;
    va_start(argptr, str);
    len_group = snprintf( buf, MAX_MESG_SIZE-1, "%s", group); 
    len_mesg = vsnprintf( buf+len_group, MAX_MESG_SIZE-len_group-1 , str ,argptr);
    len = write( logMainInfo.writeBufDes, buf, len_group + len_mesg);
    if ( len != len_group + len_mesg );
    return 0;
}
