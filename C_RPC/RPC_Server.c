#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <string.h>
#include "RPC_xFile.h"

/* STATO SERVER */
static Esempio esempio;
static int inizializzato=0;

void inizializza(){

	if( inizializzato==1 ) return;

    strcpy(esempio.stringa,"ESEMPIO RPC");

	inizializzato = 1;
	printf("Terminata inizializzazione struttura dati!\n");
}

Esempio *getstringa_1_svc(void *in, struct svc_req *rqstp) {

	inizializza();

	return (&esempio);
}

int *getsame_1_svc(int* number,struct svc_req *rqstp){
    return number;
}
