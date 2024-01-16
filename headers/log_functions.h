/*a function to create a log file for the target process*/
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

void logProcessInfo(char *logFileName, char *behavior) {
    FILE *logFile;
    int processID = getpid();

    time_t currentTime = time(NULL);
    struct tm timeInfo = *localtime(&currentTime);

    /* Opening the log file and recording the current activity time of the process */
    logFile = fopen(logFileName, behavior);

    /* Error Checking */
    if (logFile == NULL) {
        printf("Could not open the %s file; errno=%d\n", logFileName, errno);
        exit(1);
    }

    /* Writing into the file */
    fprintf(logFile, "%d,%d,%d,%d\n", processID, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);

    /* Closing the file */
    fclose(logFile);
}

void LogFile_creation(char *logFileName, char *behavior) {
    FILE *newLogFile;

    /* In a new run, if the log file exists, remove the old log file */
    if (!remove(logFileName)) {
        printf("The old log file is deleted successfully!\n");
    }

    /* Create a new log file */
    if ((newLogFile = fopen(logFileName, behavior))) {
        printf("The new log file is created successfully!\n");
        logProcessInfo(logFileName, behavior);
        fclose(newLogFile);
    } else {
        printf("Could not create a log file!\n");
    }
}