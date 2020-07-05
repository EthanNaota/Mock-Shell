#ifndef MOCKSHELL_BUILTINS_H
#define MOCKSHELL_BUILTINS_H

/* GRABBING INPUT AND PARSING TO AN ARRAY OF ARGS */
char * grabInput();
char ** parseInput(char * userinput);
void printStrArr(char **userInput);

/* FINDING IF A CHARACTER EXISTS IN A STRING */
int charExists(char ** args, char *var); // returns -1 or array position
int isPipe(char ** args);                // returns -1 or array position
int isOutputToFile(char ** args);        // returns -1 or array position
int isOutputToFileAppend(char ** args);  // returns -1 or array position
int isInputRedirect(char ** args);       // returns -1 or array position

/* BUILT IN FUNCTIONS*/
int cd_cmd(char ** args);         // changes file location
int clrScreen(char ** args);      // clears screen
int printDirectory(char ** args); // prints out files in directory
int printEnviron(char ** args);   // print out environment variables
int echo(char ** args);           // prints out string typed after echo
int printHelp(char ** args);      // prints out help

/* PRINT USER AND PATH FOR MAIN SHELL */
void shellEnvr(char ** args);

/* BOOLEAN */
short int isBackground(char ** args);       // returns 1 if background process
short int isBuiltinFunction(char ** args);  // returns 1 if builtin function

/* RUNS BUILTIN FUNCTIONS USING FUNCTION POINTERS */
int runBuiltinFunctions(char ** args);

/* RUNS LINUX CMDS */
int linuxCmds(char ** args);

/* SPLITS COMMANDS INTO AN ARRAY STARTING AT A AND ENDING WITH B */
char ** splitCommands(char ** args, int a, int b); // returns a split array

/* FINDS THE LENGTH OF AN ARRAY */
int lengthOfArray(char ** args);    // returns length

/* CMDS FOR OUTPUT, INPUT, AND OUTPUTINPUT REDIRECTION */
int outputRedirect(char ** args, char * fileLoc, int isAppend); // redirects output to file
int inputRedirect(char ** args, char * fileLoc);                // redirects input from file
int outputInputRedirection(char ** args, char * fileIn, char * fileOut, int isAppend);  // redirection for input and output files

/* PIPE + EVALUATION OF ARGS */
int pipeFunctions(char ** argsIn, char ** argsOut); // changes in and out + calls evaluate recursively
int eval(char ** args);                             // evaluates the ARGS array + calls pipe Function Recursively

/* IF USER INPUT IS 'EXIT' EXITS PROGRAM */
int exitProgram(char ** args);


#endif //MOCKSHELL_BUILTINS_H
