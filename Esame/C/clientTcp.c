#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define h_addr h_addr_list[0]
#define DIM_BUFF 1024
#define LENGTH_FILE_NAME 20
#define WORD_LENGHT 20

int main(int argc, char *argv[])
{
    // variabili di connessione
    int sd, port, nread, fd;
    struct hostent *host;
    struct sockaddr_in servaddr;

    // variabili di algoritmo
    char tcpReq[256];
    int ris = 0, result = 0;

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    if (argc != 3)
    {
        printf("Error:%s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    /* INIZIALIZZAZIONE INDIRIZZO SERVER -------------------------- */
    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);

    /*VERIFICA INTERO*/
    nread = 0;
    while (argv[2][nread] != '\0')
    {
        if ((argv[2][nread] < '0') || (argv[2][nread] > '9'))
        {
            printf("Secondo argomento non intero\n");
            exit(2);
        }
        nread++;
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
    /* CREAZIONE SOCKET ------------------------------------ */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        perror("apertura socket");
        exit(1);
    }
    printf("Client: creata la socket sd=%d\n", sd);

    /* Operazione di BIND implicita nella connect */
    if (connect(sd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    printf("Client: connect ok\n");

    /* CORPO DEL CLIENT:
    ciclo di accettazione di richieste da utente ------- */
    char *operazione = "Inserire richiesta, EOF per terminare: ";
    printf("%s", operazione);

    while (gets(tcpReq))
    {
        printf("Richiesta %s...\n", tcpReq);

        write(sd, tcpReq, sizeof(tcpReq));

        if (read(sd, &result, sizeof(int)) < 0)
        {
            perror("Errore lettura");
            continue;
        }
        else
        {
            printf("Result: %d\n", result);
        }

        printf("Operazione terminata\n\n");

        printf("%s", operazione);
    } // while

    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
