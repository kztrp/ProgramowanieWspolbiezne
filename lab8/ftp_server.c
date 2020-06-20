// Proces odbierajacy komunikaty - wysyla udp_cli
// Wspolpracuje z udp_cli
// Kompilacja gcc udp_serw.c -o udp_serw -lrt
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define PORT 9950

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
int main(void) {
 	struct sockaddr_in adr_moj, adr_cli;
 	int s, i, slen=sizeof(adr_cli),snd, rec, blen=sizeof(mms_t);
 	char buf[SIZE];
 	char buffer[512];
 	mms_t msg;
 	gethostname(buf,sizeof(buf));
 	printf("Host: %s\n",buf);
 	s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 	if(s < 0) blad("socket");
 	printf("Gniazdko %d utworzone\n",s);
 // Ustalenie adresu IP nadawcy
 	memset((char *) &adr_moj, 0, sizeof(adr_moj));
 	adr_moj.sin_family = AF_INET;
 	adr_moj.sin_port = htons(PORT);
 	adr_moj.sin_addr.s_addr = htonl(INADDR_ANY);
 	if (bind(s,(struct sockaddr *) &adr_moj, sizeof(adr_moj))==-1)
 	blad("bind");
 // Odbior komunikatow ------------
 	do{
 		rec = recvfrom(s, &msg, blen, 0, &adr_cli, &slen);
 		if(rec < 0) blad("recvfrom()");
		printf("Odebrano komunikat %d\n",msg.typ);
 		switch (msg.typ) {
 		case OPENR: 
 			msg.fh = open(msg.buf,O_RDONLY);
 			break;
 		
 		case READ:
 			msg.ile = read(msg.fh,msg.buf,SIZE);
 			break;

 		case OPENW:
 			strcpy(buffer, "ServerFiles/");
 			strcat(buffer, msg.buf);
 			msg.fh = open(buffer, O_CREAT | O_WRONLY | O_TRUNC, 0660);
 			if(msg.fh < 0){
 				blad("niepoprawny plik");
 			}
 			break;

 		case WRITE:
 			write(msg.fh, msg.buf, msg.ile);
 			break;
 		}
 		snd = sendto(s, &msg, blen, 0, &adr_cli, slen);
 		if(snd < 0) perror("sendto");
 	} while(msg.typ != STOP);
 	// Odpowiedz -----
 	close(s);
 	return 0;
}
