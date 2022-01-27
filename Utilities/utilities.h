/*Funzione conteggio file in un direttorio*/
/********************************************************/
int conta_file (char *name){
	DIR *dir;
	struct dirent * dd;
	int count = 0;
	dir = opendir (name);
	if (dir==NULL) return -1;
	while ((dd = readdir(dir)) != NULL){
		printf("Trovato il file %s\n", dd-> d_name);
		count++;
	}
	/*Conta anche direttorio stesso e padre*/
	printf("Numero totale di file %d\n", count);
	closedir (dir);
	return count;
}
/********************************************************/
int isDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return 0;
	return S_ISDIR(statbuf.st_mode);
}
/********************************************************/
