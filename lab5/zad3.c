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
	int bounds = atoi(argv[3]);
	int processes = 4;
	int pids[processes];
    int number_of_primes = 0;
	int bound = (upper_bound-lower_bound)/bounds;
	int fd_enter, fd_exit;
	int writer;
	unlink("FIFO_wejscie");
	if(mkfifo("FIFO_wejscie",0666) < 0) {
 		perror("mkfifo");
 	}
 	unlink("FIFO_wyjscie");
	if(mkfifo("FIFO_wyjscie",0666) < 0) {
 		perror("mkfifo");
 	}
	fd_exit = open("FIFO_wyjscie", O_RDWR, S_IRWXU);
    if(fd_exit<0) {
		perror("open");
		exit(0);
	}
	fd_enter = open("FIFO_wejscie", O_RDWR, S_IRWXU);
	if(fd_enter<0) {
		perror("open");
		exit(0);
	}
	if((writer= fork()) == 0){
		for(int i=0 ; i<bounds; i++){
			int lb = lower_bound+i*bound;
            int ub = lower_bound+(i+1)*bound;
            struct result res = {lb, ub, 0};
			write(fd_enter, &res, sizeof(res));
		}
		for(int i = 0; i<processes; i++){
			struct result res = {0,0,0};
			write(fd_enter, &res, sizeof(res));
		}
		exit(0);
	}
	for(int i=0; i<processes; i++){
 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
            
            execl("./licz2","licz2",NULL);
		}
	}
	
 /* Proces macierzysty */
	for(int i=0; i<processes; i++){
		pids[i] += wait(&status);
        WEXITSTATUS(status);
	}
	writer += wait(&status);
    WEXITSTATUS(status);
	int bytes_read, counter=0;
	struct result wynik;
	do{
	bytes_read = read(fd_exit,&wynik,sizeof(wynik));
	if (bytes_read == 0) break;
	printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", wynik.pocz, wynik.kon, wynik.ile);
	counter++;
	number_of_primes+=wynik.ile;
	//printf("%d\n", bytes_read);
	}while(counter<bounds);
	close(fd_exit);
	close(fd_enter);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
}
