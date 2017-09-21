#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "mish.h"
#include "sighant.h"
#include "parser.h"
#include "testProgram.h"
#include "split.h"

static void fail(char* str);
char **splitString(char *s, char separator);
int mish();

int main(int argc, char **argv){
    if(strcmp(argv[argc-1],"test")==0){
        return test();
    }
    return mish();
}
int mish(){
    if(signal(SIGINT,signalCommand) == SIG_ERR)
      printf("\nCan't cath SIGINT\n");

    FILE *fp; 
    fp=stdin; 
    char *commandString = malloc(1024);
    fail(commandString);

    command comLine[2 + 1];
    while(fgets(commandString, 1024,fp)!=NULL){
        int numberOfCommands = parse(commandString,comLine);
        printf("%d", numberOfCommands);

        //Kollar om det är echo eller cd
        command myCommands = comLine[0];
        if(strcmp(myCommands.argv[0],"echo")== 0){
            printf("%s %s\n", myCommands.argv[0], myCommands.argv[1]);
        }

        else if(strcmp(myCommands.argv[0],"cd")== 0){
            changecwd(myCommands.argv[1]);
        }

        else{
            pid_t childProcessID;

            //ParameterLista till exec
            size_t sizeOfProgram = strlen(myCommands.argv[0]);
            char *tmpString = "/bin/";
            char *newDirectory = calloc((sizeOfProgram+5) ,sizeof(char*));
            newDirectory[0] = '\0';
            strcat(newDirectory,tmpString);
            strcat(newDirectory,myCommands.argv[0]);

            char *parameterList[] = { newDirectory,"sorterad.txt", 0};
            printf("%s\n%s\n", parameterList[0], parameterList[1]);
            childProcessID = fork();

            //Kollar om barnets ID är något annat än 0 i så fall blev det fel
            if(childProcessID < 0){
                perror("Problem with fork");
                exit(1);
            }
            else if(childProcessID == 0){
                if(execv(parameterList[0],parameterList)<0){
                    perror("Exec:");
                    exit(1);
                }
            }
            else{
                int status;
                /* Föräldraprocessen */
                wait(&status);
                printf("Parent signing off. Child exited with status %d \n",status);
                printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
                printf("WIFEXITED: %d\n", WIFEXITED(status));
                printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
                printf("WIFSTOPPED: %d\n", WIFSTOPPED(status));
            }
        }        

    }
    free(commandString);
    return 0; 
}

/**
 * Name: change cwd
 * Description: Changing the working directory.
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv){
    char *buff;
    char *currentDirectory;
    const char *separator2 = "/";

    if((buff = (char*)malloc(1024))!=NULL){
        currentDirectory=getcwd(buff,1024);
    }

    else{
        fprintf(stderr, "malloc failed");
        exit(1);
    }

    strcat(currentDirectory,separator2);
    strcat(currentDirectory,argv);
    if(chdir(currentDirectory)==-1){
        perror("chdir() failed \n");
    }

    currentDirectory=getcwd(buff,1024);
    printf("Print Current directory %s\n", currentDirectory);
    free(buff);
}
/**
 * Name: Join
 * Description: Function for joining strings togheter.
 * @param length (size of matrix)
 * @param stringToJoin (matrix with strings to join)
 * @param separator (char to separate the strings)
 * @return (the new joined string)
 */
char *join(int length, char **stringToJoin, const char *separator){
    int allocatedlength = 0;
    for(int i =0; i<length; i++){
        int strLength = strlen(stringToJoin[i]);
        allocatedlength = strLength +allocatedlength +1;
    }
    char *returnString = calloc(allocatedlength+1, sizeof(char));
    for(int i = 0; i<length; i++){
        strcat(returnString,separator);
        strcat(returnString, stringToJoin[i]);
    }
    return returnString;
}

static void fail(char* str) {
    if(str == NULL){
        fprintf(stderr, "ERROR: %s\n", str);
        exit(EXIT_FAILURE);
    }
}
    
