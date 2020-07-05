#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "builtins.h"

int main() {
    char *userInput;
    char **parsedInput;

    while(1){
        //grab user input
        shellEnvr(parsedInput);
        userInput = grabInput();
        //printf("User Input is: %s\n", userInput);

        //parsed user input
        parsedInput = parseInput(userInput);
        //printStrArr(parsedInput);

        //if exit commmand then quit program

        exitProgram(parsedInput);

        // else evaul parsed input
        //printf("EVALUATING ARGS: \n");
        eval(parsedInput);
    } // end while

} // end main