#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 int cs512recursionsum ( int cs512n ) { if ( cs512n == 0 ) { return 0 ; } return cs512n + cs512recursionsum ( cs512n - 1 ) ; } int main ( ) { int cs512a ; read ( cs512a ) ; write ( cs512recursionsum ( cs512a ) ) ; }  