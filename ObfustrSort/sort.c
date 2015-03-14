#include <stdio.h>
#include <unistd.h>

int sorted( void * paket , ssize_t size, int count){
    int i = 0;
    int* mass =(int*) paket;
    for ( ;i < size-1 ; )
    {
        if ( mass[i] < mass[(i++)+1])
        //if ( *(int*)(paket+i) < *(int*) 
        //if ( *(((int*) paket)+i)) < *( ((int*)paket)+((i++) +1))
            return 1;
    }
    return 0;
}

int sortLesa( void * paket , ssize_t size, int count)
{
    int rrr1 = count + 5;
    int rrr2 = count + 4;
    void* temp = malloc( size);
    while( sorted( paket , size, count) ){
        rrr1 = ((rrr2+ rrr1*2)%count+5)% count;
        rrr2 = ((rrr2+ rrr1*3+11)%count+5)% count;
        memcpy( temp, paket+ size*rrr1, size);
        memcpy( paket+ size*rrr1, paket+size*rrr2, size);
        memcpy( paket+ size*rrr2 , temp , size);
    }
    free(temp);
}

int main()
{
    int mass[4] ={ 5,3,8,-1};
    sortLesa( mass, sizeof(int), 4);
    int i;
    for(i =0; i<4; i++)
    {
        printf( "%d ",mass[i]);
    }
    printf("\n");
}
