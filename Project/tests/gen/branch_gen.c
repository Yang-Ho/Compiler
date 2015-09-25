#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 int main ( ) { int cs512a , cs512b ; read ( cs512a ) ; read ( cs512b ) ; if ( cs512a >= cs512b ) { write ( cs512a ) ; } if ( cs512b > cs512a ) { write ( cs512b ) ; } }  