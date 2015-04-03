#ifndef _ARCHIVATOR_
#define _ARCHIVATOR_

#include "log.h"
#include "data_presentation.h"
#include "sorted_mesg_stack.h"
#include "scaner.h"
#include "archivator.h"

#define FORA_IN_CHUNKS 20
#define NUMBER_OF_WORKERS 7
#define NUMBER_OF_COMPRESSORS 6
#define MAX_FILE_LENGTH 10000000000llu
#define TYPE_WORKER 1 
#define MAXIMAL_STACK_OF_DIRECTORIES 50

#define MESG_SEQ_KEY 3
#define SEMAFORS_KEY 7
#define S_TAKE_NEXT_FILE        0
#define S_ADD_FILE_TO_FL        1
#define S_WRITE_TO_OUT          2 



#endif //_ARCHIVATOR_
