#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    char commandString[128];     
    char * cmd_args[120],* total_args, * commands[10];   
    int total_commands, counter, total_arguments;
    int status;  
    
    while(1) {
        char ss[]="Bhavin shell: ";
        write(1,ss,strlen(ss));

        if(fgets(commandString, sizeof(commandString), stdin) == NULL) {
            break;
        }  
   
        if(strcmp(commandString, "exit\n") == 0) {
            strcpy(ss,"Bye Bye \n");
            write(1,ss,strlen(ss));
            break;  
        }

        total_commands = 0;
        commands[total_commands] = (char *) malloc(50 * sizeof(char));
        counter = 0;
        strcat(commandString,";");
        for(int i = 0; i < strlen(commandString); i++) {
            if(commandString[i] == ';') { 
                commands[total_commands][counter] = '\0';
                counter = 0;
                total_commands++;
                commands[total_commands] = (char *) malloc(50 * sizeof(char));
            } 
            else {
                commands[total_commands][counter] = commandString[i];
                counter++;
            }
        }

        total_arguments = 0;
        printf("\n");
        for(int i = 0; i < total_commands; i++) {
            char msg[] = "Current command executing: ";
            strcat(msg,commands[i]);
            write(1,msg,strlen(msg));
            write(1,"\n",1);
            total_arguments = 0;

            for (total_args = strtok(commands[i], " "); total_args; total_args = strtok(NULL, " ")) {
                cmd_args[total_arguments] = total_args;
                total_arguments++;
            }

            char * arguments[total_arguments + 1];
            for(int j = 0; j < total_arguments; j++) {
                arguments[j] = cmd_args[j];
            } 

            arguments[total_arguments] = NULL;

            if(arguments[total_arguments-1][strlen(arguments[total_arguments-1]) - 1] == '\n') {
                arguments[total_arguments-1][strlen(arguments[total_arguments-1]) - 1] = '\0';
            }

            pid_t pid;    
            pid = fork(); 
            if(pid == 0) {
                execvp(arguments[0], arguments);  
                exit(0);
            } 
            else {
                wait(&status); 
                printf("Parent: A Child which had %d ID is terminated with status %d\n\n", pid, WEXITSTATUS(status));
                write(1,"******************************\n\n",32);
        }
    }
}
    return 0;
}
    