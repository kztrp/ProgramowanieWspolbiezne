#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#define  MQ_NAME "/Kolejka"

main(int argc, char *argv[]) {
    mq_unlink(MQ_NAME);
    int i, res, num = 0;
    unsigned int prior;
    mqd_t mq;
    struct mq_attr attr;
    prior = 10;
    attr.mq_msgsize = sizeof(ms_type);
    attr.mq_maxmsg = 4;
    attr.mq_flags = 0;
    mq = mq_open(MQ_NAME , O_RDWR | O_CREAT , 0660, &attr );
    if( mq == -1 ) { 
        perror("Kolejka "); 
        exit(0); 
    }
    printf("Kolejka: %s otwarta, mq: %d\n", MQ_NAME, mq);
    mq_close(mq);
}