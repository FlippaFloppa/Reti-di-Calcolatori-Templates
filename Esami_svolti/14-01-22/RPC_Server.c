#include <sys/stat.h>
#include <rpc/rpc.h>
#include <stdio.h>
#include <stdlib.h>
//Venerandi Lorenzo 0000914987
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "RPC_xFile.h"

ListaFiles *lista_filetesto_1_svc(Data* dir, struct svc_req *rqstp)
{
	printf("Ricevuta richiesta\n");
	char* nome_dir=dir->stringa;
	static ListaFiles* tmp;
	DIR *mainDir;
	struct dirent *cur;
	int counter=0;

	mainDir = opendir(nome_dir);

	if (mainDir == NULL)
	{
		printf("Directory non valida\n");
		strcpy(tmp->files[0].nome,"Directory non valida!");
		for(int i=1;i<FILENUM;i++){
			strcpy(tmp->files[0].nome,"-");
		}
	}
	else
	{
		while ((cur = readdir(mainDir)) != NULL)
		{
			printf("Trovato %s\n", cur->d_name);
			if(cur->d_type != DT_DIR && strstr(cur->d_name,".txt")!=NULL && counter<6){
				strcpy(tmp->files[counter].nome,cur->d_name);
				counter++;
			}

		}
		printf("File terminati\n\n");
		for(int i=counter;i<FILENUM;i++){
			// Riempio i file non trovati con ""
			strcpy(tmp->files[i].nome,"-");
		}
	}

	return tmp;
}

int *elimina_occorrenze_1_svc(Data* file, struct svc_req *rqstp)
{
	printf("Ricevuta richiesta di eliminazione\n");
	char* nomeFile=file->stringa;
	static int esito;
	char c;
	int nread, fd, tmp;
	esito = 0;

	if ((fd = open(nomeFile, O_RDWR)) < 0)
	{
		// Fallimento
		esito=-1;
		return &esito;
	}

	// Apro file temporaneo
	if ((tmp = open("tmp", O_CREAT | O_TRUNC | O_WRONLY)) <= 0)
	{
		// Fallimento
		esito=-1;
		return &esito;
	}

	while ((nread = read(fd, &c, 1)) > 0)
	{
		if (!(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'))
		{
			// Scrivo solo i caratteri diversi dai numeri
			write(tmp, &c, 1);
		}else{
			esito++;
		}
	}

	close(fd);
	close(tmp);

	// Rimpiazzo i file
	if ((remove(nomeFile)) <= 0)
	{
		// Fallimento
		esito=-1;
		return &esito;
	}
	if ((rename("tmp", nomeFile)) <= 0)
	{
		// Fallimento
		esito=-1;
		return &esito;
	}

	return &esito;
}
