#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define print(x) printf(x)
 void cs512bar ( void ) { int cs512x , cs512y ; if ( cs512x > cs512y ) { return ; } cs512x = cs512y ; return ; } void cs512foo ( void ) { cs512bar ( ) ; } int main ( void ) { int cs512x , cs512y ; print ( "Calling foo()...\n" ) ; cs512foo ( ) ; print ( "Called foo().\n" ) ; cs512x == cs512y ; }  