#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "mish.h"
#include "sighant.h"
#include "parser.h"
/**
 * Function for changing the working directory
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv);
char **splitString(char *s, char separator);

int main(void){
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

    int stringLenght = strlen(s);
    int separatorCounter = 0;
    char buff[1024];
    int n =0;
    printf("Debug: String length %d", stringLenght);
    for(int i=0; i<stringLenght; i++){
        separatorCounter++;
    }
    char **stringArray = malloc(separatorCounter*sizeof(char*));
    for(int i =0; i<stringLenght; i++){
        if(s[i] != separator){
            buff[i]=s[i];
        }
        else{
            char *splitString = malloc(i*sizeof(char));
            strcpy(splitString,buff);
            stringArray[n] = splitString;
            n++;
        }
    }
    return stringArray;
}
    
