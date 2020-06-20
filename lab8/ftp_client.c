// Proces wysyla a potem odbiera komunikaty udp
// Wspolpracuje z udp_serw
// Kompilacja gcc udp_cli.c -o udp_cli -lrt
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define PORT 9950
#define SRV_IP "127.0.0.1"

#define OPENR 1 // Otwarcie pliku do odczytu
#define OPENW 2 // Otwarcie pliku do zapisu
#define READ 3 // Odczyt fragmentu pliku
#define CLOSE 4 // Zamkniecie pliku
#define WRITE 5 // Zapis fragmentu pliku
#define OPENDIR 6 // Otworz zdalny katalog
#define READDIR 7 // Czytaj zdalny katalog
#define STOP 10 // Zatrzymanie serwera
#define SIZE 512
typedef struct {
 int typ; // typ zlecenia
 int ile; // liczba bajtow
 int fh; // uchwyt pliku
 char buf[SIZE]; // bufor
} mms_t;

void blad(char *s) {
 	perror(s);
 	_exit(1);
 }
int main(int argc, char * argv[]) {
 	struct sockaddr_in adr_moj, adr_serw, adr_x;
 	int s, i, slen=sizeof(adr_serw), snd, blen=sizeof(mms_t),rec;
 	int choice = 0;
 	char buf[SIZE];
 	int fh;
 	mms_t msg;
 	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 	if(s < 0) blad("socket");
 	printf("Gniazdko %d utworzone\n",s);
 	memset((char *) &adr_serw, 0, sizeof(adr_serw));
 	adr_serw.sin_family = AF_INET;
 	adr_serw.sin_port = htons(PORT);
 	if (inet_aton(argv[1], &adr_serw.sin_addr)==0) {
 		fprintf(stderr, "inet_aton() failed\n");
 		_exit(1);
 	}
 	printf("Wybierz opcje: \n\t1.Odczyt pliku \n\t2.Zapis pliku\n");
 	scanf("%d", &choice);
 	getchar();
 	switch(choice){
 		case 1:
 			do { //Otwarcie pliku
		 		printf("Podaj nazwe pliku: ");
		 		gets(msg.buf);
		 		msg.typ = OPENR;
		 		snd = sendto(s, &msg, blen, 0, &adr_serw, slen);
		 		if(snd < 0) perror("sendto()");
		 		rec = recvfrom(s, &msg, blen, 0, &adr_moj, &slen);
		 		if(rec < 0) perror("recvfrom()");
	 		} while(msg.fh < 0);

		 	do { // Odczyt --------------------
		 		msg.typ = READ;
		 		msg.ile = SIZE;
		 		snd = sendto(s, &msg, blen, 0, &adr_serw, slen);
		 		if(snd < 0) perror("sendto()");
		 		rec = recvfrom(s, &msg, blen, 0, &adr_moj, &slen);
		 		if(rec < 0) perror("recvfrom()");
		 		printf("Odebrano: %d bajtow\n",msg.ile);
		 		msg.buf[msg.ile] = '\0';
		 		if(msg.ile > 0) printf("%s\n",msg.buf);
		 	} while(msg.ile == SIZE);
		 	break;
		case 2:
 			do { //Otwarcie pliku
		 		printf("Podaj nazwe pliku: ");
		 		gets(msg.buf);
		 		msg.typ = OPENW;
		 		snd = sendto(s, &msg, blen, 0, &adr_serw, slen);
		 		if(snd < 0) perror("sendto()");
		 		rec = recvfrom(s, &msg, blen, 0, &adr_moj, &slen);
		 		if(rec < 0) perror("recvfrom()");
	 		} while(msg.fh < 0);

	 		fh = open(msg.buf,O_RDONLY);
	 		
		 	do { // Zapis --------------------
		 		printf("%s\n", msg.buf);
		 		msg.ile = read(fh, msg.buf, SIZE);
		 		msg.typ = WRITE;
		 		//if(msg.ile > 0) printf("%s\n\n",msg.buf);
		 		snd = sendto(s, &msg, blen, 0, &adr_serw, slen);
		 		if(snd < 0) perror("sendto()");
		 		rec = recvfrom(s, &msg, blen, 0, &adr_moj, &slen);
		 		if(rec < 0) perror("recvfrom()");
		 	} while(msg.ile == SIZE);
		 	break;
 	}
 	close(s);
 	return 0;
}
