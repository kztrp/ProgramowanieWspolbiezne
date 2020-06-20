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
    if ((fd = open("wynik.bin", O_WRONLY | O_APPEND)) == -1) {
        perror("open");
        exit(0);
    }
    res = lockf(fd,F_LOCK,0);
    if (res == -1) {
        perror("lockf - 1");
        exit(1);
    }
    printf("Blokada zajeta(%d-%d)\n",lower_bound, upper_bound);
    //printf("Dolny przedział: %d\n Górny przedział: %d\n Wynik: %d \n", my_res.pocz, my_res.kon, my_res.ile);
    write(fd, &my_res, sizeof(my_res));
    res = lockf(fd,F_ULOCK,0);
    if (res == -1) {
        perror("lockf - 1");
        exit(1);
    }
    printf("Blokada zwolniona(%d-%d)\n",lower_bound, upper_bound);
    close(fd);
    return 0;
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}
