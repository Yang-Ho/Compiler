#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 void cs512foo ( int cs512m , int cs512n ) { cs512m = cs512m + cs512n ; cs512n = cs512n + cs512m ; } int main ( ) { int cs512a ; read ( cs512a ) ; cs512foo ( cs512a , cs512a ) ; write ( cs512a ) ; }  