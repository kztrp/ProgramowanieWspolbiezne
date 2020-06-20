#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#define  MQ_NAME "/Kolejka"

int main(int argc, char *argv[]) {
 int i, res;
 unsigned int prior;
 mqd_t  mq;
 struct mq_attr attr;
 prior = 10;
 int steps=atoi(argv[1]);

 ms_type consumer;
 mq=mq_open(MQ_NAME , O_RDWR , 0660, &attr );
 if( mq == -1 ) {  
 	perror("Kolejka "); 
 	exit(0); 
 }
 printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME,mq);
 for(i=0; i < steps ;i++) {

    res = mq_receive(mq,(char *)&consumer,sizeof(consumer),&prior);
    if (res == -1 )  perror("Blad odczytu z mq");
    else                 printf("Odebrano: %s\n", consumer.text);
    sleep(1);
  }
  mq_close(mq);
}