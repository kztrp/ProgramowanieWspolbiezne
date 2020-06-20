#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BSIZE 4 // Rozmiar bufora
#define LSIZE 80 // Dlugosc linii

typedef struct {
	char buf[BSIZE][LSIZE];
	int head;
	int tail;
	int cnt;
	sem_t mutex;
	sem_t empty;
	sem_t full;
} bufor_t;


int main(int argc,char *argv[]) {
	int i,stat,k, pid, size, fd,res;
	bufor_t *wbuf ;
	char c;
// Utworzenie segmentu ---------------------------
	shm_unlink("bufor");
	fd=shm_open("bufor", O_RDWR|O_CREAT , 0774);
	if(fd == -1){
 		perror("open"); _exit(-1);
	}
	printf("fd: %d\n",fd);
	size = ftruncate(fd,sizeof(bufor_t));
	if(size < 0) {
		perror("trunc");
		_exit(-1);
	}
// Odwzorowanie segmentu fd w obszar pamieci procesow
		wbuf = (bufor_t *)mmap(0, sizeof(bufor_t), PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
	if(wbuf == NULL) {
		perror("map");
		_exit(-1);
	}
// Inicjacja obszaru --------------------------------
	wbuf-> cnt = 0;
	wbuf->head = 0;
	wbuf->tail = 0;
	if(sem_init(&(wbuf->mutex),1,1)){
 		perror("mutex");
 		_exit(0);
	}
	if(sem_init(&(wbuf->empty),1,BSIZE)) {
 		perror("empty"); _exit(0);
	}
	if(sem_init(&(wbuf->full),1,0)) {
 		perror("full");
 		_exit(0);
	}
	sem_close(&(wbuf->mutex));
	sem_close(&(wbuf->empty));
	sem_close(&(wbuf->full));
	return 0;
}