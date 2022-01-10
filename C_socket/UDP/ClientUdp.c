#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define LINE_LENGTH 256

/*Struttura di una richiesta*/
/********************************************************/
// MODIFICARE SECONDO SPECIFICA
typedef struct
{
    int op1;
    int op2;
    char tipoOp;
} Request;
/********************************************************/

int main(int argc, char **argv)
{
    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;
    int port, sd, num1, num2, len, ris, ok;
    char okstr[LINE_LENGTH];
    char c;
    Request req;
    char *operazione = "Primo operando (intero), EOF per terminare: ";

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
    printf("%s", operazione);

    //while(gets(line))
    while ((ok = scanf("%i", &num1)) != EOF)
    {

        // Esempio richiesta operazione fra numeri interi

        if (ok != 1)
        {

            do
            {
                c = getchar();
                printf("%c ", c);
            } while (c != '\n');
            printf(operazione);
            continue;
        }

        // quando arrivo qui l'input e' stato letto correttamente
        req.op1 = htonl(num1);
        // Consumo il new line, ed eventuali altri caratteri
        // immessi nella riga dopo l'intero letto
        gets(okstr);
        printf("Stringa letta: %s\n", okstr);

        printf("Inserire secondo operando (intero): ");

        while (scanf("%i", &num2) != 1)
        {
            do
            {
                c = getchar();
                printf("%c ", c);
            } while (c != '\n');
            printf("Secondo operando (intero): ");
        }

        req.op2 = htonl(num2);
        gets(okstr); //consumo resto linea
        printf("Stringa letta: %s\n", okstr);

        do
        {
            printf("Operazione (+ = addizione, - = sottrazione, * = moltiplicazione, / = divisione): ");
            c = getchar();
        } while (c != '+' && c != '-' && c != '*' && c != '/');

        req.tipoOp = c;
        gets(okstr); //consumo resto linea
        printf("Stringa letta: %s\n", okstr);

        // lettura completata
        printf("Operazione richiesta: %d %c %d \n",
               ntohl(req.op1), req.tipoOp, ntohl(req.op2));

        // richiesta operazione
        len = sizeof(servaddr);
        if (sendto(sd, &req, sizeof(Request), 0, (struct sockaddr *)&servaddr, len) < 0)
        {
            perror("sendto");
            continue;
        }

        // ricezione del risultato
        printf("Attesa del risultato...\n");
        if (recvfrom(sd, &ris, sizeof(ris), 0, (struct sockaddr *)&servaddr, &len) < 0)
        {
            perror("recvfrom");
            continue;
        }

        printf("Esito dell'operazione: %i\n", (int)ntohl(ris));
        printf("Primo operando (intero), EOF per terminare: ");

    } // while gets

    //CLEAN OUT
    close(sd);
    printf("\nClient: termino...\n");
    exit(0);
}
