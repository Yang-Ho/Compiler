#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 int cs512max ( int cs512a , int cs512b ) { if ( cs512a > cs512b ) { return cs512a ; } return cs512b ; } int main ( ) { int cs512a , cs512b ; read ( cs512a ) ; read ( cs512b ) ; write ( cs512max ( cs512a , cs512b ) ) ; }  