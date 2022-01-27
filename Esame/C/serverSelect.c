#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define DIM_BUFF 1024
#define LENGTH_FILE_NAME 20
#define WORD_LENGHT 20
#define STRUCT_SIZE 7
#define max(a, b) ((a) > (b) ? (a) : (b))

// Modificare se necessario
void gestore(int signo)
{
    int stato;
    printf("esecuzione gestore di SIGCHLD\n");
    wait(&stato);
}

typedef struct
{
    char first[WORD_LENGHT];
    char second[WORD_LENGHT];
} request;

int main(int argc, char **argv)
{
    // variabili di connessione
    int listenfd, connfd, udpfd, fd_file, nready, maxfdp1, filefd;
    const int on = 1;
    fd_set rset;
    int len, nread, nwrite, num, port;
    struct sockaddr_in cliaddr, servaddr;

    // variabili di algoritmo
    struct dirent *cur;
    DIR *mainDir;
    request structReq;
    char tcpReq[256];
    int ris = 0, result = 0;

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

        /* TCP */
        if (FD_ISSET(listenfd, &rset))
        {
            printf("Ricevuta richiesta TCP\n");
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

            if (fork() == 0)
            {
                close(listenfd);

                printf("Dentro il figlio, pid=%i\n", getpid());

                result = 1;

                while ((num = read(connfd, tcpReq, sizeof(tcpReq))) > 0)
                {

                    printf("Richiesta: %s\n", tcpReq);

                    write(connfd, &result, sizeof(int));

                    printf("Invio result: %d\n", result);

                    printf("Fine\n\n");
                }

                printf("Figlio %i: termino\n", getpid());
                shutdown(connfd, 0);
                shutdown(connfd, 1);
                close(connfd);
                exit(0);
            }
        }

        /* UDP */
        if (FD_ISSET(udpfd, &rset))
        {
            len = sizeof(struct sockaddr_in);

            if (recvfrom(udpfd, &structReq, sizeof(structReq), 0, (struct sockaddr *)&cliaddr, &len) < 0)
            {
                perror("recvfrom");
                continue;
            }

            result = 0;

            printf("Richiesta: %s %s\n", structReq.first, structReq.second);

            ris = htonl(result);

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