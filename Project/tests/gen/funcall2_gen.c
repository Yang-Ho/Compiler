#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define print(x) printf(x)
 int cs512c ( ) { return 1 ; } int cs512b ( ) { return 2 ; } int cs512a ( ) { return 3 ; } int cs512foo ( int cs512a , int cs512b , int cs512c ) { return ( cs512a * 3 + cs512b * 2 + cs512c ) ; } int main ( ) { int cs512val ; cs512val = cs512foo ( cs512a ( ) , cs512b ( ) , cs512c ( ) ) ; print ( "I calculate the answer to be: " ) ; write ( cs512val ) ; }  