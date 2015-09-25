#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 int main ( ) { int cs512a , cs512sum ; read ( cs512a ) ; cs512sum = 0 ; while ( cs512a > 0 ) { cs512sum = cs512sum + cs512a ; cs512a = cs512a - 1 ; } write ( cs512sum ) ; }  