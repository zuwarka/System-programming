#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <wait.h>
#include <signal.h>


//Процесс 1 порождает два дочерних процесса, один из которых записывает  строку в  файл, второй нумерует строки в файле. Процесс 1 выводит на экран содержимое файла. Реализовать синхронизацию с помощью сигналов.


int writenumstr = 0;
	int exitch1=0;
	int exitch2=0;
	int waitsignal=0;
	pid_t pid[2];
void write_to_file(int signal_num)
{
  writenumstr=1;
}

void toexitch1(int signal_num)
{
 exitch1=1;
}

void revwaitsig(int signal_num)
{
waitsignal=1;
}

void sendsig(int signal_num)
{
kill(pid[1],SIGUSR1);
}


int main()
{
	int numstr=0;	
	FILE* file;
	file = fopen("file.txt", "w+");
    printf("Im a parent. I have a pid = %d\n", getpid());
	pid[0] = fork(); //создаем процесс 1
    if (pid[0] == -1)
    {
        printf("Cant create Child[0]\n");
		fclose(file);
        exit(1);
    }
    else if (pid[0] == 0)
    {
		signal(SIGUSR1,SIG_IGN);
		signal(SIGUSR2,SIG_IGN);
	struct sigaction sa = { 0 };
	sa.sa_flags = SA_RESTART;
    sa.sa_handler = &write_to_file; //обработчик сигнала
	struct sigaction sa2 = { 0 };
	sa2.sa_flags = SA_RESTART;
    sa2.sa_handler = &toexitch1; //обработчик сигнала
	
	sigaction(SIGUSR2, &sa2, NULL);  //сигнал на завершение процесса
	sigaction(SIGUSR1, &sa, NULL);   //сигнал на запись числа процессом
		printf("Child[1] was created. He has a pid = %d. His parent is %d\n", getpid(), getppid());
		while (!exitch1){
		if(writenumstr)
		{
		writenumstr=0;
		numstr++;
		fprintf(file,"%d ",numstr);
		fflush(file);
		printf("child 1 wrote num to file \n");
		kill(getppid(),SIGUSR1); //отправляем родительскому процессу сигнал о том, что закончили писать в файл
		}
		}
		exit(0);
    }

	
	pid[1] = fork(); //создаем процесс 2
	if (pid[1] == -1)
    {
        printf("Cant create Child[1]\n");
		fclose(file);
        exit(1);
    }
    else if (pid[1] == 0)
    {
    
		
		int status;
		signal(SIGUSR1,SIG_IGN);
	struct sigaction sa = { 0 };
	sa.sa_flags = SA_RESTART;
    sa.sa_handler = &revwaitsig; //обработчик сигнала
	sigaction(SIGUSR1, &sa, NULL);  
		printf("Child[2] was created. He has a pid = %d. His parent is %d\n", getpid(), getppid());
		
		for (int i=0; i<10; i++){
		waitsignal=0;
		kill(pid[0], SIGUSR1);
		printf("child 2 waiting to other chile wrote to file...\n");	
		while(!waitsignal) //ожидаем сигнала от родительского процесса, что первый дочерний завершил запись в файл
		{}
		fprintf(file,"str \n");
			fflush(file);
		printf("child 2 wrote str to file \n");
		}
		
		kill(pid[0],SIGUSR2);
		exit(0);
    }
	
	
	
	pid_t p;
    int status;
	
	signal(SIGUSR1,SIG_IGN);
	struct sigaction sa = { 0 };
	sa.sa_flags = SA_RESTART;
    sa.sa_handler = &sendsig; //обработчик сигнала
	sigaction(SIGUSR1, &sa, NULL);
	
	waitpid(pid[0], &status, 0);
	fseek(file, 0, SEEK_SET);
    char buf[10];
    printf("Text:\n");
    while(fgets(buf, 10, file) != NULL)
        printf("%s", buf);    

	
    fclose(file);
    return 0;
}
