#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <stdbool.h>
#include <time.h>

int prime(int n);

int main(int argc, char * argv[]){
    int lower_bound = atoi(argv[1]);
    int upper_bound = atoi(argv[2]);
    int number_of_primes = 0;
    //printf("Dolny przedział: %d \t Górny przedział: %d\n", lower_bound, upper_bound);
    for(int i=lower_bound; i<upper_bound; i++){
        number_of_primes += prime(i);
    }
    return(number_of_primes);
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}
