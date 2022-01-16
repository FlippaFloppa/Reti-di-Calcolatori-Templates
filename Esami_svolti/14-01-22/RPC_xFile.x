/*Venerandi Lorenzo 0000914987*/
const STRLENGHT = 100;
const FILENUM = 6;

struct File{
	char nome[STRLENGHT];
};

struct Data{
	char stringa[STRLENGHT];
};

struct ListaFiles
{
	File files[FILENUM];
};

/* Definizione programma e metodi */
program SERVER {
	version SERVERVERS{
		int ELIMINA_OCCORRENZE(Data) = 1;
        ListaFiles LISTA_FILETESTO(Data) = 2;
	} = 1;
} = 0x20000013;
