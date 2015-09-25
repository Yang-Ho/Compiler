#include <stdio.h>
 #define read(x) scanf("%d",&x)
 #define write(x) printf("%d\n",x)
 #define print(x) printf(x)
 int cs512g ( ) { return 1 ; } int cs512f ( ) { return cs512g ( ) + 1 ; } int cs512e ( ) { return cs512f ( ) + 1 ; } int cs512d ( ) { return cs512e ( ) + 1 ; } int cs512c ( ) { return cs512d ( ) + 1 ; } int cs512b ( ) { return cs512c ( ) + 1 ; } int cs512a ( ) { return cs512b ( ) + 1 ; } int main ( ) { int cs512val ; cs512val = cs512a ( ) ; print ( "I calculate the answer to be: " ) ; write ( cs512val ) ; }  