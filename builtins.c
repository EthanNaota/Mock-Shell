#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_STRING 1028

int eval(char ** args);

char * grabInput(){
/* GETS USER INPUT AND REMOVES NEWLINE AT THE END OF THE STRING.
 * RETURNS STRING */
    char * userString = malloc (sizeof(char *) * 64);

    // get user input
    fgets(userString, MAX_STRING, stdin);

    /* if user entered nothing return an empty string
     * else remove newline character */
    if((strcmp(userString, "\n") == 0 )|| (strcmp(userString, "\0") == 0)){
        strcpy(userString, "");
    } else {
        strtok(userString, "\n");
    } // end if else

    return userString;
} // end grabinput

char ** parseInput(char * userinput){
/* PARSES A STRING INTO AN ARRAY OF STRINGS BROKEN UP BY
 * SPACE CHARACTERS */
    char breakChar[2] = " ";
    char *token;
    char **tokenArray = malloc (sizeof(char **) * 64);
    int i = 0;

    /* if the users input is empty then dont do anything
     * else break up into tokens by spaces and add the
     * tokens to an array */
    if(strcmp(userinput, "") == 0){

    } else {
        token = strtok(userinput, breakChar);

        while(token != NULL){
            tokenArray[i] = token;
            token = strtok(NULL, breakChar);
            i++;
        } // end while
    } // end if else

    // add a NULL to end the array and allow iteration
    tokenArray[i] = NULL;

    return tokenArray;
} // end parseinput

void printStrArr(char **userInput){
/* PRINTS AN ARRAY OF STRINGS, USED FOR TESTING */
    int i = 0;

    while(userInput[i] != NULL){
        printf("String %d: %s\n", i, userInput[i]);
        i++;
    } // end while

    return;
} // end printstrarr

int charExists(char ** args, char *var){
/* FINDS IF A STRING EXISTS IN AN ARRAY OF STRINGS AND
 * RETURNS -1 IF NOT FOUND OR THE ARRAY POSITION WHERE
 * THE STRING IS PRESENT */
    int i = 0;

    while(args[i] != NULL){
        if(strcmp(args[i], var) == 0){
            return i; // return position

        } // end if
        i++;
    } // end while

    return -1;
} // end charexists

int isPipe(char ** args){
// returns -1 if pipe doesn't exist otherwise returns the arg it exists on
    return charExists(args, "|");
} // end ispipe

int isOutputToFile(char ** args){
// returns -1 if > doesn't exist otherwise returns the arg it exists on
    return charExists(args,">");
} // end isoutputtofile

int isOutputToFileAppend(char ** args){
// returns -1 if >> doesn't exist otherwise returns the arg it exists on
    return charExists(args, ">>");
} // end isoutputtofileappend

int isInputRedirect(char ** args){
//returns -1 if < doesn't exist otherwise returns the arg it exists on
    return charExists(args,"<");
} // end isinputredirect

int cd_cmd(char ** args){
/* FIRST ARG IS 'CD', SECOND IS 'FOLDER LOCATION' AND CHANGES
 * THE ENVIRON PATH AND FOLDER PATH */
    if((args[1] == NULL)){

        // if the folder path arg is empty return
        printf("MockShell: expected folder path argument\n");
        return 0;

    } else if(args[2] == NULL){

        // if the 3rd arg is null then execute code and see if the directory exits
        if(chdir(args[1]) != 0){
            printf("MockShell: directory not found\n");
            return 0;
        } // end if

        char * test = malloc(sizeof(char *)*64);
        getcwd(test, 1024); // get the full path name of the new directory

        setenv("PWD", test, 1); // set the environ variable to new directory

        return 1;
    } else {
        // too many arguments
        printf("MockShell: too many arguments included \n");
        return 0;
    } // end if else if else

} // end cdcmd

int clrScreen(char ** args){
/* CLEARS SCREEN BY PRINTING A NEWLINE 60 TIMES */
    int i = 0;

    while(i < 60){
        printf("\n");
        i++;
    } // end while
    return 1;
} // end clrscreen

int printDirectory(char ** args){
/* PRINTS THE FILES IN THE DIRECTORY STORED IN ENVIRON 'PWD' */

    struct dirent *de;                  // stores a file system directory entry
    DIR *dr = opendir(getenv("PWD"));   // stores directory path

    /* iterate through the directories files
     * and print the file names */
    while((de=readdir(dr))!= NULL){
        printf("%s \n", de->d_name);
    } // end while

    closedir(dr);
    return 1;
} // end printDirectory

int printEnviron(char ** args){
/* PRINT THE ENVIRONMENT VARIABLES */
    char * envVars[] = {
            "LANG",
            "PWD",
            "USER",
            "DESKTOP_SESSION",
            "HOME",
            "XDG_SESSION_TYPE",
            "XDG_SESSION_DESKTOP",
            "XDG_CURRENT_DESKTOP",
            "SHELL",
            "LOGNAME",
            "PATH"
    }; // array of environment names

    int i = 0;
    while(i < 11){
        printf("%s = %s\n", envVars[i], getenv(envVars[i]));
        i++;
    } // end while
    return 1;
} // end printenviron

int echo(char ** args){
/* PRINTS ARGS AFTER ECHO */

    int i = 1; // start at array[1]
    while(args[i] != NULL){
        printf("%s ", args[i]);
        i++;
    } //end while

    printf("\n");

    return 1;
} // end echo

int printHelp(char ** args){
    /* print help file */
    if(args[1] == NULL){
        printf("Pass ARG of command you need information for \n");
        printf("Passable ARGS: cd, clr, dir, environ, echo, help, EXIT\n");
    } else if(args[2] != NULL){
        printf("Too many ARGS");
    } else if(strcmp(args[1], "cd") == 0){
        printf("Takes in a path as the argument. If the directory exists \n"
               "it will change to the directory specified. Will also set \n"
               "the PWD environment variable to the current path.\n");
    } else if(strcmp(args[1], "dir") == 0){
        printf("Clears the screen by printing a newline 60 times.\n");
    } else if(strcmp(args[1], "environ") == 0){
        printf("Prints all of the directories and files in the current \n"
               "directory. \n");
    } else if(strcmp(args[1], "echo") == 0){
        printf("Prints out the current environment variables for: LANG, \n"
               "PWD, USER, DESKTOP_SESSION, HOME, XDG_SESSION_TYPE, \n"
               "XDG_SESSION_DESKTOP, XDG_CURRENT_DESKTOP, SHELL, LOGNAME, \n"
               "and PATH.\n");
    } else if(strcmp(args[1], "help") == 0){
        printf("Takes in ARGS as a string and prints everything after the \n"
               "echo. Will be stopped by pipes and/or input or output \n"
               "redirection\n");
    } else if(strcmp(args[1], "EXIT") == 0){
        printf("Takes in an ARG that prints the help options for the built \n"
               "in commands created\n");
    } else {
        printf("MockShell: Command does not exists. \n");
        printf("Passable ARGS: cd, clr, dir, environ, echo, help, EXIT\n");
    }
    return 1;
} // end printHelp

void shellEnvr(char ** args){
/* PRINT SHELL CMDLINE PROMPT - PRINTS *USER*: *PATH*$ */
    printf("%s: %s$ ", getenv("USER"), getenv("PWD"));
} // end shellenvr

short int isBackground(char ** args){
/* BOOLEAN - RETURNS 1 IF BACKGROUND PROCESS */
    int i = 0;

    while(args[i] != NULL){
        i++;
    } // end while
    i--;

    // if last arg is '&'
    if(strcmp(args[i], "&") == 0){
        return 1; // is background
    } // end if

    return 0;
} // end isBackground

short int isBuiltinFunction(char ** args){
/* BOOLEAN - RETURNS ONE IF ARG[0] IS A BUILT IN
 * FUNCTION */
    char * cmdNames[] = {
            "cd",
            "clr",
            "dir",
            "environ",
            "echo",
            "help",

    }; // built in function names

    int i = 0;

    while(i < 6){
        if(strcmp(args[0],cmdNames[i]) == 0){
            return 1; // returns is built in function
        } // end if
        i++;
    } // end while
    return 0;

} // end isbuiltinfunction

int runBuiltinFunctions(char ** args){
/* RUNS BUILT IN FUNCTIONS */
    int (*builtinFunctions[])(char **) = {
            &cd_cmd,
            &clrScreen,
            &printEnviron,
            &printDirectory,
            &echo,
            &printHelp
    }; // function pointers to builtin functions

    char * cmdNames[] = {
            "cd",
            "clr",
            "environ",
            "dir",
            "echo",
            "help",

    }; // names of functions relative to builtin functions

    int i = 0;

    //
    while (i < 6){
        if(strcmp(args[0], cmdNames[i]) == 0){
            // run builtin function
            return (*builtinFunctions[i])(args);
        } // end if
        i++;
    } // end while
    return 1;
} // end runbuiltinfunctions

int linuxCmds(char ** args){
/* FORKING FOR LINUX EXECUTABLE COMMANDS */
    pid_t pid, pid1;

    pid = fork();

    if(pid == 0){
    // child process
        //execute linux commands
        if(execvp(args[0], args) == -1){
            perror("MockShell: Error executing outside commands");
        }

        exit(0); // exit child process
    } else if(pid < 0){
        perror("Mockshell: Error Forking");


    } else {
        // parent process

        // if run in background then skip wait
        if((isBackground(args)) == 0) {
            int status;
            pid1 = waitpid(pid, &status, 0);
        } // end if

    } // end if else fork

    return 1;
} // end linuxcmds

char ** splitCommands(char ** args, int a, int b){
/* CREATES A NEW ARRAY OF ARGS FROM STARTING AT ARG[A]
 * AND ENDING WITH ARG[B] */
    char ** split = (malloc (sizeof(char *) * 64));
    int count = a;
    int i = 0;

    // something went wrong or no args return NULL
    if(a == b){
        split[0] = NULL;
        return split;
    } // end if

    while(count < b){
        split[i] = args[count];

        i++;
        count++;
    } // end while

    split[++i] = NULL; // end array with NULL
    return split;
} // end splitcommands

int lengthOfArray(char ** args){
/* RETURNS THE LENGTH OF AN ARRAY */
    int i = 0;

    while(args[i] != NULL){
        i++;
    } // end while
    return i;
} // end lengthofarray

int outputRedirect(char ** args, char * fileLoc, int isAppend){
/* REDIRECTING OUTPUT TO FILELOC */
    int pid = fork();

    if(pid == 0){
        // child

        int out;

        if(isAppend){
        // open file with append
            out = open(fileLoc, O_WRONLY|O_CREAT|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
        } else {
        // open file with no append
            out = open(fileLoc, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
        } // end if else

        close(1);       // close read
        dup2(out, 1);   // change write to file
        close(out);     // close write file

        // run builtinFunctions otherwise linux commands
        if(isBuiltinFunction(args)){
            runBuiltinFunctions(args);
        } else {
            linuxCmds(args);
        } // end if else
        exit(0); // EXIT CHILD

    } else {
    // parent process
        // wait for child process to end
        int status = 0;
        wait(&status);
    } // end if else fork
    return 0;
} // end outputredirect

int inputRedirect(char ** args, char * fileLoc){
/* REDIRECTING INPUT TO FILELOC */
    int pid = fork();

    if(pid == 0){
        // child changes stdin from file

        int in = open(fileLoc, O_RDONLY);

        close(0);       // close write
        dup2(in, 0);    // change read from file
        close(in);      // close read

        /* run linux command */
        if(execvp(args[0], args) == -1){
            perror("MockShell: Error executing outside commands");
        } // end if

        exit(0); // EXIT CHILD

    } else {
        // parent waits for child
        int status;
        wait(&status);
    } // end if else fork
    return 0;
} // end inputredirection

int outputInputRedirection(char ** args, char * fileIn, char * fileOut, int isAppend){
/* COMBINES OUTPUT AND INPUT REDIRECTION FROM 'FILEIN' TO 'FILEOUT' */
    int pid = fork();

    if(pid == 0){
        // child changes file descriptors
        int out;

        if(isAppend){
            // open file with append
            out = open(fileOut, O_WRONLY|O_CREAT|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
        } else {
            // open file without append
            out = open(fileOut, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
        } // end if else

        int in = open(fileIn, O_RDONLY); // open read file

        // close write, change read, close read
        close(0);
        dup2(in, 0);
        close(in);

        // close read, change write, close write
        close(1);
        dup2(out, 1);
        close(out);

        // execute command with file redirection
        if(execvp(args[0], args) == -1){
            perror("MockShell: Error executing outside commands");
        } // end if

        exit(0); // EXIT CHILD PROCESS

    } else {
        // parent waits

        int status;
        wait(&status);
    } // end if else fork
} // end input output redirection

int pipeFunctions(char ** argsIn, char ** argsOut){
/* PIPE FUNCTION TAKES IN ARGIN AS THE WRITER AND
 * ARGOUT AS THE ARGS TO READ THE OUTPUT FROM ARGIN*/
    pid_t pid;
    int fd[2]; // pipe
    int READ = 0;
    int WRITE = 1;

    if (pipe(fd) == -1) {
        printf("MockShell: Pipe error.\n");
        return 0;
    } // end if

    if ((pid = fork()) == -1) {
        printf("MockShell: Fork error.\n");
        return 0;

    } else if (pid == 0) {
        // child sends output to parent

        // close read, change write, close write
        dup2(fd[WRITE], 1);
        close(fd[WRITE]);
        close(fd[READ]);

        // if it is a built in challenge run built in else run linux cmd
        if (isBuiltinFunction(argsIn) == 1) {
            runBuiltinFunctions(argsIn);
            fprintf(stdin, "\n");

            exit(0); // EXIT CHILD
        } else {
            linuxCmds(argsIn);
            fprintf(stdin, "\n");

            exit(0); // EXIT CHILD
        } // end if else

    } else {
        // parent reads input from child
        pid = fork();

        // fork second child
        if(pid==0){
        // second child process

            // close write, change read, close read
            dup2(fd[READ], 0);
            close(fd[WRITE]);
            close(fd[READ]);

            // if its a builtin function tell user can't take input
            if (isBuiltinFunction(argsOut) == 1){
                printf("MockShell: Cannot Pipe to BuiltinFunctions\n");
                exit(0); // EXIT SECOND CHILD
            } else {
                eval(argsOut); // recursively eval right side of args after pipe
                exit(0); // EXIT SECOND CHILD
            } // end if else
        } else {
            // original parent
            int status;
            close(fd[WRITE]);           // close write
            close(fd[READ]);            // close read
            waitpid(pid, &status, 0);   // wait
        } // end if else 2nd fork
    } // end if else fork

    return 1;
} // end pipefunction

int eval(char ** args){
/* TAKES IN ARGS AND PARSES ARGS BASED ON PIPES AND INPUT
 * AND/OR OUTPUT REDIRECTION */

    // if user input is NULL return
    if(args[0] == NULL){
        printf("MockShell: No Input\n");
        return 0;
    } // end if

    int pipeNum = -1;
    int outputNum = -1;
    int inputNum = -1;
    int outputAppendNum = -1;

    char ** splitOne = (malloc (sizeof(char *) * 64)); // holds left side of split ARG
    char ** splitTwo = (malloc (sizeof(char *) * 64)); // holds right side of split ARG

    // finds if there is any type of redirect
    inputNum = isInputRedirect(args);
    outputNum = isOutputToFile(args);
    outputAppendNum = isOutputToFileAppend(args);
    pipeNum = isPipe(args);


    if(pipeNum != -1){
    // if a pipe exists

        // parse by pipe char
        splitOne = splitCommands(args, 0, pipeNum);
        splitTwo = splitCommands(args, (pipeNum + 1), lengthOfArray(args));

        // run pipe function
        pipeFunctions(splitOne, splitTwo);

    } else if((inputNum != -1) && (outputNum != -1)) {
        // if there is a input and output redirect

        if (inputNum > outputNum) {
            printf("MockShell: Input and Output Redirect Error\n");
            return 0;
        } else if ((strcmp(args[inputNum + 1], ">")) == 0) {
            printf("Mockshell: Input and output redirect error\n");
            return 0;
        } else if (args[outputNum + 1] == NULL) {
            printf("Mockshell: Input and output reditect error\n");
            return 0;
        } else {
            // split commands
            splitOne = splitCommands(args, 0, inputNum);

            // run input output redirection
            outputInputRedirection(splitOne, args[inputNum + 1], args[outputNum + 1], 0);
        } // end if else

    } else if((inputNum != -1) && (outputAppendNum != -1)){
        // if there is a input and output redirect with append

        if (inputNum > outputAppendNum) {
            printf("MockShell: Input and Output Redirect Error\n");
            return 0;
        } else if ((strcmp(args[inputNum + 1], ">>")) == 0) {
            printf("Mockshell: Input and output redirect error\n");
            return 0;
        } else if (args[outputAppendNum + 1] == NULL) {
            printf("Mockshell: Input and output reditect error\n");
            return 0;
        } else {
            // split commands
            splitOne = splitCommands(args, 0, inputNum);

            // run input output redirection
            outputInputRedirection(splitOne, args[inputNum + 1], args[outputAppendNum + 1], 1);
        } // end else if

    } else if(inputNum != -1){
        // if there is an input redirection

        // split commands
        splitOne = splitCommands(args, 0, inputNum);

        if(args[inputNum+1] == NULL){
            printf("MockShell: Input file argument missing.\n");
            return 0;

        } else {
            // run input redirection
            inputRedirect(splitOne, args[inputNum + 1]);
        } // end if else

    } else if(outputNum != -1) {
        // if there is an output redirection

        // split commands
        splitOne = splitCommands(args, 0, outputNum);

        if (args[outputNum + 1] == NULL) {
            printf("MockShell: Output file argument missing.");
            return 0;

        } else {

            // run output redirection
            outputRedirect(splitOne, args[outputNum + 1], 0);

        } // end if else

    } else if(outputAppendNum != -1) {
        // if there is output redirection with append

        // split commands
        splitOne = splitCommands(args, 0, outputAppendNum);

        if (args[outputAppendNum + 1] == NULL) {
            printf("MockShell: Output file argument missing.");
            return 0;
        } else {

            // run output redirection with append flag
            outputRedirect(splitOne, args[outputAppendNum + 1], 1);
        } // end if else

    } else if(isBuiltinFunction(args)){
        // else if it just a built in function run the built in

        runBuiltinFunctions(args);

    } else {
        // else run the linux cmd function

        linuxCmds(args);
    } // end if else

    return 1;
} // end eval

int exitProgram(char ** args){
/* EXITS THE PROGRAM IF USER ENTERED 'EXIT' */
    if(*args == NULL){
        return 0;
    } else {
        if ((strcmp(args[0], "EXIT") == 0)) {
            if (args[1] == NULL){

                exit(0);
            } // end if
        } // end if
    } // end if else
    return 0;
} // end exitprogram