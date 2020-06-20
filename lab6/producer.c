#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#define  MQ_NAME "/Kolejka"
#define PROD 1

 int main(int argc, char *argv[]) {
   int i, res, num=0,steps;
   unsigned int prior;
   mqd_t  mq;
   struct mq_attr attr;
   prior = 10;
   if(argc < 2) { printf("Uzycie: mq_send numer\n"); exit(0); }
   num = atoi(argv[1]);
   steps=atoi(argv[2]);
   ms_type producer;
   // Otworzenie kolejki komunikatow ----------------

   mq=mq_open(MQ_NAME , O_RDWR , 0660, &attr );
   if( mq == -1 ) {  perror("Kolejka "); exit(0);  }
   printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME,mq);
   for(i=0; i < steps ;i++) {
     sprintf(producer.text,"Producent %d komunikat %d",num,i);
     producer.pnr = num;
     producer.type = PROD;
     res = mq_send(mq,(char *)&producer,sizeof(producer.text),prior);
     if (res == -1 ) { perror("Blad zapisu do mq");
      continue; 
  	 }
     printf("Producent %d Komunikat %d wyslany\n",num,i);
    sleep(1);
  }
  mq_close(mq);
}  
 
