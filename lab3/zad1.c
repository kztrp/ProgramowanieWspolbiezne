#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>


void main(int argc, char * argv[]){
	struct timespec start, finish;
	double time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	int status;
	int lower_bound = atoi(argv[1]);
	int upper_bound = atoi(argv[2]);
	int processes = atoi(argv[3]);
	int pids[processes];
    int number_of_primes = 0;
	int bound = (upper_bound-lower_bound)/processes;
	for(int i=0; i<processes; i++){
 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
            int lb = lower_bound+i*bound;
            int ub = lower_bound+(i+1)*bound;
            //printf("Dolny przedział: %d \t Górny przedział: %d\n",lb,ub);
			char buffer[50];
            char buffer2[50];
            sprintf(buffer, "%d ", lb);
            sprintf(buffer2, "%d ", ub);
            execl("./licz","licz", buffer, buffer2, NULL);
		}
	}
	
 /* Proces macierzysty */
	for(int i=0; i<processes; i++){
		pids[i] += wait(&status);
        number_of_primes += WEXITSTATUS(status);
	}
	
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
}
