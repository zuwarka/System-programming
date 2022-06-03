#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <sys/types.h>

#define N 64
#define DELIM ",.!? "

const char* PATH_L = "log.txt";
const char* PATH_F = "file.txt";
const char* PATH_R = "res.txt";
FILE* file, *arg, *result;

pthread_attr_t attr;

void PrintReverse(char *str)
{
    if (str)
    {
        PrintReverse(strtok(NULL, DELIM));
        fprintf(result,"%s ", str);
    }
}

void* write_file(void* arg){
    PrintReverse(strtok((char*)arg, DELIM));
    fprintf(result,"\n");
    pthread_exit(0);
}

void* read_str(void* arg1){
    char str[N];
    pthread_t wr;
    result = fopen(PATH_R, "w+");
    if (result == NULL)
    {
        printf("Cant open file {%s}", PATH_R);
        pthread_exit(0);
    }
    while (fgets(str, N,(FILE*)arg1) != NULL)
    {
	str[strlen(str) - 1] = '\0';
        pthread_create(&wr, &attr, write_file, (void*)str);
        fprintf(file, "Thread [2] was created\n");
        pthread_join(wr, NULL);
        fprintf(file, "Thread [2] was joined\n");
    }
    fclose(result);
    pthread_exit(0);
}
int main(void){
    pthread_t swop;
    file = fopen(PATH_L, "w+");
    if (file == NULL)
    {
        printf("Cant open file {%s} for writting log!", PATH_L);
        return 1;
    }
    arg = fopen(PATH_F, "r");
    if (arg  == NULL)
    {   
        printf("Cant open file {%s}", PATH_F);
        return 1;
    }

    fprintf(file, "PROGRAM BEGIN\n");
    pthread_create(&swop, NULL, read_str, (void*)arg);
    fprintf(file, "Thread [1] was created\n");
    pthread_join(swop, NULL);
    fprintf(file, "Thread [1] was joined\n");
    
    fprintf(file, "PROGRAM END\n");
    fclose(file);
    fclose(arg);
    return 0;
}
