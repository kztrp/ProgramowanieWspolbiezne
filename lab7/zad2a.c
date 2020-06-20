#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int prime(int n);

void main(int argc, char * argv[]){
	int bounds = atoi(argv[3]);
	typedef struct {
 		int pocz;
 		int kon ;
 		int suma;
	} dane_t;

	typedef struct {
 		int wymiar;
 		dane_t dane[bounds];
	}buf_t;

	buf_t * buf;
	struct timespec start, finish;
	double time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);
	int status;
	int lower_bound = atoi(argv[1]);
	int upper_bound = atoi(argv[2]);
	int size;	
	int pids[bounds];
    int number_of_primes = 0;
	int bound = (upper_bound-lower_bound)/bounds;
	int writer;
	int fd;
	int res;

	shm_unlink("bufor");
	fd=shm_open("bufor", O_RDWR|O_CREAT , 0774);
	if(fd == -1){
 		perror("open"); _exit(-1);
	}
	printf("fd: %d\n",fd);
	size = ftruncate(fd,sizeof(buf_t));
	if(size < 0) {
		perror("trunc");
		_exit(-1);
	}
	int lb[bounds];
	int ub[bounds];
// Odwzorowanie segmentu fd w obszar pamieci procesow
	buf = (buf_t *)mmap(0, sizeof(buf_t), PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    
		for(int i=0 ; i<bounds; i++){
			lb[i] = lower_bound+i*bound;
            ub[i] = lower_bound+(i+1)*bound;
			//printf("Wysłano przedział (%d-%d)\n", lb, ub);
		}
		for(int i=0; i<bounds; i++){
	 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
				dane_t data;
	        	int primes_found = 0;
        //printf("Odebrano przedział (%d-%d)\n", lower_bound, upper_bound);

        	for(int j=lb[i]; j<ub[i]; j++){
            	primes_found += prime(j);
        	}
        	data.pocz = lb[i];
        	data.kon = ub[i];
        	data.suma = primes_found;
        	buf->dane[i] = data;
        	return(0);
			}
		}
 /* Proces macierzysty */
	
	for(int i=0; i<bounds; i++){
		pids[i] += wait(&status);
        WEXITSTATUS(status);
	}
	for(int i=0 ; i<bounds; i++){
	printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", buf->dane[i].pocz, buf->dane[i].kon, buf->dane[i].suma);
	number_of_primes+=buf->dane[i].suma;
	//printf("%d\n", bytes_read);
	}
	//writer += wait(&status);
    //WEXITSTATUS(status);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
    return(0);
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}