#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fd, fd2, rd;
	char buf[512];
	if(argc < 2) return 0;
	fd = open(argv[1], O_RDONLY);
	if(fd<0) {
		perror("open");
		exit(0);
	}
	fd2 = open(argv[2], O_CREAT | O_TRUNC | O_RDWR);
	if(fd2<0) {
		perror("open");
		exit(0);
	}
	do {
		rd = read(fd, buf, 512);
		write(fd2, buf, rd);
	} while(rd >= 512);
	close(fd);
	close(fd2);
	return 1;
}