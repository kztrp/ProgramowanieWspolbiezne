#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void main(int argc, char * argv[]){
    while(1){
        malloc((10000)*sizeof(int));
    }
}


