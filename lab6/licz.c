#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <tgmath.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#define  MQ_NAME_ENTER "/KolejkaWejsciowa"
#define  MQ_NAME_EXIT "/KolejkaWyjsciowa"

int prime(int n);

int main(int argc, char * argv[]){
    typedef struct {
        int nr; // numer przedzialu i
        int pocz; // poczatek zakresu obliczen Zd(i)
        int kon; // koniec zakresu obliczen Zg(i)
        int liczb; // ile liczb pierwszych w przedziale
    } result;

    struct mq_attr attr, attr2;
    mqd_t mq, mq2;
    unsigned int prior = 10;
    int number_of_primes = 0;
    int res;

    mq = mq_open(MQ_NAME_ENTER , O_RDWR , 0660, &attr);
    if( mq == -1 ) { 
        perror("Kolejka "); 
        exit(0); 
    }
    printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME_ENTER, mq);
    mq2 = mq_open(MQ_NAME_EXIT , O_RDWR , 0660, &attr);
    if( mq2 == -1 ) { 
        perror("Kolejka "); 
        exit(0); 
    }
    printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME_EXIT, mq2);
    result my_res;
    mq_getattr(mq, &attr);
    while(1){
        // mq_getattr(mq, &attr);
        // long messages = attr.mq_curmsgs;
        // if (messages==0){
        //     exit(0);
        // }
        // printf("Wiadomości:%d\n", messages);
        number_of_primes = 0;
        res = mq_receive(mq,(char *)&my_res,sizeof(my_res),&prior);
        int lower_bound = my_res.pocz;
        int upper_bound = my_res.kon;
        //printf("Odebrano przedział (%d-%d)\n", lower_bound, upper_bound);
        if(lower_bound==0&&upper_bound==0){
            mq_close(mq);
            mq_close(mq2);
            printf("Koncze\n");
            exit(0);
            
        }
        for(int i=lower_bound; i<upper_bound; i++){
            number_of_primes += prime(i);
        }
        my_res.liczb = number_of_primes;
        res = mq_send(mq2,(char *)&my_res, sizeof(my_res),prior);
    }
    
}

int prime(int n){
    for(int j=2; j*j<n; j++){
            if(n%j==0) return(0);           
        }
    return(1);
}
