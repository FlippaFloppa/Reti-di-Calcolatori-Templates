#include <stdio.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

int main(int argc, char *argv[])
{
	char *host;
	CLIENT *cl;
	void *in;
	char line[STRLENGHT], number[STRLENGHT];
	int *res, numero;

	if (argc != 2)
	{
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}
	host = argv[1];

	// Modificare a seconda del file .x
	cl = clnt_create(host, SERVER, SERVERVERS, "tcp");
	if (cl == NULL)
	{
		clnt_pcreateerror(host);
		exit(1);
	}

	char *richiesta = "Inserire ^D per terminare: \nInserire A o B\n";
	printf("%s", richiesta);

	while (scanf("%s", line) > 0)
	{

		if (strcmp(line, "A") == 0)
		{
			printf("Caso A: \n");
		}

		else if (strcmp(line, "B") == 0)
		{
			printf("Caso B: \n");

			printf("Insersci numero: ");
			scanf("%s", number);
			int ok;
			do
			{
				ok = 1;
				for (int i = 0; i < strlen(number) && ok; i++)
				{
					if ((number[i] < '0') || (number[i] > '9'))
					{
						ok = 0;
					}
				}
				if (!ok)
				{
					printf("Inserisci numero ");
					scanf("%s", number);
				}
			} while (!ok);

			numero = atoi(number);

			res = getsame_1(&numero, cl);

			if (res == NULL)
			{
				printf("Errore risultato null\n");
			}
			else
			{
				printf("Risultato: %d\n", *res);
			}
		}

		else
			printf("Argomento di ingresso errato!!\n");
		printf("%s", richiesta);
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	exit(0);
}
