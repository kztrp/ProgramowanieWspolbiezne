#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#define  MQ_NAME_ENTER "/KolejkaWejsciowa"
#define  MQ_NAME_EXIT "/KolejkaWyjsciowa"


void main(int argc, char * argv[]){
	typedef struct {
 		int nr; // numer przedzialu i
 		int pocz; // poczatek zakresu obliczen Zd(i)
 		int kon; // koniec zakresu obliczen Zg(i)
 		int liczb; // ile liczb pierwszych w przedziale
	} result;
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
	int writer;

	int res;
	mq_unlink(MQ_NAME_ENTER);
	mq_unlink(MQ_NAME_EXIT);
	mqd_t mq, mq2;
    struct mq_attr attr;
    struct mq_attr atrib;
    unsigned int prior = 10;
    attr.mq_msgsize = sizeof(result);
    attr.mq_maxmsg = 10;
    attr.mq_flags = 0;
    atrib.mq_msgsize = sizeof(result);
    atrib.mq_maxmsg = 10;
    atrib.mq_flags = 0;
    mq = mq_open(MQ_NAME_ENTER , O_RDWR | O_CREAT , 0660, &attr );
    if( mq == -1 ) { 
        perror("Kolejka "); 
        exit(0); 
    }
    printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME_ENTER, mq);
    
    mq2 = mq_open(MQ_NAME_EXIT , O_RDWR | O_CREAT , 0660, &atrib);
    if( mq2 == -1 ) { 
        perror("Kolejka "); 
        exit(0); 
    }
    printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME_EXIT, mq2);


	if((writer= fork()) == 0){
		for(int i=0 ; i<bounds; i++){
			int lb = lower_bound+i*bound;
            int ub = lower_bound+(i+1)*bound;
            result my_res = {i, lb, ub, 0};
			res = mq_send(mq,(char *)&my_res, sizeof(my_res),prior);
			//printf("Wysłano przedział (%d-%d)\n", lb, ub);
		}
		for (int i=0; i<processes; i++){
			result my_res = {0,0,0,0};
			res = mq_send(mq,(char *)&my_res, sizeof(my_res),prior);
			//printf("Wysłano przedział (%d-%d)\n", 0, 0);
		}
		exit(0);
		
	}
	for(int i=0; i<processes; i++){
 		if((pids[i] = fork()) == 0) { /* Proces potomny ---*/
            
            execl("./licz","licz",NULL);
		}
	}
 /* Proces macierzysty */
	
	int bytes_read, counter=0;
	result wynik;
	do{
	res = mq_receive(mq2,(char *)&wynik,sizeof(wynik),&prior);
    if (res == -1 )  perror("Blad odczytu z mq");
	printf("Przedział'%d-%d, znaleziono %d liczb pierwszych\n", wynik.pocz, wynik.kon, wynik.liczb);
	counter++;
	number_of_primes+=wynik.liczb;
	//printf("%d\n", bytes_read);
	}while(counter<bounds);
	for(int i=0; i<processes; i++){
		pids[i] += wait(&status);
        WEXITSTATUS(status);
	}
	writer += wait(&status);
    WEXITSTATUS(status);
	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_elapsed = (finish.tv_sec - start.tv_sec);
	time_elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Time elapsed: %f seconds\n", time_elapsed);
    printf("Number of primes: %d \n", number_of_primes);
    mq_close(mq);
    mq_close(mq2);
}
