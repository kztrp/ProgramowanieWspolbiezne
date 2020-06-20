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
#include <semaphore.h>
#define SIZE 4

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
 		dane_t dane[SIZE];
 		int head;
 		int tail;
 		int cnt;
 		sem_t mutex;
 		sem_t empty;
 		sem_t full;
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

	buf-> cnt = 0;
	buf->head = 0;
	buf->tail = 0;
	if(sem_init(&(buf->mutex),1,1)){
 		perror("mutex");
 		_exit(0);
	}
	if(sem_init(&(buf->empty),1,SIZE)) {
 		perror("empty"); _exit(0);
	}
	if(sem_init(&(buf->full),1,0)) {
 		perror("full");
 		_exit(0);
	}
    
		for(int i=0 ; i<bounds; i++){
			lb[i] = lower_bound+i*bound;
            ub[i] = lower_bound+(i+1)*bound;
			//printf("Wysłano przedział (%d-%d)\n", lb, ub);
		}
		for(int i=0; i<bounds; i++){
	 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
				sem_wait(&(buf->empty));
 				sem_wait(&(buf->mutex));
				dane_t data;
	        	int primes_found = 0;

        	for(int j=lb[i]; j<ub[i]; j++){
            	primes_found += prime(j);
        	}
        	data.pocz = lb[i];
        	data.kon = ub[i];
        	data.suma = primes_found;
        	//printf("Wysłano %d-%d\n", lb[i], ub[i]);
        	buf->dane[buf->head] = data;
 			buf-> cnt ++;
 			buf->head = (buf->head +1) % SIZE;
 			sem_post(&(buf->mutex));
 			sem_post(&(buf->full));
        	return(0);
			}
		}
 /* Proces macierzysty */
	for(int i=0 ; i<bounds; i++){
		sem_wait(&(buf->full));
 		sem_wait(&(buf->mutex));
 		
 		printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", buf->dane[buf->tail].pocz, buf->dane[buf->tail].kon, buf->dane[buf->tail].suma);
		number_of_primes+=buf->dane[buf->tail].suma;
		buf-> cnt --;
 		buf->tail = (buf->tail +1) % SIZE;
 		sem_post(&(buf->mutex));
 		sem_post(&(buf->empty));
		
	//printf("%d\n", bytes_read);
	}
	for(int i=0; i<bounds; i++){
		pids[i] += wait(&status);
        WEXITSTATUS(status);
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