#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>
#include "../headers/User_interface.h"
#include <math.h>
#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"
#define THRESHOLD 0.0001

void handleSIGINT(int signal);

int main()
{
    /*Establishing a communication channel through shared memory to facilitate real-time interaction and data exchange between processes.*/
   LogFile_creation("./drone.txt", "w+");
    // Log initial process information
    logProcessInfo("drone.txt", "w+");

    // Handle SIGINT signal with cleanupAndTerminate function
    signal(SIGINT, handleSIGINT);
 


    // start a shared memory instance
    communicationChannel_fd = shm_open("/User_interface", O_RDWR, 0666);
    userInterfaceBuffer = (UserInterface *)mmap(NULL, sizeof(UserInterface), PROT_READ | PROT_WRITE, MAP_SHARED, communicationChannel_fd, 0);


    munmap(userInterfaceBuffer, sizeof(UserInterface));
    close(communicationChannel_fd);
    return 0;
}


/*Function to perform cleanup and terminate the process*/
void cleanupAndTerminate()
{
    printf("Cleaning up and terminating.\n");

    /* Clean up shared resources */
    munmap(userInterfaceBuffer, sizeof(UserInterface));
    close(communicationChannel_fd);

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
