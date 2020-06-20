#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

void main(int argc, char * argv[]){
	struct result {
 		int pocz;// początek przedzialu
 		int kon; // koniec przedzialu
 		int ile; // Ile liczb w przedziale
	};
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
	int fd;
	unlink("FIFO");
	if(mkfifo("FIFO",0666) < 0) {
 		perror("mkfifo");
 	}
	fd = open("FIFO", O_RDWR, S_IRWXU);
    if(fd<0) {
		perror("open");
		exit(0);
	}
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
        WEXITSTATUS(status);
	}
	
	int bytes_read, counter=0;
	while (counter<processes){
	struct result wynik;
	bytes_read = read(fd,&wynik,sizeof(wynik));
	printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", wynik.pocz, wynik.kon, wynik.ile);
	counter++;
	number_of_primes+=wynik.ile;
	}
	close(fd);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
}
