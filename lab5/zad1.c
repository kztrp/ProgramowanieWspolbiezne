#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

int prime(int n);

void main(int argc, char * argv[]){
	struct result {
 		int pocz;// początek przedzialu
 		int kon; // koniec przedzialu
 		int ile; // Ile liczb w przedziale
	};
	struct timespec start, finish;
	double time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	int fd[2];
	int status;
	int lower_bound = atoi(argv[1]);
	int upper_bound = atoi(argv[2]);
	int processes = atoi(argv[3]);
	int pids[processes];
    int number_of_primes = 0;
	int bound = (upper_bound-lower_bound)/processes;
	pipe(fd);
	for(int i=0; i<processes; i++){
 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
			close(fd[0]);
            int lb = lower_bound+i*bound;
            int ub = lower_bound+(i+1)*bound;
            int primes_found = 0;
            for(int j = lb; j<ub;j++){
            	primes_found+=prime(j);
            }
            struct result my_res = {lb, ub, primes_found};
            write(fd[1], &my_res, sizeof(my_res));
            close(fd[1]);
            exit(0);
		}
	}
	close(fd[1]);
	int counter = 0;
	struct result wynik;
	for(int i=0; i<processes; i++){
		pids[i] += wait(&status);
        WEXITSTATUS(status);
	}
	while(counter<processes){
		read(fd[0], &wynik, sizeof(wynik));
		printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", wynik.pocz, wynik.kon, wynik.ile);
		number_of_primes += wynik.ile;
		counter++; 
	}
	close(fd[0]);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}