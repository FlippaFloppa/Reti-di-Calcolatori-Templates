#include <stdio.h>
#include <stdlib.h>
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

#define DIM_BUFF 1024
#define SIZE 256
#define N 10

int endsWith(const char *str, const char *suffix);
int startsWith(const char *str, const char *prefix);
int isNumeric(const char *str);
void ricevi_file(int sd);
void invia_file(int connfd);
int *elimina_occorrenze_1_svc(char **in /*, struct svc_req *rqstp*/);
int check_number();

int endsWith(const char *str, const char *suffix)
{
    return strncmp(str + strlen(str) - strlen(suffix), suffix, strlen(suffix));
    // returns 0 if is equals, !0 if is not equals
}

int startsWith(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix));
}

int isNumeric(const char *str)
{
    while (*str != '\0')
    {
        if (*str < '0' || *str > '9')
            return 0;
        str++;
    }
    return 1;
}

void ricevi_file(int sd)
{
    char nomeDir[SIZE], nomeFile[SIZE], buff[DIM_BUFF];
    int num, ok, dim, fd, letti;

    printf("Inserire direttorio, EOF per terminare: \n");
    while (scanf("%s", nomeDir) > 0)
    {
        printf("Download file relative a %s...\n", nomeDir);
        write(sd, nomeDir, sizeof(nomeDir));

        // ricevo il numero dei file
        read(sd, &num, sizeof(int));
        printf("Ricevo %d file\n", num);

        while (num > 0)
        {
            ok = 1;
            // ricevo la dim
            read(sd, &dim, sizeof(int));
            // ricevo il nome
            read(sd, nomeFile, sizeof(nomeFile));
            printf("Ricevo %s di dim %d\n", nomeFile, dim);
            fd = open(nomeFile, O_WRONLY | O_CREAT, 0666);
            if (fd < 0)
            {
                printf("Errore creazione %s\n", nomeFile);
                ok = 0;
            }
            if (ok)
            {
                while (dim > 0)
                {
                    letti = read(sd, buff, dim > sizeof(buff) ? sizeof(buff) : dim);
                    dim = dim - letti;
                    if (write(fd, buff, letti) < 0 && letti > 0)
                    {
                        printf("Errore scrittura sul file %s\n", nomeFile);
                    }
                }
                close(fd);
            }
            else
            {
                while (dim > 0)
                {
                    letti = read(sd, buff, dim > sizeof(buff) ? sizeof(buff) : dim);
                    dim = dim - letti;
                    // mangio il buff
                }
            }
            num--;
        }

        printf("Operazione terminata\n\n");
        printf("Inserire direttorio, EOF per terminare: \n");
    } // while
}

void invia_file(int connfd)
{
    char nome_dir[SIZE], nome_file[SIZE], buff[DIM_BUFF];
    int numFile, filefd, fileSize, nread;
    DIR *mainDir;
    struct dirent *cur;

    while (read(connfd, nome_dir, sizeof(nome_dir)) > 0)
    {
        printf("Richiesto trasferimento file da %s\n", nome_dir);
        numFile = 0;

        mainDir = opendir(nome_dir);
        if (mainDir == NULL)
        {
            printf("Directory non valida\n");
            numFile = 0;
            write(connfd, &numFile, 4);
        }
        else
        {
            while ((cur = readdir(mainDir)) != NULL)
            {
                printf("Esploro %s\n", cur->d_name);
                if (cur->d_type != DT_DIR /*&& !strncmp(cur->d_name + strlen(cur->d_name) - strlen(".txt"), ".txt", strlen(".txt"))*/)
                {
                    strcpy(nome_file, nome_dir);
                    strcat(strcat(nome_file, "/"), cur->d_name);
                    // conto il file se posso aprirlo
                    if ((filefd = open(nome_file, O_RDONLY)) > 0)
                    {
                        // verifico dimensione minima
                        // if (lseek(filefd, 0, SEEK_END) > dimMinima)
                        // {
                        numFile++;
                        // }
                        close(filefd);
                    }
                }
            }

            // invio num file
            printf("Num file %d\n", numFile);
            write(connfd, &numFile, sizeof(int));

            closedir(mainDir);
            mainDir = opendir(nome_dir);

            while ((cur = readdir(mainDir)) != NULL)
            {
                printf("Esploro %s\n", cur->d_name);
                if (cur->d_type != DT_DIR /*&& !strncmp(cur->d_name + strlen(cur->d_name) - strlen(".txt"), ".txt", strlen(".txt"))*/)
                {
                    strcpy(nome_file, nome_dir);
                    strcat(strcat(nome_file, "/"), cur->d_name);

                    if ((filefd = open(nome_file, O_RDONLY)) > 0)
                    {
                        fileSize = lseek(filefd, 0, SEEK_END);
                        // invio dimensione
                        write(connfd, &fileSize, sizeof(fileSize));
                        lseek(filefd, 0, SEEK_SET);
                        // invio nome
                        write(connfd, cur->d_name, SIZE);

                        printf("Trasferisco:\t%s\t%d\n", nome_file, fileSize);

                        while ((nread = read(filefd, buff, sizeof(buff))) > 0)
                        {
                            write(connfd, buff, nread);
                        }
                        close(filefd);
                    }
                }
            }
            closedir(mainDir);
            // Finiti i files
            printf("File terminati\n\n");
        }
    }
}

int *elimina_occorrenze_1_svc(char **in /*, struct svc_req *rqstp*/)
{
    static int res = 0;
    char parola[SIZE]; // parola da eliminare

    char c;
    int fd = open(*in, O_RDONLY);
    char filetmp[SIZE + 5];
    strcpy(filetmp, *in);
    strcat(filetmp, ".txt");
    int fd2 = open(filetmp, O_RDWR | O_CREAT, 0666);
    if (fd < 0 || fd2 < 0)
    {
        res = -1;
    }
    else
    {
        // elimina numero
        /*
        while (read(fd, &c, sizeof(char)) > 0)
        {
            if (!(c > '0' && c < '9'))
            {
                if (write(fd2, &c, sizeof(char)) < 0)
                {
                    printf("Errore nella scrittura\n");
                }
            }
            else
            {
                res++;
                printf("%d\n", res);
            }
        }
        */

        // elimina parola
        /*
        char buff[SIZE];
        int indice = 0;

        while (read(fd, &c, sizeof(char)) > 0)
        {
            if (c == ' ' || c == '\n')
            {
                buff[indice] = '\0';
                if (strcmp(buff, parola) != 0)
                {
                    if (write(fd2, buff, strlen(buff)) < strlen(buff))
                    {
                        printf("Ho scritto meno di quanto avrei dovuto\n");
                    }
                }
                write(fd2, &c, sizeof(char));
                indice=0;
            }
            else
            {
                buff[indice] = c;
                indice++;
            }
        }
        */

        close(fd);
        close(fd2);
        rename(filetmp, *in);
    }

    return &res;
}

int check_number()
{
    int ok, num;
    char number[SIZE];
    scanf("%s", number); // scanf of the big while

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
            printf("Inserisci prezzo minimo\n");
            scanf("%s", number);
        }
    } while (!ok);

    num = atoi(number);
    return num;
}

int main(int argc, char const *argv[])
{

    return 0;
}
