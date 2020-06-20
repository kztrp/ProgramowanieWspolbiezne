#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

int prime(int n);

int main(int argc, char * argv[]){
    struct result {
        int pocz;// początek przedzialu
        int kon; // koniec przedzialu
        int ile; // Ile liczb w przedziale
    };
    int fd, res;
    int lower_bound = atoi(argv[1]);
    int upper_bound = atoi(argv[2]);
    int number_of_primes = 0;
    //printf("Dolny przedział: %d \t Górny przedział: %d\n", lower_bound, upper_bound);
    for(int i=lower_bound; i<upper_bound; i++){
        number_of_primes += prime(i);
    }
    struct result my_res = {lower_bound, upper_bound, number_of_primes};

    fd = open("FIFO", O_RDWR);
    if(fd<0) {
        perror("open");
        exit(0);
    }
    write(fd, &my_res, sizeof(my_res));
    close(fd);
    return 0;
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}
