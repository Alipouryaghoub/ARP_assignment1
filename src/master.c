#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <signal.h>
#include <time.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"

// process creations
int executeProgram(const char * program_app , char *functionArgs[]) {

    pid_t childPID = fork();

    if (childPID < 0) {
        perror("errror in process creations");
        return 1;
    } else if (childPID != 0) {
        // Parent process
        return childPID;
    } else {
        // Child process
        if (execvp(program_app, functionArgs) == 0);
        perror("error in creation ");
        return 1;
    }
}

/*Function to perform cleanup and terminate the process
*/
void cleanupAndTerminate()
{
    printf("Cleaning up and terminating.\n");


    /* Terminate the process */
    exit(EXIT_SUCCESS);
}

/*
Signal handler for handling SIGINT (Ctrl+C)
*/
void handleSIGINT(int signal)
{
    if (signal == SIGINT)
    {
        printf("Received SIGINT Signal! ");
        cleanupAndTerminate();
    }
}

// Function to wait for multiple PIDs
void waitForMultiplePIDs(pid_t *pids, int numPids , int status) {

    for (int i = 0; i < numPids; i++) {
        waitpid(pids[i], &status, 0);
        
    }
}
int main() {
  
  /*Creating a 'Log File' to record the process information like 'PID' and 'Last Activity Time'*/
  LogFile_creation("./master.txt","w+");
  /*Logging the data*/
  logProcessInfo("master.txt","w+");
  //a 'SIGINT' signal from the 'watchdog process' to the 'motorx process'.
  signal(SIGINT, handleSIGINT);

  /*Defining the command line's arguments for different process*/
    char * parameter_display_area [] = { "/usr/bin/konsole", "-e", "./bin/window", NULL };
  /*Creating different processes*/
  pid_t pid_s = executeProgram("./bin/server", NULL);
  pid_t pid_keyboard_manager = executeProgram("./bin/keyboardManager", NULL);
  pid_t pid_d = executeProgram("./bin/drone", NULL);
  pid_t pid_wd = executeProgram("./bin/watch_dog", NULL);
  pid_t pid_w = executeProgram("/usr/bin/konsole", parameter_display_area);
    
 /*Waiting for all the child processes*/
//   int status;
//   waitpid(pid_s, &status, 0);
//    waitpid(pid_keyboard_manager, &status, 0);
//   waitpid(pid_d, &status, 0);
//   waitpid(pid_wd, &status, 0);
//   waitpid(pid_w, &status, 0);

pid_t myPids[] = {pid_s, pid_keyboard_manager, pid_d, pid_wd, pid_w};
int numPids = sizeof(myPids);
int status;

waitForMultiplePIDs(myPids, numPids , status);

  printf ("Main program exiting with status %d\n", status);
  return 0;
}




