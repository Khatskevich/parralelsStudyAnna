#define DEBUG
#define LOG_GROUP "LOG_TEST:"
#include "log.h"

int main(){
    logInit(LOG_INFO, NULL);
    LOGMESG(LOG_INFO,"%d, %d\n", 5, 74);
    LOGMESG(LOG_ALL,"%d, %d\n", 5, 84);
    LOGMESG(LOG_FATAL,"Valera!!! %d,\n", 5);
    logClose();
    logInit(LOG_INFO, "log_test_file_1_Valera" );
    LOGMESG(LOG_INFO,"%d, %d\n", 5, 74);
    LOGMESG(LOG_ALL,"%d, %d\n", 5, 84);
    LOGMESG(LOG_FATAL,"Valera!!! %d,\n", 5);
    logClose();
    return 0;
}
