#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include <string.h>
#include "RPC_xFile.h"

/* STATO SERVER */
static Dati dati;
static int inizializzato = 0;

void inizializza()
{

	if (inizializzato == 1)
		return;

	inizializzato = 1;
	printf("Terminata inizializzazione struttura dati!\n");
}

Dati *getstringa_1_svc(void *in, struct svc_req *rqstp)
{

	inizializza();

	return (&dati);
}

int *getsame_1_svc(int *number, struct svc_req *rqstp)
{

	inizializza();

	static int res;
	res = -1;

	if (number == NULL)
	{
		return &res;
	}

	res=*number;

	// operazioni

	return &res;
}
