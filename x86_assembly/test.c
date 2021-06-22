// gcc -m64 -no-pie -Wall test.o test.c -o test

#include <stdio.h>

extern int foo();

int main(int argc, char *argv[]){
    int n = foo();
    printf("%d\n", n); 
    return 0;
}