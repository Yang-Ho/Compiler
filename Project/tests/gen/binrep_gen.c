#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define print(x) printf(x)
 void cs512recursedigit ( int cs512n ) { int cs512on ; if ( 0 == cs512n ) { return ; } cs512on = 0 ; if ( 0 != ( cs512n - ( ( cs512n / 2 ) * 2 ) ) ) { cs512on = 1 ; } cs512recursedigit ( cs512n / 2 ) ; if ( 0 == cs512on ) { print ( "0" ) ; } if ( 1 == cs512on ) { print ( "1" ) ; } } int main ( ) { int cs512a ; cs512a = 0 ; while ( 0 >= cs512a ) { print ( "Give me a number: " ) ; read ( cs512a ) ; if ( 0 >= cs512a ) { print ( "I need a positive integer.\n" ) ; } } print ( "The binary representation of: " ) ; write ( cs512a ) ; print ( "is: " ) ; cs512recursedigit ( cs512a ) ; print ( "\n\n" ) ; }  