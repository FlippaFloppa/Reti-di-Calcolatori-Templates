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

#define DIM_BUFF 1024
#define LENGTH_FILE_NAME 20
#define WORD_LENGHT 20
#define STRUCT_SIZE 1
#define max(a, b) ((a) > (b) ? (a) : (b))


typedef struct stutturaDati
{
    char targa[WORD_LENGHT];
    char patente[WORD_LENGHT];
}prenotazione;


// Modificare se necessario
void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD\n");
    wait(&stato);
}




int main(int argc, char **argv)
{
    int listenfd, connfd, udpfd, fd_file, nready, maxfdp1,filefd,fileSize=-1;
    const int on = 1;
    char buff[DIM_BUFF], nome_file[LENGTH_FILE_NAME], nome_dir[LENGTH_FILE_NAME];
    char targa[10];
    char end[1];
	end[0] = (char)4;
    fd_set rset;
    int len, nread, nwrite, num, ris, port;
    struct sockaddr_in cliaddr, servaddr;
    DIR *mainDir;
    struct dirent *cur;
    prenotazione request;

    // CONTROLLO ARGOMENTI
    if (argc != 2)
    {
        printf("Error: %s port\n", argv[0]);
        exit(1);
    }

    nread = 0;
    while (argv[1][nread] != '\0')
    {
        if ((argv[1][nread] < '0') || (argv[1][nread] > '9'))
        {
            printf("Terzo argomento non intero\n");
            exit(2);
        }
        nread++;
    }
    port = atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        printf("Porta scorretta...");
        exit(2);
    }

    /* INIZIALIZZAZIONE INDIRIZZO SERVER E BIND ---------------------------- */
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    printf("Server avviato\n");

    /*INIZIALIZZAZIONE STRUTTURA DATI*/
    prenotazione prenotazioni[STRUCT_SIZE];
    strcpy(prenotazioni[0].patente,"393949");
    strcpy(prenotazioni[0].targa,"AB123AA");

    /* CREAZIONE SOCKET TCP ------------------------------------------------ */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        perror("apertura socket TCP ");
        exit(1);
    }
    printf("Creata la socket TCP d'ascolto, fd=%d\n", listenfd);

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket TCP");
        exit(2);
    }
    printf("Set opzioni socket TCP ok\n");

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind socket TCP");
        exit(3);
    }
    printf("Bind socket TCP ok\n");

    if (listen(listenfd, 5) < 0)
    {
        perror("listen");
        exit(4);
    }
    printf("Listen ok\n");


    /* CREAZIONE SOCKET UDP ------------------------------------------------ */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpfd < 0)
    {
        perror("apertura socket UDP");
        exit(5);
    }
    printf("Creata la socket UDP, fd=%d\n", udpfd);

    if (setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket UDP");
        exit(6);
    }
    printf("Set opzioni socket UDP ok\n");

    if (bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind socket UDP");
        exit(7);
    }
    printf("Bind socket UDP ok\n");

    /* AGGANCIO GESTORE PER EVITARE FIGLI ZOMBIE -------------------------------- */
    signal(SIGCHLD, gestore);

    /* PULIZIA E SETTAGGIO MASCHERA DEI FILE DESCRIPTOR ------------------------- */
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;

    /* CICLO DI RICEZIONE EVENTI DALLA SELECT ----------------------------------- */
    for (;;)
    {
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                perror("select");
                exit(8);
            }
        }

        /* GESTIONE RICHIESTE DI GET DI UN FILE ------------------------------------- */
        if (FD_ISSET(listenfd, &rset))
        {
            printf("Ricevuta richiesta di download di immagini\n");
            len = sizeof(struct sockaddr_in);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
            {
                if (errno == EINTR)
                    continue;
                else
                {
                    perror("accept");
                    exit(9);
                }
            }

            // Gestione tramite processo figlio

            if (fork() == 0)
            { /* processo figlio che serve la richiesta di operazione */
                close(listenfd);
				printf("Dentro il figlio, pid=%i\n", getpid());

                while ((num = read(connfd, targa, sizeof(targa))) > 0)
				{
                    strcpy(nome_dir,targa);
                    strcat(nome_dir,"_img");

					printf("Richiesto trasferimento immagini da %s\n", nome_dir);

					mainDir = opendir(nome_dir);
					if (mainDir == NULL)
					{
						printf("Directory non valida\n");
						write(connfd,&fileSize,4);
					}
					else
					{
						while ((cur = readdir(mainDir)) != NULL)
						{
                            printf("Esploro %s\n",cur->d_name);

							if (cur->d_type != DT_DIR)
							{
								strcpy(nome_file, nome_dir);
								strcat(strcat(nome_file, "/"), cur->d_name);

                                filefd=open(nome_file,O_RDONLY);
                                fileSize=lseek(filefd,0,SEEK_END);
                                write(connfd,&fileSize,sizeof(fileSize));
                                lseek(filefd,0,SEEK_SET);

                                fileSize=strlen(cur->d_name)+1;
                                write(connfd,&fileSize,4);
								write(connfd,cur->d_name,strlen(cur->d_name)+1);

								printf("Trasferisco:\t%s\t%d\n", nome_file,fileSize);

                                while((nread=read(filefd,buff,sizeof(buff))) > 0){
                                    write(connfd,buff,nread);
                                }
							}
						}

                        // Finiti i files
                        fileSize=0;
                        write(connfd,&fileSize,4); // Print carattere terminatore
                        printf("File terminati\n\n");
					}
				}

                printf("Figlio %i: termino\n", getpid());
				shutdown(connfd, 0);
				shutdown(connfd, 1);
				close(connfd);
				exit(0);
            }
        }

        /* GESTIONE RICHIESTE DI CONTEGGIO ------------------------------------------ */
        if (FD_ISSET(udpfd, &rset))
        {
            num=0;
            len = sizeof(struct sockaddr_in);
            if (recvfrom(udpfd, &request, sizeof(prenotazione), 0, (struct sockaddr *)&cliaddr, &len) < 0)
            {
                perror("recvfrom");
                continue;
            }

            printf("Richiesto aggiornamento targa %s\n",request.targa);

            for(int i=0;i<STRUCT_SIZE && num<1;i++){
                if(strcmp(prenotazioni[i].targa,request.targa)==0){
                    strcpy(prenotazioni[i].patente,request.patente);
                    num=1;
                    printf("Aggiornata targa %s con patente %s\n\n",prenotazioni[i].targa,prenotazioni[i].patente);
                }
            }

            ris = htonl(num);
            if (sendto(udpfd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
            {
                perror("sendto");
                continue;
            }
        }

    } /* ciclo for della select */
    /* NEVER ARRIVES HERE */
    exit(0);
}
