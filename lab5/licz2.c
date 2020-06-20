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
    int fd_enter, fd_exit;
    int number_of_primes = 0;
    

    fd_enter = open("FIFO_wejscie", O_RDWR);
    if(fd_enter<0) {
        perror("open");
        exit(0);
    }
    fd_exit = open("FIFO_wyjscie", O_RDWR);
    if(fd_exit<0) {
        perror("open");
        exit(0);
    }
    struct result my_res;
    int bytes_read;
    while(1){
        number_of_primes = 0;
        read(fd_enter, &my_res, sizeof(my_res));
        int lower_bound = my_res.pocz;
        int upper_bound = my_res.kon;
        if(lower_bound==0&&upper_bound==0){
            close(fd_exit);
            close(fd_enter);
            return 0;
        }
        for(int i=lower_bound; i<upper_bound; i++){
            number_of_primes += prime(i);
        }
        my_res.ile = number_of_primes;
        write(fd_exit, &my_res, sizeof(my_res));
    }
    
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}
