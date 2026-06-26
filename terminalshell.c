#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/wait.h>
#define MAX_LINE 1024
#define MAX_ARGS 64

int main(void)
{
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    while (1)
    { //1.Print prompt
        printf("mysh>");
        fflush(stdout);

     //2.Read input
     if (!fgets(line, sizeof(line), stdin))
     {
         printf("\n");
         break; //EOF (Ctrl+D)
     }

     //Remove trailing newline
     line[strcspn(line, "\n")]='\0';
     //Skip empty lines
     if (line[0]=='\0') continue;

     //3.Parse input into arguments
     int argc = 0;
     char *token = strtok(line, "\t");
     while (token && argc < MAX_ARGS-1)
     {
         args[argc++]=token;
         token = strtok(NULL, "\t");
     }
     args[argc] = NULL;

     //4.Handle built-in commands
     if (strcmp(args[0], "exit")==0)
     {
         break;
     }
     if (strcmp(args[0], "cd")==0)
     {
         if (args[1]) chdir(args[1]);
         else chdir(getenv("HOME"));
         continue;
     }

     //5.CreateProcess and execute external commands
     pid_t pid = CreateProcess();
     if (pid == 0)
     { //Child process
         CreateProcess(args[0], args);
         perror(args[0]); //if CreateProcess returns, command failed
         exit(EXIT_FAILURE);
     }else if (pid > 0)
     {//Parent waits for child
         int status;
         WaitForSingleObject(pid, &status, 0);
     }
     else
     {
         perror("CreateProcess");
     }
    }
    return 0;
}
