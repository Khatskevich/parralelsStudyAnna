void sort( int* a , int* b )

{
    int i;
    int temp2 ;
    int j = 0 ;
    int N= b[0] ;
    int z[ 1000 ] = {0} ;
    int flag = 0 ;
    int y ;
    int temp ;
    int x ;
    j = N/2 - 1;
    for (i= N / 2 - 1;i>= 0;i-- )
    {
        x =i;
        flag = 0 ;
        int l = x * 2 + 1;
        while ( (l< N ) && j <= N && flag == 0 )
        {
            if ( l == N -1 )
            {
                int k = 0;
                for ( k = 0 ; k < N ; k ++ )
                {
                    z[ k ] = N ;
                }
                y =l;
            }
            else if ( a[l] > a[l+ 1 ] ) y = l;
            else y = l + 1;
            if ( a[ x ] < a[ y ] )
            {
                if (i< 1000 )
                {
                    temp = a[ x ] ;
                    a[ x ] = a[ y ] ;
                    a[ y ] = temp ;
                    x = y ;
                   l= 2 * x + 1;
                }
                else
                {
                    break ;
                }
            }
            else
            {
                flag = 1 ;
            }
        }
        j -- ;
    }

    i = N-1;
   while (i>= 1 )
    {
        temp2 = a[ 0] ;
        a[ 0] = a[i] ;
        a[i] = temp2 ;
        flag = 0 ;
        x = 0;
        int l = 0;
        while ( l  <i -1 && flag == 0 )
        {
            if ( l  ==i- 2 ) y = i - 1;
            else if ( a[ l + 1 ] > a[ l + 2 ] ) y = l + 1;
            else y = l + 2;
            if ( a[ x ] < a[ y] )
            {
                temp = a[ x ] ;
                a[ x ] = a[ y ] ;
                a[ y ] = temp ;
                x = y ;
                l = x * 2;
            }
            else
            {
                flag = 1 ;
            }
        }
       i-- ;
    }
}
