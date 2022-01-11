#include <stdio.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

int main (int argc, char *argv[]){
	char *host;
	CLIENT *cl;
	int *ris, *start_ok;
	void * in;
    Esempio* esempio;
	char str[5];
	int  i, j, fila, col;
	char c, line[STRLENGHT];

	if (argc != 2){
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];

    // Modificare a seconda del file .x
	cl = clnt_create (host, SERVER, SERVERVERS, "tcp");
	if (cl == NULL){
		clnt_pcreateerror (host);
		exit (1);
	}

    char *richiesta = "Inserire:\nE) per esempio\n^D per terminare: ";
	printf("%s",richiesta);

	while (gets (line)){

		if( strcmp(line,"E")==0 ){
			printf("ESEMPIO: \n");
			
            esempio=getstringa_1(in,cl);

            printf("%s\n",esempio->stringa);
		}

		else printf("Argomento di ingresso errato!!\n");
		printf("%s",richiesta);
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy (cl);
	exit (0);
}
