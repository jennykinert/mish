#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "sighant.h"
#include "parser.h"
#include "testProgram.h"

#define WRITE_END  1
#define READ_END 0
#define MAX_COMMAND 5

/**
 * Name: change cwd
 * Description: Changing the working directory. If the user write .. the
 * function exits the current directory and if another string is used
 * function tries to enter new directory if fail an error is printed.
 * @param argv (new file to enter or .. to exit directory)
 */
void changecwd(char *argv);

/**
 * Name: Join
 * Description: Function for joining strings togheter.
 * @param length (size of matrix)
 * @param stringToJoin (matrix with strings to join)
 * @param separator (char to separate the strings)
 * @return (the new joined string)
 */
char *join(int length, char **stringToJoin, const char* separator);

/**
 * Name: getPath
 * Description: Gets the path of the external program to be executed
 * @param myCommand
 * @return (pointer to path)
 */
char *getPath(command myCommand);

/**
 *
 * @param myCommand
 * @return
 */
char **getExecParam(command myCommand);

void createChildWrite( int fd[], char **parameterList);
void createChildRead(int fd[], char **parameterList);
void createChildWithoutPipe(char **parameterList);
void createReadAndWriteChild(int fd[], char **parameterList);
void waitForChild();