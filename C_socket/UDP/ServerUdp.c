/* Server che fornisce la valutazione di un'operazione tra due interi */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

/*Struttura di una richiesta*/
/********************************************************/
//MODIFICARE A SECONDA DELLA RICHIESTA
typedef struct
{
    int op1;
    int op2;
    char tipoOp;
} Request;
/********************************************************/

int main(int argc, char **argv)
{
    int sd, port, len, num1, num2, ris;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    struct hostent *clienthost;
    Request *req = (Request *)malloc(sizeof(Request));

    /* CONTROLLO ARGOMENTI ---------------------------------- */
    // MODIFICARE
    if (argc != 2)
    {
        printf("Error: %s port\n", argv[0]);
        exit(1);
    }
    else
    {
        num1 = 0;
        while (argv[1][num1] != '\0')
        {
            if ((argv[1][num1] < '0') || (argv[1][num1] > '9'))
            {
                printf("Secondo argomento non intero\n");
                printf("Error: %s port\n", argv[0]);
                exit(2);
            }
            num1++;
        }
        port = atoi(argv[1]);
        if (port < 1024 || port > 65535)
        {
            printf("Error: %s port\n", argv[0]);
            printf("1024 <= port <= 65535\n");
            exit(2);
        }
    }

    /* INIZIALIZZAZIONE INDIRIZZO SERVER ---------------------------------- */
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    /* CREAZIONE, SETAGGIO OPZIONI E CONNESSIONE SOCKET -------------------- */
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        perror("creazione socket ");
        exit(1);
    }
    printf("Server: creata la socket, sd=%d\n", sd);

    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("set opzioni socket ");
        exit(1);
    }
    printf("Server: set opzioni socket ok\n");

    if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind socket ");
        exit(1);
    }
    printf("Server: bind socket ok\n");

    /* CICLO DI RICEZIONE RICHIESTE ------------------------------------------ */
    for (;;)
    {
        len = sizeof(struct sockaddr_in);
        if (recvfrom(sd, req, sizeof(Request), 0, (struct sockaddr *)&cliaddr, &len) < 0)
        {
            perror("recvfrom ");
            continue;
        }

        /* trattiamo le conversioni possibili */
        /*
        ESEMPIO CONVERSIONI
		num1=ntohl(req->op1);
		num2=ntohl(req->op2);
        */

        printf("Operazione richiesta: %i %c %i\n", num1, req->tipoOp, num2);
        clienthost = gethostbyaddr((char *)&cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
        if (clienthost == NULL)
            printf("client host information not found\n");
        else
            printf("Operazione richiesta da: %s %i\n", clienthost->h_name, (unsigned)ntohs(cliaddr.sin_port));

        /* Processamento dati, modificare
        if (req->tipoOp == '+')
            ris = num1 + num2;
        else if (req->tipoOp == '-')
            ris = num1 - num2;
        else if (req->tipoOp == '*')
            ris = num1 * num2;
        else if (req->tipoOp == '/')
        {
            if (num2 != 0)
                ris = num1 / num2;
            // Caso di errore
            else
                ris = 0;
        }
        */

        // Invio risposta
        ris = htonl(ris);
        if (sendto(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&cliaddr, len) < 0)
        {
            perror("sendto ");
            continue;
        }
    }
}
