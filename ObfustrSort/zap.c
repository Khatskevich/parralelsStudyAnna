#include <stdio.h>
#include <stdlib.h>

#define proisxodit tut

 main()
{
    printf("16\n");
    int mass[] = {4, 67,23,64,34,5, -1 , 24 , 5 , -6, 76, 23,765,34,1,856,34,85,3,1,85,3,55,-546,45};
    int i;
    sort(mass, 25);    
for ( i =0; i<25;i++)
printf("%d ", mass[i]);
printf("\n");
}

void ShellsSort2(char** A)
{
	unsigned i,j,k,x,y;
	int t;
	for(x =0; x<= (*A); )
        {for(i = ((int*)A[8])[x]; i < (*A); i++)
        {
            t = ((int*)A[4])[i];
            for( y = A[2]; y > ((int*)A[8])[x]; y--){
                int temp =0;
                if ( ((int*) A[4])[y] > ((int*) A[4])[y+1]) { temp = ((int*) A[4])[y]; ((int*) A[4])[y] = ((int*) A[ 4])[y+1]; ((int*) A[4 ])[y+1] = temp;
            }}
            for(j = i; j>= ((int*)A[8])[x]; j-= ((int*)A[8])[x])
            {
                if(t < ( ( int*)A[4] )[ j- ((int*)A[ 8 ])[ x]])
                   ((int*) A[4])[j] = ((int*)A[4 ])[j- ((int*) A[8])[x]];
                else
                    break;
            }
            ((int*)A[4])[j] = t;
        }
        if( (i , !((int*)A[8])[++x] ))
 break;
        }
}


int compar(const void *a, const void *b){
return *((int*)a)-*((int*)b);
}

int getD(long long int* A, unsigned N){
    int i=0, j=2,k;
	A[1<<2] = A[2<<2];
    int mass1[1000];
	for(k = N/2; k > 0; k /=2)
        {mass1[i++]=k; }
        mass1[i]=0;
    A[1<<++j] =  malloc(sizeof(int)*(++i));
    k = --i;
    for(; i>=0; )
    { ((int*)A[8])[k-i]= mass1[k-i--];}
    return ++k;
}

#define proishodit 100
#define tut chto
#define chto poimi
#define poimi fig
#define fig (temp + i)

void sort(int* A, unsigned N){
    char * mass1[1000];
    int i = 3;
    mass1[8] = A;
    int temp = 0;
int *a ;
    mass1[(temp = getD( mass1, N) ,  fig  , poimi , chto , tut , proisxodit  , (temp)/15 )] = N;
    a=(int*)(mass1[8]); 
    for( i = 0; i<temp; i++){ printf("%d ", a[i]);} printf("\n");   
    mass1[2] = N /( *((int*)mass1[8]) <<2 );
    printf("%d<<", (int) mass1[2]);
    ShellsSort2(mass1);
    free(mass1[1<<3]); 
}

