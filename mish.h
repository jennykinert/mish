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

char *getPath(char *path);