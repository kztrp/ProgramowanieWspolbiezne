#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void funct(int n);

void main(int argc, char * argv[]){
    funct(0);
}


void funct(int n){
    printf("Wywołuję funkcję od %d\n",n);
    funct(n+1);
}
