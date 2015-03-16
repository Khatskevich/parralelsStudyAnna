#ifndef _ARCHIVATOR_LOG_
#define _ARCHIVATOR_LOG_


#ifdef DEBUG
#define LOG_GROUP ""
#define LOGMESG(a, ...) logMesg( LOG_GROUP, a, ##__VA_ARGS__ )
#else
#define LOG_GROUP ""
#define LOGMESG(a) do{}while(0)
#endif //DEBUG

#include <pthread.h>


#define MAX_MESG_SIZE 1024
#define WRITER_ATOM_SIZE MAX_MESG_SIZE

enum LOGLEVELS{
    LOG_ALL = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_LEVELS_COUNT
};

typedef struct
{
    int isStarted ;
    int logDes;
    unsigned flags;
    int logLevel;
    int writeBufDes;
    int readBufDes;
    pthread_t writerThreadId;
} LOGMAININFO;


int logInit(unsigned logLevel, const char * filename);

int logMesg( char* group, int priority ,const char* str,...);

#endif // _ARCHIVATOR_LOG_
