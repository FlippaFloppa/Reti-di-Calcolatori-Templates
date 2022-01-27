/* Definizione costanti/strutture */
const STRLENGHT = 100;

struct Dati{
	char stringa[STRLENGHT];
};


/* Definizione programma e metodi */
program SERVER {
	version SERVERVERS{
		Dati GETSTRINGA(void) = 1;
        int GETSAME(int) = 2;
	} = 1;
} = 0x20000013;
