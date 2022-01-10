#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/********************************************************/
void gestore(int signo){
  int stato;
  printf("esecuzione gestore di SIGCHLD\n");
  wait(&stato);
}
/********************************************************/

int main(int argc, char **argv)
{
	int  listen_sd, conn_sd;
	int port, len, num;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	struct hostent *host;

	/* CONTROLLO ARGOMENTI ---------------------------------- */
	if(argc!=2){
		printf("Error: %s port\n", argv[0]);
		exit(1);
	}
	else{
		num=0;
		while( argv[1][num]!= '\0' ){
			if( (argv[1][num] < '0') || (argv[1][num] > '9') ){
				printf("Secondo argomento non intero\n");
				exit(2);
			}
			num++;
		} 	
		port = atoi(argv[1]);
		if (port < 1024 || port > 65535){
			printf("Error: %s port\n", argv[0]);
			printf("1024 <= port <= 65535\n");
			exit(2);  	
		}

	}

	/* INIZIALIZZAZIONE INDIRIZZO SERVER ----------------------------------------- */
	memset ((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	/* CREAZIONE E SETTAGGI SOCKET D'ASCOLTO --------------------------------------- */
	listen_sd=socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sd <0)
	{perror("creazione socket "); exit(1);}
	printf("Server: creata la socket d'ascolto per le richieste di ordinamento, fd=%d\n", listen_sd);

	if(setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
	{perror("set opzioni socket d'ascolto"); exit(1);}
	printf("Server: set opzioni socket d'ascolto ok\n");

	if(bind(listen_sd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
	{perror("bind socket d'ascolto"); exit(1);}
	printf("Server: bind socket d'ascolto ok\n");

	if (listen(listen_sd, 5)<0) //creazione coda d'ascolto
	{perror("listen"); exit(1);}
	printf("Server: listen ok\n");

	/* AGGANCIO GESTORE PER EVITARE FIGLI ZOMBIE,
	* Quali altre primitive potrei usare? E' portabile su tutti i sistemi?
	* Pregi/Difetti?
	* Alcune risposte le potete trovare nel materiale aggiuntivo!
	*/
	signal(SIGCHLD, gestore);

	/* CICLO DI RICEZIONE RICHIESTE --------------------------------------------- */
	for(;;){ 
	  	len=sizeof(cliaddr);
		if((conn_sd=accept(listen_sd,(struct sockaddr *)&cliaddr,&len))<0){
		/* La accept puo' essere interrotta dai segnali inviati dai figli alla loro
		* teminazione. Tale situazione va gestita opportunamente. Vedere nel man a cosa 
		* corrisponde la costante EINTR!*/
			if (errno==EINTR){
				perror("Forzo la continuazione della accept");
				continue;
			}
			else exit(1);
		}

		if (fork()==0){ // figlio
			/*Chiusura FileDescr non utilizzati e ridirezione STDIN/STDOUT*/
			close(listen_sd);
			host=gethostbyaddr( (char *) &cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
			if (host == NULL){
				printf("client host information not found\n"); continue;
			}
			else printf("Server (figlio): host client e' %s \n", host->h_name);
			printf("Server (figlio): eseguo l'ordinamento\n");

			close(1); close(0);
			dup(conn_sd); dup(conn_sd);
			close(conn_sd);
			/*execl = execute and leave, esegue un processo e termina.
			* DEF:
			*	execl(char *path, char *arg0, ..., char *argn,  0).
			*		  ^		^		^	^--- Terminatore lista argomenti		
			*		  |		|		---- Ultimo argomento del comando
			*		  |		---- Primo argomento del comando
			*		  ----- PATH del file del comando da eseguire
			*
			* Prestare attenzione al path del programma SORT sull'host server.
			* Alcuni comandi UNIX per cercare path:
			*	which; whereis...
			*/
			execl("/usr/bin/sort", "sort", (char *)0);

		} // figlio
		close(conn_sd);  // padre chiude socket di connessione non di scolto
	} // ciclo for infinito
}