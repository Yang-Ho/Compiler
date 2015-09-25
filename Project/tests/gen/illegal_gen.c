#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define print(x) printf(x)
 int cs512array_1 [ 4 ] ; int cs512array_2 [ 4 ] ; void cs512populate_arrays ( void ) { cs512array_1 [ 0 ] = 0 ; cs512array_1 [ 1 ] = 1 ; cs512array_1 [ 2 ] = 1 ; cs512array_1 [ 3 ] = 2 ; cs512array_2 [ 0 ] = 3 ; cs512array_2 [ 1 ] = 5 ; cs512array_2 [ 2 ] = 8 ; cs512array_2 [ 3 ] = 13 ; } int main ( void ) { int cs512idx , cs512bound ; cs512populate_arrays ( ) ; cs512idx = 0 ; cs512bound = 8 ; print ( "The first few digits of the Fibonacci sequence are:\n" ) ; while ( cs512idx < cs512bound ) { write ( cs512array_1 [ cs512idx ] ) ; cs512idx = cs512idx + 1 ; } }  