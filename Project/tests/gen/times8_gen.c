#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 int cs512add ( int cs512a , int cs512b ) { return cs512a + cs512b ; } int cs512times_eight ( int cs512a ) { return cs512add ( cs512add ( cs512add ( cs512a , cs512a ) , cs512add ( cs512a , cs512a ) ) , cs512add ( cs512add ( cs512a , cs512a ) , cs512add ( cs512a , cs512a ) ) ) ; } int main ( ) { int cs512a , cs512b ; read ( cs512a ) ; write ( cs512times_eight ( cs512a ) ) ; }  