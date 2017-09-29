#include "mish.h"

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
    command comLine[MAX_COMMAND];


    while(fgets(commandString, 1024,fp)!=NULL){
        int fd[2];
        pipe(fd);
        int numberOfCommands = parse(commandString,comLine);
        printf("%d", numberOfCommands);

        for(int i=0; i<numberOfCommands; i++){

            //Kollar om det är echo eller cd
            command myCommands = comLine[i];
            if(strcmp(myCommands.argv[0],"echo")== 0){
                printf("%s %s\n", myCommands.argv[0], myCommands.argv[1]);
            }

            else if(strcmp(myCommands.argv[0],"cd")== 0){
                changecwd(myCommands.argv[1]);
            }

            else{
                char **parameterList = getExecParam(myCommands);
                if(numberOfCommands == 1){
                    createChildWithoutPipe(parameterList);
                    waitForChild();
                }
                else {
                    if(i == 0){
                        createChildWrite(fd, parameterList);
                    }
                    else if(i == numberOfCommands-1){
                        createChildRead(fd,parameterList);
                    }
                    else{
                        createReadAndWriteChild(fd, parameterList);
                    }
                }
            }
        }
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        waitForChild();
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
 *
 * @param myCommand
 * @return
 */
char **getExecParam(command myCommand){
    char *newDirectory = getPath(myCommand);
    char **parameterList = calloc(myCommand.argc+1,sizeof(char *));
    parameterList[0]=newDirectory;

    for (int i = 1; i < myCommand.argc; ++i) {
        parameterList[i] = myCommand.argv[i];
    }
    parameterList[myCommand.argc] = 0;
    return parameterList;
}
/**
 * Name: getPath
 * Description: Gets the path of the external program to be executed
 * @param myCommand
 * @return (pointer to path)
 */
char *getPath(command myCommand){
    size_t sizeOfProgram = strlen(myCommand.argv[0]);
    char *tmpString = "/bin/";
    char *newDirectory = calloc((sizeOfProgram+5) ,sizeof(char*));
    newDirectory[0] = '\0';
    strcat(newDirectory,tmpString);
    strcat(newDirectory,myCommand.argv[0]);
    fprintf(stderr,"%s\n",newDirectory);
    return newDirectory;
}
/**
 * Name createChildWrite
 * @param fd
 * @param parameterList
 */
void createChildWrite(int fd[], char **parameterList){
    pid_t pid = fork();
    if(pid < 0){
        perror("Problem with fork");
        exit(1);
    }
    else if(pid == 0){
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
}
/**
 * Name createChildRead
 * @param fd
 * @param parameterList
 */
void createChildRead(int fd[], char **parameterList){
    pid_t pid = fork();
    if(pid < 0){
        perror("Problem with fork");
        exit(1);
    }
    else if(pid == 0){
        dup2(fd[READ_END], STDIN_FILENO);
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
}

void createReadAndWriteChild(int fd[], char **parameterList){
    pid_t pid = fork();
    if(pid < 0){
        perror("Problem with fork");
        exit(1);
    }
    else if(pid == 0){
        dup2(fd[READ_END], STDIN_FILENO);
        dup2(fd[WRITE_END],STDOUT_FILENO);
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
}

void createChildWithoutPipe( char **parameterList){
    pid_t pid= fork();
    if(pid < 0){
        perror("Problem with fork");
        exit(1);
    }
    else if(pid == 0){
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
}


void waitForChild(){
    int pid;
    int status;
    while((pid = wait(&status)) != -1){
        printf("Process %d \n",pid);
        printf("WEXITSTATUS: %d\n", WEXITSTATUS(status));
        printf("WIFEXITED: %d\n", WIFEXITED(status));
        printf("WIFSIGNALED: %d\n", WIFSIGNALED(status));
        printf("WIFSTOPPED: %d\n", WIFSTOPPED(status));
    }
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
    
