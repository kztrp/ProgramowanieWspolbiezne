// Proces odbierajacy komunikaty - wysyla udp_cli
// Wspolpracuje z udp_cli
// Kompilacja gcc udp_serw.c -o udp_serw -lrt
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define BUFLEN 80
#define KROKI 10
#define PORT 9950
typedef struct {
 int typ;
 char buf[BUFLEN];
} msgt;
void blad(char *s) {
 perror(s);
 _exit(1);
}
int main(void) {
 struct sockaddr_in adr_moj, adr_cli;
 int s, i, slen=sizeof(adr_cli),snd, rec, blen=sizeof(msgt);
 char buf[BUFLEN];
 msgt msg;
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
 for (i=0; i<KROKI; i++) {
 rec = recvfrom(s, &msg, blen, 0,(struct sockaddr *) &adr_cli, &slen);
 if(rec < 0) blad("recvfrom()");
 printf("Odebrano komunikat z %s:%d res %d\n Typ: %d %s\n",
 inet_ntoa(adr_cli.sin_addr), ntohs(adr_cli.sin_port),
 rec,msg.typ,msg.buf);
 // Odpowiedz -----
 sprintf(msg.buf,"Odpowiedz %d",i);
 snd = sendto(s, &msg, blen, 0,(struct sockaddr *) &adr_cli, slen);
 if(snd < 0) blad("sendto()");
 printf("wyslano odpowiedz -res %d\n",snd);
 }
 close(s);
 return 0;
}
