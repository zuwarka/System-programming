#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	
	pid_t pid_2;
char pidtree[10];
sprintf(pidtree, "%d", getpid());
	switch(pid_2 = fork()) {
		case -1:
			printf("Error process 2\n");
			break;
		case 0: {
			
			pid_t pid_3;
			
			switch(pid_3 = fork()) {
				case -1:
					printf("Error process 3\n");
					break;
				case 0: {
					
					pid_t pid_4, pid_5;
					
					switch(pid_4 = fork()) {
						case -1:
							printf("Error process 4\n");
							break;
						case 0: {
							printf("I'm process 4 and my PID is %d, my parent is 3 -> %d\n", getpid(), getppid());
							sleep(10);
					exit(0);
							break;
						}
						default: {
							
							switch(pid_5 = fork()) {
								case -1:
									printf("Error process 5\n");
									break;
								case 0: {
									
									pid_t pid_6;
									
									switch(pid_6 = fork()) {
										case -1:
											printf("Error process 6\n");
											break;
										case 0: {
											
											printf("I'm process 6 and my PID is %d, my parent 5 -> %d\n", getpid(), getppid());
											sleep(10);
					exit(0);
											break;
										}
										default: {
											printf("I'm process 5 and my PID is %d, my parent is 3 -> %d, my child is 6 -> %d\n", getpid(), getppid(), pid_6);
											sleep(10);
					exit(0);
										}
									}
									
									break;
								}
								default: {
									printf("I'm process 3 and my PID is %d, my parent 2 -> %d, my childs are 4 -> %d and 5 -> %d\n", getpid(), getppid(), pid_4, pid_5);
									sleep(10);
					exit(0);
								}
							}
							
						}
					}
					
					break;
				}
				default: {
					printf("I'm process 2 and my PID is %d, my parent is 1 -> %d, my child is 3 -> %d\n", getpid(), getppid(), pid_3);
					sleep(10);
					exit(0);
				}
				
			}
			
			
			break;
		}
		default: {
			printf("I'm process 1 and my PID is %d, my parent is 2 -> %d\n", getpid(), pid_2);
		}
	}
	
	sleep(1);
	execlp("pstree","pstree",pidtree, "-c","-p", (int *)NULL);
	return 0;
	
}
