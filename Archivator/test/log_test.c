#define DEBUG
#include "log.h"

int main(){
    logInit(5, NULL);
    LOGMESG(5,"%d, %d\n", 5, 74);
    return 0;
}
