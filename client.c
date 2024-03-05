/*12. Sa se realizeze o aplicatie client-server prin care sa se poata masura viteza de transfer intre client si
server. Clientul trebuie sa citeasca un fisier specificat pe linia de comanda si datele sa le transmita linie
cu linie catre server. Datele returnate de server vor fi directionate spre un fisier specificat. Viteza de
transmitere va fi afisata pe ecran. Se va folosi protocolul TCP.*/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MY_PORT_ID 6666
#define SERV_HOST_ADDR "127.0.0.1"

int main()
{   
    int sockid;
    struct sockaddr_in ssock_addr;
    FILE *f,*g;
    char mesaj[255];
    char s1[255];
    char s2[255];    
    struct timeval tv1,tv2;
    struct stat stat_file;
   // functia socket() care returneaza un descriptor sau in caz de eroare returneaza valoarea -1
 if ((sockid = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
	perror("eroare socket");
	return -1;
    }
//Daca dorim ca procesul server sa asculte pe toate interfetele de retea existente pe sistem se poate folosi ca valoare pentru s_addr constanta INADDR_ANY.
    bzero((char *) &ssock_addr, sizeof(ssock_addr));

    ssock_addr.sin_family = AF_INET;
    ssock_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    ssock_addr.sin_port = htons(MY_PORT_ID);
//functia  connect() care returneaza 0 in caz de reusita sau -1 in caz de eroare
    if (connect(sockid, (struct sockaddr *) &ssock_addr,  sizeof(ssock_addr)) < 0)
    {
perror("eroare la conectare");
	return -1;
    }
    //citire cele 2 fisiere (cel din care se citeste si cel in care se scrie)
   bzero((char *) &s1, sizeof(s1));
   printf("Numele fisierului sursa: ");
   scanf("%s",s1);
   
   bzero((char *) s2, sizeof(s2));
   printf("Numele fisierului destinatie: ");
   scanf("%s",s2);
  	f=fopen(s1,"rt");
  	g=fopen(s2,"wt");
     if (f==NULL) 
printf("ER:deschidere fisier %s ",s1);
     else if (g==NULL)
        printf("fisierul nu a fost creat %s ",s2);
     else if (stat(s1, &stat_file)<0)
        printf("eroare la stat");
     else
{
gettimeofday(&tv1,NULL);
//se citeste mesajul pana cand nu se ajunge la sfarsitul fisierului
while (!feof(f)) 
 { 
    
    fgets(mesaj,100,f);
    if ( (write(sockid, &mesaj, sizeof(mesaj))) < 0)
    {
	perror("Eroare la scriere");
	return -1;
    }

    bzero((char*) &mesaj, sizeof(mesaj));
    if ( (read(sockid, &mesaj, sizeof(mesaj))) < 0)
    {
	perror("Eroare la citire");
	return -1;
    }
 
    fputs(mesaj,g);

 }
gettimeofday(&tv2,NULL);
fclose(f);
fclose(g);
sprintf(mesaj,"stop");
//mesajul citit este scris in cel de-al doilea fisier.
write(sockid, &mesaj, sizeof(mesaj));
//se calculeaza viteza de transfer a datelor
printf("Viteza de transfer este: %f Kbytes/sec\n ",(stat_file.st_size/(((tv2.tv_usec-tv1.tv_usec)*0.000001)+tv2.tv_sec-tv1.tv_sec)/1024));
    
}
//se inchide socketul
    close(sockid);

    return 0;
}


