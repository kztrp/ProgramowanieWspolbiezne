#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define SIZE 512
int main(int argc, char *argv[]){
	int ile;
	FILE *f1, *f2;
	char buf[SIZE];
	f1 = fopen(argv[1],"r");
	if(f1 == NULL) {
		perror("fopen"); 
		exit(0);
	}
	f2 = fopen(argv[2],"w");
	if(f1 == NULL) {
		perror("fopen"); 
		exit(0);
	}

	do {
		ile = fread(&buf,sizeof(buf), 1, f1);
		fwrite(buf, 1, sizeof(buf), f2);
	} while(!feof(f1));
	fclose(f1);
	fclose(f2);
	return 0;
}