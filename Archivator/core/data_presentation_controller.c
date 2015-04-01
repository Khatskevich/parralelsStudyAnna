#define DEBUG

#include "log.h"
#include "data_presentation.h"
#include "scaner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stddef.h>
#include <sys/mman.h>
#include "zlib.h"
#include <assert.h>
 
 
 
 
//#include <sys/msg.h>
//#include <sys/ipc.h>


#include <linux/ipc.h>
#include <linux/msg.h>

#define FORA_IN_CHUNKS 0
#define MAX_FILE_LENGTH 10000000000llu
#define TYPE_WORKER 1 
void* pthreadMainLoop(void*);
size_t writeToOut( char* data, size_t size );
char* takeNextFile(char** names);
size_t addFileToFL(int fd,char* nextFileName );

typedef struct{
    char* mmap_start;
    size_t mmap_size;
    size_t mmap_last_symbol;
    int nthread;
    int compression_lvl; // 1-7
    char** files;
    int fd_out;
    char* progname;
    int workers_qid;
} controllerMainStruct;

typedef struct{
    long controllerID;          /* Message type */
    int compression_lvl;
    char* data_decomp;
    size_t size_decomp;
    char* data_comp;
    size_t size_comp;
}mesgStruct;


static controllerMainStruct controllerMainInfo;
size_t fileNameIter;

int dataPresentationControllerInit(char* outFName,
                                   int compression_lvl, int nthread, char ** files, char* progname)
{
    
    int fd_out, rc;
    char* mmap_start;
    fileNameIter= 0;
    if ( nthread <= 0 ||  nthread > 100){ 
        LOGMESG(LOG_ERROR ,"Not valid nthread");
        goto dataPresentationControllerInit_exit_error_0;
    }
    controllerMainInfo.nthread = nthread;
    if ( compression_lvl <= 0 ||  compression_lvl > 7){ 
        LOGMESG(LOG_ERROR ,"Not valid compression_lvl");
        goto dataPresentationControllerInit_exit_error_0;
    }
    
    controllerMainInfo.compression_lvl = compression_lvl;
    if ( files == NULL){
        LOGMESG(LOG_ERROR ,"Not valid files struct");
        goto dataPresentationControllerInit_exit_error_0;
    }
    controllerMainInfo.files = files;
    if ( progname == NULL){
        LOGMESG(LOG_ERROR ,"Not valid progname");
        goto dataPresentationControllerInit_exit_error_0;
    }
    controllerMainInfo.progname = progname;
    fd_out = open( outFName, O_CREAT | O_RDWR, 0666);
    if ( fd_out < 0){
        LOGMESG(LOG_ERROR ,"Error while opening output file");
        goto dataPresentationControllerInit_exit_error_0; 
    }
    
    controllerMainInfo.fd_out = fd_out;
    if (ftruncate( fd_out, MAX_FILE_LENGTH) ){
        LOGMESG(LOG_ERROR ,"Error while ftruncate output file");
        goto dataPresentationControllerInit_exit_error_1;
    }
    controllerMainInfo.mmap_size = MAX_FILE_LENGTH;
    
    mmap_start = mmap(NULL, MAX_FILE_LENGTH, PROT_READ | PROT_WRITE ,
                    MAP_SHARED, fd_out, 0);
    if ( mmap_start == MAP_FAILED){
        LOGMESG(LOG_ERROR ,"Error while creating mmap");
        goto dataPresentationControllerInit_exit_error_1;
    }
    controllerMainInfo.mmap_start = mmap_start;
    controllerMainInfo.mmap_last_symbol = 0;
    
    int controllerID = 0;
    int qid;
    key_t msgkey = ftok(controllerMainInfo.progname, controllerID);
    if(( qid = msgget( msgkey, IPC_CREAT | 0666  )) == -1) {
                LOGMESG(LOG_ERROR,"open_queue for workers");
                goto dataPresentationControllerInit_exit_error_2;
    }
    controllerMainInfo.workers_qid = qid;
    
    fileList firstStructure;
    firstStructure.count = 0;
    size_t offset;
    offset = writeToOut( (char*) &firstStructure, sizeof(fileList));
    
    
    return 0;
    dataPresentationControllerInit_exit_error_2:
        munmap( controllerMainInfo.mmap_start, controllerMainInfo.mmap_size);
    dataPresentationControllerInit_exit_error_1:
        close(fd_out);
    dataPresentationControllerInit_exit_error_0:
        return -1;
}

int compressionPerform(){
    int rc;
    pthread_t writerThreadId;
    rc = pthread_create(&writerThreadId, (void*) NULL, pthreadMainLoop, (void*) NULL);
    int ID = 2;
    controllerCompress((void*) &ID);
    LOGMESG(LOG_INFO, "Closing messages");
    msgctl( controllerMainInfo.workers_qid, IPC_RMID, NULL); 
    munmap( controllerMainInfo.mmap_start, controllerMainInfo.mmap_size);
    ftruncate( controllerMainInfo.fd_out, controllerMainInfo.mmap_last_symbol);
    close( controllerMainInfo.fd_out);
}

void* pthreadMainLoop( void* params){
    while( threadDoCompression() == 0 ){};
    return ;
}

int threadDoCompression(){
    mesgStruct mesgInfo;
    if ( takeInfoMesg( &mesgInfo, TYPE_WORKER) ){
        return 1;
    }
    LOGMESG(LOG_INFO, "Compressing %d bytes for controller %d", (int) mesgInfo.size_decomp, mesgInfo.controllerID );
    compressChunk(&mesgInfo);
    mesgInfo.controllerID = 2; //?????????
    giveInfoMesg(&mesgInfo); // controller must free this memory
    return 0;
}

int controllerCompress(void* ID){
    int controllerID = *((int*) ID);
    char* nextFileName;
    while( 1)
    {
        nextFileName = takeNextFile(controllerMainInfo.files);
        if ( nextFileName == NULL) {
            return 0;
        }
        compressFileByChunks(controllerID , nextFileName);
        free(nextFileName);
    }
    
}

int compressFileByChunks(int controllerID, char* nextFileName){
    int fd;
    char* mmap_next_file;
    size_t length;
    size_t placeForNextChunkOffset;
    struct stat sb;
    
    fd = open( nextFileName , O_RDONLY);
    if ( fd == -1 ){
        LOGMESG(LOG_ERROR, "opening next file name");
        goto compressFileByChunks_exit_error_0;
    }
    fstat(fd, &sb);
    length = sb.st_size;
    mmap_next_file = mmap(NULL, length, PROT_READ ,
                    MAP_PRIVATE, fd, 0);
    if ( mmap_next_file == NULL){
        LOGMESG(LOG_ERROR ,"Error while creating mmap");
        goto compressFileByChunks_exit_error_1;
    }
    LOGMESG(LOG_INFO , "Offset to firstFileChunkOffset =%d", offsetof( fDescription, firstFileChunkOffset ) );
    placeForNextChunkOffset = addFileToFL(fd,nextFileName ) + offsetof( fDescription, firstFileChunkOffset );
    size_t sended = 0;
    size_t received = 0;
    size_t next_receiving = 0;
    size_t number_of_cicles = 0;
    while ( received*CHUNK < length ){
        if ( sended*CHUNK < length){
            mesgStruct mesgInfo;
            mesgInfo.size_decomp = (length - sended*CHUNK < CHUNK) ? length - sended*CHUNK : CHUNK;
            mesgInfo.data_decomp = mmap_next_file+ sended*CHUNK;
            mesgInfo.compression_lvl = controllerMainInfo.compression_lvl;
            mesgInfo.controllerID = TYPE_WORKER;
            LOGMESG(LOG_INFO, "Sending %d bytes", (int) mesgInfo.size_decomp );
            giveInfoMesg( &mesgInfo );
            sended++;
        }
        if ( number_of_cicles >= FORA_IN_CHUNKS ){// when number will be more ther 1 , you should care about order.
            mesgStruct mesgInfo;
            LOGMESG(LOG_INFO, "Receiving... ");
            takeInfoMesg(&mesgInfo , controllerID );
            LOGMESG(LOG_INFO, "Receiving %d bytes", (int) mesgInfo.size_comp );
            size_t offset = writeToOut(mesgInfo.data_comp,mesgInfo.size_comp);
            free(mesgInfo.data_comp);
            writeToOutByOffset( (char*) &offset, sizeof(size_t) ,placeForNextChunkOffset);
            LOGMESG(LOG_INFO , "placeForNextChunkOffset = %x", (int) placeForNextChunkOffset);
            placeForNextChunkOffset = offset + offsetof( fileChunk, nextFileChunk);
            LOGMESG(LOG_INFO , "placeForNextChunkOffset = %x", (int) placeForNextChunkOffset);
            received++;
        }
        number_of_cicles ++;
    }
    compressFileByChunks_exit_error_1:
    munmap( mmap_next_file, length);
    compressFileByChunks_exit_error_0:
        return -1;
}

int writeToOutByOffset(char* data, size_t size, size_t offset ){
    if ( offset + size > controllerMainInfo.mmap_size){
        return -1;
    }
    memcpy( controllerMainInfo.mmap_start + offset, data, size);
    return 0;
    
}

size_t writeToOut( char* data, size_t size ){
    LOGMESG(LOG_INFO, "Writing data to OUT  %c", data[0] );
    if (controllerMainInfo.mmap_size<controllerMainInfo.mmap_last_symbol+size){
        /*
        char *new_mapping = (char*) mremap((void*)controllerMainInfo.mmap_start, controllerMainInfo.mmap_size,
                                   controllerMainInfo.mmap_size + 10*1000*1000, MREMAP_MAYMOVE);
        if (new_mapping == MAP_FAILED){
            LOGMESG( LOG_ERROR, "mremap");
            return 0;
        }
        controllerMainInfo.mmap_start = new_mapping;*/
        LOGMESG(LOG_ERROR, "FILE BIGGER THAN IT WAS EXPECTED");
        //use int ftruncate(int fildes, off_t length);
    }
    if ( size >=17){
    LOGMESG( LOG_INFO ,"Compressing first two bytes  %X %X", (int)data[16], (int)data[17]);
    }
    size_t offset_to_new_data = controllerMainInfo.mmap_last_symbol;
    memcpy( controllerMainInfo.mmap_start + controllerMainInfo.mmap_last_symbol, data, size);
    //char *a =controllerMainInfo.mmap_start + controllerMainInfo.mmap_last_symbol;
    //a[0] = 'x';
    controllerMainInfo.mmap_last_symbol = controllerMainInfo.mmap_last_symbol + size;
    
    return offset_to_new_data;
}

 
char* takeNextFile(char** names){
    if ( names[fileNameIter] == NULL){
        return NULL;
    }
    char* new_name = (char*) malloc(sizeof(char) * (strlen(names[fileNameIter]) + 1 ));
    memcpy( new_name , names[fileNameIter], strlen(names[fileNameIter])+ 1);
    fileNameIter++;
    return new_name;
}


int takeInfoMesg( mesgStruct * mesgInfo , long controllerID ){
        int result, length;

        /* The length is essentially the size of the structure minus sizeof(mtype) */
        length = sizeof(mesgStruct) - sizeof(long);        
        if((result = msgrcv( controllerMainInfo.workers_qid , mesgInfo, length, controllerID ,  0)) == -1)
        {
            LOGMESG( LOG_INFO , "msgrcving by controllerID %d" ,controllerID);
            return(-1);
        }
        return 0;
}

int giveInfoMesg(mesgStruct * mesgInfo){// controller must free this memory
        int     result, length;
        /* The length is essentially the size of the structure minus sizeof(mtype) */
        length = sizeof(mesgStruct) - sizeof(long);        
        if((result = msgsnd( controllerMainInfo.workers_qid, mesgInfo, length, 0)) == -1)
        {
                return(-1);
        }
        return(result);
}

int compressChunk(mesgStruct * mesgInfo){
    size_t destLen = CHUNK*2 + 1000;
    fileChunk * data_comp = (fileChunk *) malloc( destLen - sizeof(fileChunk) );
    data_comp->nextFileChunk = 0;
    char * data = ((char*)data_comp) + sizeof(fileChunk);
    LOGMESG( LOG_INFO ,"Compressinf offset between polya = %d, sizeof(fileChunk) = %d", (int) (data-(char*)data_comp), sizeof(fileChunk) );
    mesgInfo->data_comp = (char*) data_comp;
    int ret, flush;
    size_t have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, mesgInfo->compression_lvl);
    size_t numberOfCompressedBytes = 0;

    if (ret != Z_OK)
        return ret;
    
    /* compress until end of file */
    do
    {
        size_t bytesLeft = mesgInfo->size_decomp;
        strm.avail_in = bytesLeft; 
        flush = Z_FINISH;
        strm.next_in = mesgInfo->data_decomp;
        LOGMESG( LOG_INFO, "ProcessingNextChunk");
        do
        {sleep(1);
            if ( destLen - numberOfCompressedBytes < 0){
                LOGMESG( LOG_ERROR, "Too small destbuf size");
            }
            size_t current_out_buf_size = (destLen - numberOfCompressedBytes);
            strm.avail_out = current_out_buf_size ;
            printf("bytesLeft = %d\n", (int) strm.avail_out);
            strm.next_out = data + numberOfCompressedBytes;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = current_out_buf_size - (size_t) strm.avail_out;
            printf("Have = %d\n", (int) have);
            numberOfCompressedBytes += have;
            //if (fwrite(out, 1, have, dest)
            //!= have || ferror(dest)) { (void)deflateEnd(&strm);return Z_ERRNO; }
        }
        while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    }
    while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    LOGMESG(LOG_INFO, "Compressing first two bytes  %X %X", (int)((char*)data_comp)[16], (int)((char*)data_comp)[17]);
    mesgInfo->size_comp = numberOfCompressedBytes+sizeof(fileChunk);
    data_comp->size_of_data = numberOfCompressedBytes +sizeof(fileChunk);
    return 0;
}
    
size_t addFileToFL(int fd,char* nextFileName ){
    struct stat stbuf;
    fDescription *fDesc = (fDescription*) malloc( sizeof(fDescription) + strlen(nextFileName)+1 );
    memcpy( ((char*)fDesc) + sizeof(fDescription) , nextFileName,  strlen(nextFileName)+1);
    if (fstat( fd, &stbuf) == -1){
    }
    fDesc->st_mode = stbuf.st_mode;
    fDesc->st_uid = stbuf.st_uid;
    fDesc->st_gid = stbuf.st_gid;
    fDesc->st_size = stbuf.st_size;
    //fDesc->st_atime = stbuf.st_atime;
    //fDesc->st_mtime = stbuf.st_mtime;
    //fDesc->st_ctime = stbuf.st_ctime;
    size_t offset = writeToOut( (char*) fDesc , sizeof(fDescription)+strlen(nextFileName)+1);
    fileList *FL =  ( fileList* )controllerMainInfo.mmap_start;
    FL->fDescriptionOffset[FL->count] = offset; // todo
    LOGMESG(LOG_INFO, "fileList files count = %d", (int)FL->count );
    FL->count = FL->count + 1;
    LOGMESG(LOG_INFO, "fileList files count = %d", (int)FL->count );
    return offset;
}
    
int dataRestore(char* filename){
    LOGMESG(LOG_INFO, "Data restore...");
    int fd;
    char* mmap_start;
    size_t length;
    size_t placeForNextChunkOffset;
    struct stat sb;
    size_t i;
    
    fd = open( filename , O_RDONLY);
    if ( fd == -1 ){
        LOGMESG(LOG_ERROR, "opening file");
    }
    fstat(fd, &sb);
    length = sb.st_size;
    mmap_start = (char*) mmap(NULL, length, PROT_READ ,
                    MAP_PRIVATE, fd, 0);
    if ( mmap_start == NULL){
        LOGMESG(LOG_ERROR ,"Error while creating mmap");
    }
    fileList * FL = (fileList *) mmap_start;
    LOGMESG(LOG_INFO, "fileList files count = %d", (int)FL->count );
    for( i = 0; i<FL->count; i++){
        restoreFile(mmap_start ,FL->fDescriptionOffset[i]);
    }
}

int restoreFile( char* mmap_start, size_t offset_to_file ){
    LOGMESG(LOG_INFO, "Restoring file...");
    fDescription * f_description = (fDescription *) (mmap_start + offset_to_file);
    int fd_out = open( "restored", O_CREAT | O_RDWR, 0666);
    fileChunk * file_chunk = (fileChunk *) (mmap_start + f_description->firstFileChunkOffset);
    
    while( 1 )
    {
        mesgStruct mesg;
        mesg.data_comp = (char*) file_chunk;
        if (restoreChunk(&mesg) == -1){
            LOGMESG(LOG_ERROR, "Error while restoring CHUNK...");
        }
        LOGMESG(LOG_INFO, "Restoring numberOfRestoredBytes = %d", (int) mesg.size_decomp);
        write( fd_out , mesg.data_decomp, mesg.size_decomp);
        free(mesg.data_decomp);
        if ( file_chunk->nextFileChunk == 0){
            break;
        }else{
            file_chunk = (fileChunk *) (mmap_start + file_chunk->nextFileChunk);
        }
    }
    close( fd_out);

    
    
    

    
}

int restoreChunk( mesgStruct* mesg){
    
    fileChunk * file_chunk = (fileChunk *)mesg->data_comp;
    char * data_decomp = (char*) malloc( CHUNK +1 ); // need to be free from outside
    char * data_comp = (char*) &file_chunk[1];
    size_t numberOfCompressedBytes = file_chunk->size_of_data - sizeof(fileChunk);
    size_t numberOfInflatedBytes = 0;
    mesg->data_decomp = data_decomp;
    LOGMESG(LOG_INFO, "Restoring numberOfCompressedBytes = %d", (int) numberOfCompressedBytes);
    LOGMESG(LOG_INFO, "Restoring offset between polya = %d, sizeof(fileChunk) = %d", (int) (data_comp -(char*)file_chunk), sizeof(fileChunk) );
    LOGMESG(LOG_INFO, "Restoring first two bytes  %X %X", (int)((char*)file_chunk)[16], (int)((char*)file_chunk)[17]);
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = numberOfCompressedBytes;
        strm.next_in = data_comp;
        if (strm.avail_in == 0)
            break;
        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK-numberOfInflatedBytes + 1;
            strm.next_out = data_decomp+numberOfInflatedBytes;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
                LOGMESG(LOG_ERROR, "Restoring Error");
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - numberOfInflatedBytes + 1- strm.avail_out;
            LOGMESG(LOG_INFO, "Restoring have = %d", (int) have);
            numberOfInflatedBytes = numberOfInflatedBytes + have;
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    
    mesg->size_decomp = numberOfInflatedBytes;
    return ret == Z_STREAM_END ? 0 : -1;

}