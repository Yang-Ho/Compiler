#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define binary int
 #define decimal int
 void cs512print_two ( int cs512a , int cs512b ) { write ( cs512a ) ; write ( cs512b ) ; } int main ( ) { binary cs512b ; decimal cs512a ; read ( cs512a ) ; read ( cs512b ) ; cs512print_two ( cs512a , cs512b ) ; cs512print_two ( cs512b , cs512a ) ; }  