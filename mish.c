/* <mish>.c
 *  <Jenny> <Kinert>
 *  Laboration <3 mish> <5DV088> <HT17>
 *  <A simple shell acting as a command promt>
 */

#include "mish.h"
pid_t childID[MAXCOMMANDS];
static void controlMemoryAllocated(char* str);
int main(){
    mysignal(SIGINT,setUpSignalHandeling);
    FILE *fp;
    fp=stdin;
    char *commandString = malloc(1024);
    controlMemoryAllocated(commandString);
    command comLine[MAXCOMMANDS];
    fprintf(stderr, "mish%% ");
    fflush(stderr);
    while(fgets(commandString, 1024,fp)!=NULL){
        if(commandString[0]=='\n'){
            continue;
        }
        int numberOfCommands = parse(commandString,comLine);
        int fd[2];
        int tempFD[2];

        for(int i=0; i<numberOfCommands; i++){
            //Kollar om det är echo eller cd
            command myCommands = comLine[i];
            if(strcmp(myCommands.argv[0],"echo")== 0){
                int argCounter=1;
                while(argCounter < myCommands.argc){
                    if(argCounter == myCommands.argc - 1){
                        printf("%s", myCommands.argv[argCounter]);
                    }
                    else{
                        printf("%s ", myCommands.argv[argCounter]);
                    }
                    argCounter++;
                }
                printf("\n");
            }
            else if(strcmp(myCommands.argv[0],"cd")== 0){
                changecwd(myCommands.argv[1]);
            }
            else{
                checkSystemCalls(pipe(fd),"pipe");
                char **parameterList = getExecParam(myCommands);
                if(numberOfCommands == 1){
                    childID[i]=createChildWithoutPipe(fd,parameterList,
                                                      myCommands);
                }
                else if(i == 0){
                    childID[i]=createChildWrite(fd,parameterList,myCommands);
                    checkSystemCalls(close(fd[WRITE_END]),"close");
                    tempFD[READ_END] = fd[READ_END];
                }
                else if(i == numberOfCommands-1){
                    childID[i]=createChildRead(tempFD,fd,parameterList,
                                               myCommands);
                    checkSystemCalls(close(fd[READ_END]),"close");
                    checkSystemCalls(close(fd[WRITE_END]),"close");
                    checkSystemCalls(close(tempFD[READ_END]),"close");
                }
                else{
                    childID[i]=createReadAndWriteChild(tempFD,fd,parameterList,
                                                       myCommands);
                    checkSystemCalls(close(fd[WRITE_END]),"close");
                    checkSystemCalls(close(tempFD[READ_END]),"close");
                    tempFD[READ_END]=fd[READ_END];
                }
                free(parameterList);
            }
        }
        waitForChild();
        fprintf(stderr, "mish%% ");
        fflush(stderr);
    }
    checkSystemCalls(fclose(fp),"close");
    free(commandString);
    return 0;
}

/**
 * Name: change cwd
 * Description: Changing the working directory.
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv){
    if(chdir(argv)==-1){
        perror(argv);
    }
}
/**
 * Name getExecParam
 * Description constructs the parameterList to be used when starting a new
 * process with execvp
 * @param myCommand
 * @return parameterList (matrix of parameters)
 */
char **getExecParam(command myCommand){
    char **parameterList = calloc(myCommand.argc+1,sizeof(char *));
    if(parameterList == NULL){
        fprintf(stderr, "ERROR: %s\n", parameterList[0]);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < myCommand.argc; ++i) {
        parameterList[i] = myCommand.argv[i];
    }
    parameterList[myCommand.argc] = 0;
    return parameterList;
}
/**
 * * Name createChildWrite
 * Description creates a child that can only write to the file descriptor
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildWrite(int fd[], char **parameterList, command myCommand){
    pid_t pid = fork();
    checkSystemCalls(pid,"fork");
    if(pid == 0){
        //Handle if a new inputFile is chosen
        if(myCommand.infile != NULL){
           redirectInFile(fd, myCommand.infile);
        }
        checkSystemCalls(dup2(fd[WRITE_END], STDOUT_FILENO),"dup2");
        checkSystemCalls(close(fd[READ_END]),"close");
        checkSystemCalls(close(fd[WRITE_END]),"close");
        if(execvp(parameterList[0],parameterList)<0){
            perror(parameterList[0]);
            exit(1);
        }
    }
    return pid;
}
/**
 * Name createChildRead
 * Description Creates a child that can only read from the filedescriptor
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildRead(int tempfd[],int fd[], char **parameterList,
                      command myCommand){
    pid_t pid = fork();
    checkSystemCalls(pid,"fork");
    if(pid == 0){
        if(myCommand.outfile != NULL){
            redirectOutFile(fd, myCommand.outfile);
        }
        checkSystemCalls(dup2(tempfd[READ_END], STDIN_FILENO),"dup2");
        checkSystemCalls(close(fd[WRITE_END]),"close");
        checkSystemCalls(close(fd[READ_END]),"close");
        checkSystemCalls(close(tempfd[READ_END]),"close");
        if(execvp(parameterList[0],parameterList)<0){
            perror(parameterList[0]);
            exit(1);
        }
    }
    return pid;
}

/**
 * Name createReadAndWriteChild
 * Description creates a child that can pipe to both read and write of the
 * filedescriptor.
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createReadAndWriteChild(int tempfd[], int fd[],char **parameterList,
                              command myCommand){
    pid_t pid = fork();
    checkSystemCalls(pid,"fork");
    if(myCommand.infile !=NULL || myCommand.outfile !=NULL){
        fprintf(stderr,"Invalid format: cannot redirect to in our outfile");
        exit(1);
    }
    if(pid == 0){
        checkSystemCalls(dup2(tempfd[READ_END], STDIN_FILENO),"dup2");
        checkSystemCalls(dup2(fd[WRITE_END],STDOUT_FILENO),"dup2");
        checkSystemCalls(close(fd[WRITE_END]),"close");
        checkSystemCalls(close(fd[READ_END]),"close");
        checkSystemCalls(close(tempfd[READ_END]),"close");
        if(execvp(parameterList[0],parameterList)<0){
            perror(parameterList[0]);
            exit(1);
        }
    }
    return pid;
}

/**
 * Name createChildWithoutPipe
 * Description used when only one external command is supposed to be executed
 * here no pipes are created.
 * @param fd
 * @param parameterList
 * @param myCommand
 * @return
 */
pid_t createChildWithoutPipe( int fd[2], char **parameterList,
                              command myCommand){
    pid_t pid= fork();
    checkSystemCalls(pid,"fork");
    if(pid == 0){
        if(myCommand.outfile != NULL){
            redirectOutFile(fd, myCommand.outfile);
        }
        if(myCommand.infile != NULL){
            redirectInFile(fd,myCommand.infile);
        }
        if(execvp(parameterList[0],parameterList)<0){
            perror(parameterList[0]);
            exit(1);
        }
    }
    return pid;
}

/**
 * Name redirectInFile
 * Description: Function to redirect input to a file
 * @param fd (filedescriptor)
 * @param infile (name of file to read from)
 */
void redirectInFile(int fd[], char *infile){
    int file = open(infile ,O_RDONLY, S_IROTH);
    if(file < 0){
        perror(infile);
        exit(1);
    }
    checkSystemCalls(dup2(file,STDIN_FILENO),"dup2");
    checkSystemCalls(dup2(fd[READ_END],file),"dup2");
    checkSystemCalls(close(file),"close");
}
/**
 * Name redirectOutFile
 * Description: Function to redirect output to a file. If the outfile exists
 * the redirect fails.
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectOutFile(int fd[], char *outfile){
    int file = open(outfile ,O_WRONLY | O_CREAT | O_EXCL);
    if(file < 0){
        perror(outfile);
        exit(1);
    }
    checkSystemCalls(dup2(file,STDOUT_FILENO),"dup2");
    checkSystemCalls(dup2(fd[WRITE_END],file),"dup2");
    checkSystemCalls(close(file),"close");
}
/**
 * Name waitForChild
 * Description waits for all the childs created during the execproce
 * ss
 */
void waitForChild(){
    int status;
    while((wait(&status)) != -1);

}

/**
 * Name controlMemoryAllocated
 * Description controls if function systemcalls was allocated correctly
 * @param str
 */
static void controlMemoryAllocated(char* str) {
    if(str == NULL){
        fprintf(stderr, "ERROR: %s\n", str);
        exit(EXIT_FAILURE);
    }
}
/**
 * Name checkFork
 * Description controls if a fork command is executed correct.
 * @param fork
 */
void checkSystemCalls(int fork, char *systemcall){
    if(fork < 0){
        perror(systemcall);
        exit(1);
    }
}

/**
 * Name setUpSignalHandeling
 * Descriprion A signalhandeler if SIGINT is send to the program
 */
void setUpSignalHandeling(){
    int i =0;
    while(childID[i] != 0) {
        kill(childID[i],SIGKILL);
        i++;
    }
}

