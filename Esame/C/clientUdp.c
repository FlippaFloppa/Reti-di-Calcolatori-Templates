#define h_addr h_addr_list[0]
#define DIM_BUFF 1024
#define LENGTH_FILE_NAME 20
#define WORD_LENGHT 20
#define STRUCT_SIZE 7

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

typedef struct
{
    char first[WORD_LENGHT];
    char second[WORD_LENGHT];
} request;

int main(int argc, char **argv)
{
    // variabili di connessione
    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;
    int port, num1, sd, len;

    // variabili di algoritmo
    char udpReq[WORD_LENGHT];
    request structReq;
    int res = 0, ris = 0, result = 0;

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 3)
    {
        printf("Error:%s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    /* INIZIALIZZAZIONE INDIRIZZO CLIENT E SERVER --------------------- */
    memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = INADDR_ANY;

    /* Passando 0 ci leghiamo ad un qualsiasi indirizzo libero,
     * ma cio' non funziona in tutti i sistemi.
     * Se nel nostro sistema cio' non funziona come si puo' fare?
     */
    clientaddr.sin_port = 0;

    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);

    /* VERIFICA INTERO */
    num1 = 0;
    while (argv[2][num1] != '\0')
    {
        if ((argv[2][num1] < '0') || (argv[2][num1] > '9'))
        {
            printf("Secondo argomento non intero\n");
            printf("Error:%s serverAddress serverPort\n", argv[0]);
            exit(2);
        }
        num1++;
    }
    port = atoi(argv[2]);

    /* VERIFICA PORT e HOST */
    if (port < 1024 || port > 65535)
    {
        printf("%s = porta scorretta...\n", argv[2]);
        exit(2);
    }
    if (host == NULL)
    {
        printf("%s not found in /etc/hosts\n", argv[1]);
        exit(2);
    }
    else
    {
        servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
        servaddr.sin_port = htons(port);
    }

    /* CREAZIONE SOCKET ---------------------------------- */
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("apertura socket");
        exit(1);
    }
    printf("Client: creata la socket sd=%d\n", sd);

    /* BIND SOCKET, a una porta scelta dal sistema --------------- */
    if (bind(sd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
    {
        perror("bind socket ");
        exit(1);
    }
    printf("Client: bind socket ok, alla porta %i\n", clientaddr.sin_port);

    /* CORPO DEL CLIENT: ciclo di accettazione di richieste da utente */
    char *operazione = "Inserire richiesta, ^D per terminare\n";
    printf("%s", operazione);

    while (gets(structReq.first))
    {

        printf("Inserisci second\n");
        gets(structReq.second);

        printf("Inserita richiesta %s %s\n", structReq.first, structReq.second);

        /* richiesta operazione */
        len = sizeof(servaddr);
        if (sendto(sd, &structReq, sizeof(structReq), 0, (struct sockaddr *)&servaddr, len) < 0)
        {
            perror("sendto");
            continue;
        }

        /* ricezione del risultato */
        printf("Attesa del risultato...\n");
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&servaddr, &len) < 0)
        {
            perror("recvfrom");
            continue;
        }
        res = (int)ntohl(ris);
        if (res < 0)
            printf("Operazione fallita!\n\n");
        else
            printf("Ricevuto risultato: %d!\n\n", res);
        printf("%s", operazione);
    } // while gets

    // CLEAN OUT
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
