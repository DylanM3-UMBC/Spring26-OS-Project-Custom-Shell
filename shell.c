#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include "utils.h"
#include <sys/types.h>
#include <sys/wait.h>


int stringCheck(char *str1, char *str2) {
    if (strlen(str1) == strlen(str2)) {
        for(int i = 0; i < strlen(str1); i++){
            if(str1[i] != str2[i]) {
                return -1;
            }
        }
        return 0;
    }
   
    return -1;
}

int getNumOfArguments(char* userInput, size_t size) { 
    if (userInput != NULL) {
        // This detects arguments by spaces
        if(strchr(userInput, ' ') != NULL){

            // Copy string to not modify source string
            char copyInput[size];
            strcpy(copyInput, userInput);
            
            // This will be use to parse the arguments for counting
            char* arg = strtok(copyInput, " ");
            int argCounter = 0;
            
            // Counts arguments
            while(arg != NULL){
                arg = strtok(NULL, " ");
                argCounter++;
            }
            
            return argCounter;
        }
    }


    return 1;
}


int systemCommandsProcess(char **argArray, int numArg) {
    pid_t pid = fork();
    
    if (pid == 0){    
        // Executed commands
        execvp(argArray[0], argArray);
        
        // Executed if execvp not successful in executing command
        printf("ERROR: command not found! \n");

        return -1;
    }
    else {
        wait(NULL);
        return 0;
    }
}

void readHistory(FILE* myFile) {
    fclose(myFile);
    myFile = NULL;
    
    FILE *readMyFile = fopen(".421history", "r");
    
    if (readMyFile != NULL) {
        char getCharacter =  fgetc(readMyFile);
        
        while (getCharacter != EOF) {
            // Prints char
            printf("%c", getCharacter);

            // Gets character from file
            getCharacter =  fgetc(readMyFile);
        } 
        

        fclose(readMyFile);
        readMyFile = NULL;
    }
}

int procreadfn(char **argArray) {
    char *version = "version";
    char *partitions = "partitions";
    char *drivers = "tty/drivers";
    char *options = "fs/ext4/sda1/options";
    char *catPath = "/usr/bin/cat";
    char *procreadArg = argArray[1]; 

    // Used to check valid commands before executing procread
    if(stringCheck(procreadArg, version) == 0 && argArray[2] == NULL) {
        char* versionPath = "/proc/version";
        strcpy(argArray[1],versionPath);
    }
    else if(stringCheck(procreadArg, partitions) == 0 && argArray[2] == NULL) {
        char *partitionsPath = "/proc/partitions";
        strcpy(argArray[1], partitionsPath);
    }
    else if(stringCheck(procreadArg, drivers) == 0 && argArray[2] == NULL) {
        char *driversPath = "/proc/tty/drivers";
        strcpy(argArray[1], driversPath);
    }
    else if (stringCheck(procreadArg, options) == 0 && argArray[2] == NULL) {
        char *optionsPath = "/proc/fs/ext4/sda1/options";
        strcpy(argArray[1], optionsPath);
    }
    else {
        printf("ERROR: command failed, does the file exist? \n");
        return 0;
    }
   
   
   // This will only execute when there is a valid argument
    pid_t pid = fork();

    if (pid == 0) {
        strcpy(argArray[0], catPath);
        
        execv(argArray[0], argArray);
        
        return -1;
    }
    else {
        wait(NULL);
        return 0;
    }
}

int shell(char **argArray, int numArg, FILE *myFile) {
    /* Command list */
    char *procread = "procread";
    char *history = "history";

    // The command the user typed
    char *userCommand = argArray[0];
    char* userArgument = argArray[1];

    if (stringCheck(userCommand, history) == 0) {
        readHistory(myFile);
        return 1;
    }
    else if (stringCheck(userCommand, procread) == 0) {
       
        if (argArray[1] == NULL) {
          printf ("Command failed, procread requires one argument \n");
        }
        else if (argArray[2] != NULL) {
            printf ("Command failed, only a single argument is allowed in using procread \n");
        }
        else if (argArray[1] != NULL) {
            char charArg = userArgument[0];
            
            if (charArg == '/') {
                printf ("Command failed, file cannot start with a slash using procread \n");
            }
            else  {
                return procreadfn(argArray);
            }
            
        }
        else {
            return procreadfn(argArray);
        }
    }
    else {
        return systemCommandsProcess(argArray, numArg);
    }

    return 0;
}

int main() {
    // Variables to take in input
    FILE *myFile = fopen(".421history", "w");
    FILE *stream = fdopen(0, "r");
    char* userInput = NULL;
    char *exitShell = "exit";
    int shellResult;
    size_t size = 0;
    ssize_t getLineErrorCheck;

    do {
        // This is used to show a prompt
        printf("SimpleShell $ ");

        // Get input then compare it by using strcmp to see if it exit
        getLineErrorCheck = getline(&userInput, &size, stream);

        // This if condition ensures if getline run into errors 
        // the program will exit properly without crashing
        if(getLineErrorCheck == -1){
            free(userInput);
            fclose(myFile);
            fclose(stream);
            exit(EXIT_FAILURE);
        }

        char inputHistory[size];
        strcpy(inputHistory, userInput);

        // Getline adds a extra 1 on char len, this is to ensure
        // that the string len go back to what it should be without extra 1
        if (strcmp(userInput, "\0") != 0) {
            userInput[strcspn(userInput,"\n")] = '\0';
        }

        int charLen = strlen(userInput);        

        // This validates if the user type other commands other than exit
        if (stringCheck(userInput, exitShell) != 0 && charLen != 0) {
            
            int numArg = (getNumOfArguments(userInput, size) + 1);
            char **argArray = NULL;
            argArray = malloc(sizeof(char *) * numArg);     

            
            // This will handle arguments depending if its one or multiple
            if (numArg == 2) {
                argArray[0] = userInput;
                argArray[1] = NULL;
             
                shellResult = shell(argArray, numArg, myFile);
            }
            else if (numArg > 2) {
                // Allocating for more than one argument
                for (int i = 0; i < numArg - 1; i++) {
                    argArray[i] = malloc(sizeof(char) * charLen);
                }

                // This parse arguments as well as commands into the array
                char* tempHold = tempHold = strtok(userInput, " ");
                strcpy(argArray[0], tempHold);

                for (int i = 1; i < numArg - 1; i++) {
                    tempHold = strtok(NULL, " ");
                    strcpy(argArray[i], tempHold);
                }

                argArray[numArg - 1] = NULL;
                
                // Call the shell for commands
                shellResult = shell(argArray, numArg, myFile);

                // Free the 2d array memory
                for(int i = 0; i < numArg - 1; i++) {
                    free(argArray[i]);
                }
            }
            
            free(argArray);

            // This is for when a child process fails so there be no errors
            if (shellResult == -1) {
                free(userInput);
                fclose(stream);
                fclose(myFile);
                exit(EXIT_FAILURE);
            }
            else if (stringCheck(inputHistory, exitShell) != 0 && charLen != 0){
                // Records commands
                if (myFile != NULL && shellResult != 1) {
                    fprintf(myFile, inputHistory);
                }
                else if (shellResult == 1) {
                    // This means that the user type the command history
                    // And needs to append rather than overight file
                    myFile = fopen(".421history", "a");
                }
            }
        }
        
    } while(stringCheck(userInput, exitShell) != 0);
    
    // Free pointers to avoid memory leaks
    free(userInput);
    fclose(stream);
    fclose(myFile);
    
    return 0;
}