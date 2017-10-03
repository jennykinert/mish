#include "mish.h"
pid_t childID[100];
static void fail(char* str);
//int mish();

/**int main(int argc, char **argv){
    if(strcmp(argv[argc-1],"test")==0){
        return test();
    }
    return mish();
}*/
int main(){
    setUpSignalHandeling();
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
                    childID[i]=createChildWithoutPipe(fd,parameterList,myCommands);
                    waitForChild();
                }
                else {
                    if(i == 0){
                        childID[i]=createChildWrite(fd, parameterList,myCommands);
                    }
                    else if(i == numberOfCommands-1){
                        childID[i]=createChildRead(fd,parameterList,myCommands);
                    }
                    else{
                        childID[i]=createReadAndWriteChild(fd, parameterList);
                    }
                }
                char *temp = parameterList[0];
                free(temp);
                free(parameterList);
            }
        }
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        waitForChild();
    }
    fclose(fp);
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
 * Name getExecParam
 * Description constructs the parameterList to be used when starting a new
 * process with execvp
 * @param myCommand
 * @return parameterList (matrix of parameters)
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
 * Description creates a child that can only write to the file descriptor
 * @param fd (file descriptor)
 * @param parameterList
 */
pid_t createChildWrite(int fd[], char **parameterList, command myCommand){
    pid_t pid = fork();
    checkFork(pid);
    if(pid == 0){
        //Handle if a new inputFile is chosen
        if(myCommand.infile != NULL){
           redirectInFile(fd, myCommand.infile);
        }
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
    return pid;
}
/**
 * Name createChildRead
 * Description Creates a child that can only read from the filedescriptor
 * @param fd (file descriptor)
 * @param parameterList
 */
pid_t createChildRead(int fd[], char **parameterList, command myCommand){
    pid_t pid = fork();
    checkFork(pid);
    if(pid == 0){
        if(myCommand.outfile != NULL){
            redirectOutFile(fd, myCommand.outfile);
        }
        dup2(fd[READ_END], STDIN_FILENO);
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
    return pid;
}

/**
 * Name createReadAndWriteChild
 * Description creates a child that can pipe to both read and write of the
 * filedescriptor
 * @param fd
 * @param parameterList
 */
pid_t createReadAndWriteChild(int fd[], char **parameterList){
    pid_t pid = fork();
    checkFork(pid);
    if(pid == 0){
        dup2(fd[READ_END], STDIN_FILENO);
        dup2(fd[WRITE_END],STDOUT_FILENO);
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
    return pid;
}

/**
 * Name createChildWithoutPipe
 * Description used when only one external command is supposed to be executed
 * here no pipes are created.
 * @param parameterList
 */
pid_t createChildWithoutPipe( int fd[2], char **parameterList, command myCommand){
    pid_t pid= fork();
    checkFork(pid);
    if(pid == 0){
        if(myCommand.outfile != NULL){
            redirectOutFile(fd, myCommand.outfile);
        }
        if(myCommand.infile != NULL){
            redirectInFile(fd,myCommand.infile);
        }
        if(execvp(parameterList[0],parameterList)<0){
            perror("Exec:");
            exit(1);
        }
    }
    return pid;
}

/**
 * Name redirectInFile
 * Description: Function to redirect input to a file
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectInFile(int fd[], char *infile){
    int file = open(infile ,O_RDONLY, S_IROTH);
    if(file < 0){
        perror("Open:");
        exit(1);
    }
    dup2(file,STDIN_FILENO);
    dup2(fd[READ_END],file);
    close(file);
}
/**
 * Name redirectOutFile
 * Description: Function to redirect output to a file
 * @param fd (filedescriptor)
 * @param outfile (name of file to redirect to)
 */
void redirectOutFile(int fd[], char *outfile){
    int file = open(outfile ,O_WRONLY, O_APPEND);
    if(file < 0){
        perror("Open:");
        exit(1);
    }
    dup2(file,STDOUT_FILENO);
    dup2(fd[WRITE_END],file);
    close(file);
}
/**
 * Name waitForChild
 * Description waits for all the childs created during the execproce
 * ss
 */
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

static void fail(char* str) {
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
void checkFork(int fork){
    if(fork < 0){
        perror("Problem with fork");
        exit(1);
    }
}


void setUpSignalHandeling(){
    struct sigaction act;
    act.sa_handler = &signalCommand;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror ("sigaction");
        exit(1);
    }

}
    
