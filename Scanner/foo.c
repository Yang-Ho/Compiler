#include <stdio.h>
#define read(x) scanf("%d\n", &x)
#define write(x) printf("%d\n",x)

// function foo
void foo() {
    int a;
    read(a);
    write(a);
}

int main() {
    foo();
    int y = 5;
    int asdf;
    write("This is a string");
    if (y == 5 || y < 5 && y != 1 && y > 6) {
        y = y / 2;
    }
}
