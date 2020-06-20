#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc, char *argv[]){
	int res, fd;

	 if ((fd = open("blokada2.c", O_RDWR)) == -1) {
		 perror("open");
		 exit(0);
	 }
	 printf("Proba zajecia blokady\n");
	 res = lockf(fd,F_LOCK,0);
	 if (res == -1) {
	 	perror("lockf - 1");
	 	exit(1);
	 }
	 printf("Blokada zajeta\n");
	 printf("Nacisnij <RETURN> aby zwolnic blokade\n");
	 getchar();
	 res = lockf(fd,F_ULOCK,0);
	 if (res == -1) {
		perror("lockf - 2");
		exit(1);
	 }
	 printf("Blokada zwolniona\n");
	 close(fd);
	 return 0;
} 
