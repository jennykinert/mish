#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "mish.h"
#include "sighant.h"
#include "parser.h"
#include "testProgram.h"
/**
 * Function for changing the working directory
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv);
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
            printf("Jag gick in i CD\n");
            changecwd(myCommands.argv[1]);
        }

        else{
            printf("Här ska det hittas på roligheter med externa kommandon");
        }        

    }

    return 0; 
}

void changecwd(char *argv){
    char *buff;
    char *ptr;
    if((buff = (char*)malloc(1024))!=NULL){
        ptr=getcwd(buff,1024);
        printf("%s", ptr);
    }

    if(strcmp(argv,"..")){

    }
}

char **splitString(char *s, char separator){

    size_t stringLenght = strlen(s);
    int separatorCounter = 0;
    char *buff = malloc(1024);
    int n =0;
    for(int i=0; i<stringLenght; i++){
        separatorCounter++;
    }

    char **stringArray = malloc(separatorCounter*sizeof(char*));
    int buffCounter = 0;

    for(int i =0; i<stringLenght; i++){

        if(s[i] != separator){
            buff[buffCounter] = s[i];
            buffCounter++;
        }
        else{
            char *splitString = malloc((buffCounter*sizeof(char))+1);
            buff[i]='\0';
            strncpy(splitString,buff,1023);
            splitString[buffCounter]='\0';
            stringArray[n] = splitString;
            n++;
            buffCounter=0;
        }
    }
    //free(buff);
    return stringArray;
}
    
