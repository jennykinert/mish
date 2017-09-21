//
// Created by id14jkt on 2017-09-19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mish.h"
#include "testProgram.h"

void testSplitCorrectNumberOfSplits();
void testJoin();

int test(void){
    testSplitCorrectNumberOfSplits();
    testJoin();
    return 0;
}

void testSplitCorrectNumberOfSplits(){

    /*char *str= "my/path/is/three\n";
    char separator = '/';
    char **returnString;
    returnString = splitString(str,separator);
    printf("%s %s %s %s",returnString[0],returnString[1], returnString[2],
    returnString[3]);
    assert(strcmp(returnString[0],"my")==0);
    assert(strcmp(returnString[1],"path")==0);
    assert(strcmp(returnString[2],"is")==0);
    assert(strcmp(returnString[3],"three")==0);*/
}

void testJoin(){
    char **testMatrix = calloc(3,sizeof(char**));
    testMatrix[0]="my";
    testMatrix[1]="test";
    testMatrix[2]="path";

    const char* separator = "/";
    int size = 3;
    char *newstring;
    newstring = join(size, testMatrix, separator);

    assert(strncmp(newstring, "my/test/path", 12) == 0);
    free(testMatrix);
    free(newstring);
    printf("Test: testJoin succeded\n");
}

/**void testStepBackwardscwd(){
    char *argv = "..";
    changecwd(argv);
}*/