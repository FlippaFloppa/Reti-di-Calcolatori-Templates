//Venerandi Lorenzo 0000914987
#include <stdio.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

int main(int argc, char *argv[])
{
	char *host;
	CLIENT *cl;
	static ListaFiles *listaFiles;
	Data *data=malloc(sizeof(Data));
	char c, line[STRLENGHT];
	static int *esito;

	if (argc != 2)
	{
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];

	cl = clnt_create(host, SERVER, SERVERVERS, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}

	char *richiesta = "Inserire:\nE) Elimina occorrenze numeriche in un file\nL) Lista file di testo in una directory\n^D per terminare: ";
	printf("%s", richiesta);

	while (gets(line))
	{

		if (strcmp(line, "E") == 0)
		{

			printf("Inserire nome del file da esplorare: ");
			if (gets(line))
			{
				strcpy(data->stringa, line);
				esito = elimina_occorrenze_1(data, cl);
				if (esito == NULL)
				{
					clnt_perror(cl, host);
					exit(1);
				}
				if (*esito == -1)
				{
					printf("Errore operazione\n");
				}
				else
				{
					printf("Sono state trovate ed eliminate %d occorrenze\n", *esito);
				}
			}
		}
		else if (strcmp(line, "L") == 0)
		{
			printf("Inserire nome della directory da esplorare: ");
			if (gets(line))
			{
				strcpy(data->stringa, line);
				listaFiles = lista_filetesto_1(data, cl);
				if (listaFiles == NULL)
				{
					clnt_perror(cl, host);
					exit(1);
				}
				printf("\n\n-----------------\n");

				for (int i = 0; i < FILENUM; i++)
				{
					if (strcmp(listaFiles->files[i].nome, "-") != 0)
					{
						printf("%s\n", listaFiles->files[i].nome);
					}
				}
				printf("-----------------\n");
			}
		}

		else{
			printf("Argomento di ingresso errato!!\n");
		}
			
		printf("%s", richiesta);
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto

	free(data);
	clnt_destroy(cl);
	exit(0);
}
