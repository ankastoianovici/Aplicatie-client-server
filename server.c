#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>


#define MY_PORT_ID 6666

int main()
{
    int sockid, newsockid, i,j,length,x;
    int repeat =1;
    struct sockaddr_in ssock_addr;
    char mesaj[255];
    //functia socket() care returneaza un descriptor sau in caz de eroare returneaza valoarea -1
    if ((sockid = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  //familia de protocoale-Internet protocols
    									//type- stream socket
									//protocol
    {
	perror("eroare socket");
	return -1;
    }
//Daca dorim ca procesul server sa asculte pe toate interfetele de retea existente pe sistem se poate folosi ca valoare pentru s_addr constanta INADDR_ANY.
    bzero((char *) &ssock_addr, sizeof(ssock_addr));
    ssock_addr.sin_family = AF_INET;
    ssock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    ssock_addr.sin_port = htons(MY_PORT_ID);
//functia bind() care are unul dintre parametrii valoarea returnata de functia socket(). Apelul functiei bind() returneaza valoarea 0 in caz de reusita sau valoarea -1 in caz de eroare
    if (( bind(sockid, (struct sockaddr *) &ssock_addr,
		sizeof(ssock_addr)) < 0)) 
    {
	perror("eroare bind");
	return -1;
    }
    //listen() este folosita  pe serverele cu conexiune orientate pentru a semnala ca doreste sa accepte conexiuni. In caz de eroare va returna valoarea -1.
if ( listen(sockid, 5) < 0) 
    {
	perror("eroare listen");
	return -1;
    }
 while(1)

{     
	
	 newsockid = accept(sockid, (struct sockaddr *)0, (int *)0);  //se creaza un nou socket, care are aceleasi proprietati ca si descriptorul sockid returnat de  functia socket() prin care va primi si trimite date serverul.
	if (newsockid < 0)
	{
	 perror("eroare socket nou");
	    return -1;
	}
        else repeat =1;

	// se apeleaza functia read() care citeste mesajul
	  
	while (repeat)
{
	if ((read(newsockid, &mesaj, sizeof(mesaj))) < 0)
	{
	    perror("eroare citire\n");
	    return -1;
	}

	write(newsockid, &mesaj, sizeof(mesaj));//este scris mesajul
	
	if (strcmp(mesaj,"GATA")==0) //se face o intrerupere cand se citeste in variabila  mesaj cuvantul "GATA"
		{repeat=0;

		close(newsockid); //inchidem socketul
		}
       }
 	
    }
    close(sockid);
    return 0;
}

