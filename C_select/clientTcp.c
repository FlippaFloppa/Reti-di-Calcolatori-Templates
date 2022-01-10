#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DIM_BUFF 100
#define LENGTH_FILE_NAME 20


int main(int argc, char *argv[]){
	int sd, nread, nwrite, port;
	char ok, buff[DIM_BUFF], nome_file[LENGTH_FILE_NAME];
	struct hostent *host;
	struct sockaddr_in servaddr;

	/* CONTROLLO ARGOMENTI ---------------------------------- */
	if(argc!=3){
		printf("Error:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}
	printf("Client avviato\n");

	/* PREPARAZIONE INDIRIZZO SERVER ----------------------------- */
	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	if (host == NULL){
		printf("%s not found in /etc/hosts\n", argv[1]);
		exit(2);
	}
	
	nread = 0;
	while (argv[2][nread] != '\0'){
		if ((argv[2][nread] < '0') || (argv[2][nread] > '9')){
			printf("Secondo argomento non intero\n");
			exit(2);
		}
		nread++;
	}
	port = atoi(argv[2]);
	if (port < 1024 || port > 65535)
	{printf("Porta scorretta...");exit(2);}

	servaddr.sin_addr.s_addr=((struct in_addr*) (host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	/* CORPO DEL CLIENT: */
	/* ciclo di accettazione di richieste di file ------- */
	char *operazione = "Nome del file da richiedere: ";
    printf("%s",operazione);

	while (gets(nome_file)){

		/* CREAZIONE E CONNESSIONE SOCKET (BIND IMPLICITA) ----------------- */
		/* in questo schema � necessario ripetere creazione, settaggio opzioni e connect */
		/* ad ogni ciclo (DENTRO), perch� il client fa una nuova connect ad ogni ciclo */
		sd=socket(AF_INET, SOCK_STREAM, 0);
		if (sd <0){perror("apertura socket "); exit(3);}
		printf("Creata la socket sd=%d\n", sd);

		if (connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr))<0)
		{perror("Errore in connect"); exit(4);}
		printf("Connect ok\n");

		if (write(sd, nome_file, (strlen(nome_file)+1))<0){
			perror("write");
			close(sd);
			printf("%s",operazione);
			continue;
		}
		printf("Richiesta del file %s inviata... \n", nome_file);
		shutdown(sd,1);//stop sending data

		if (read(sd, &ok, 1)<0){
			perror("read");
			close(sd);
			printf("%s",operazione);
			continue;
		}

		if (ok=='S'){
			printf("Ricevo il file:\n");
			while((nread=read(sd, buff, sizeof(buff)))>0){
				if ((nwrite=write(1, buff, nread))<0){ //print on screen
					perror("write");
					break;
				}
			}
			if ( nread<0 ){
				perror("read");
				close(sd);
				printf("%s",operazione);
				continue;
			}      
		}
		else if (ok=='N') printf("File inesistente\n");

		printf("Chiudo connessione\n");
		shutdown(sd, 0);
		close(sd); //chiusura sempre DENTRO
		printf("%s",operazione);

	}//while
	printf("\nClient: termino...\n");
	exit(0);
}
