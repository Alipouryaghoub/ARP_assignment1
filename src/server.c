/*1-Standard C Library*/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

/*2-File Descriptor Management*/
#include <fcntl.h>

/*3-Shared Memory Management*/
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>

/*4-Socket and Networking Management*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"


int socketDescriptor, newSocketDescriptor, clientDescriptor;

struct sockaddr_in serverSocketAddress, clientSocketAddress;

int* allocateSharedMemory(const char *memoryIdentifier);

/* Pointer to an integer, named 'memoryPointer', used for dynamic memory allocation or referencing an integer variable in memory */
int *memoryPointer;

/* Declaration of a character pointer 'sharedMemoryName', initialized to point to the string literal "User_interface". This is typically used for naming a shared memory segment in inter-process communication. */
char *memoryObjectName = "User_interface";
/*Function to execute cleanup and gracefully conclude the program*/
void cleanupAndTerminateGracefully ()
{
    printf("Performing cleanup and concluding.\n");

    /* End the program with success status */
    exit(EXIT_SUCCESS);
}

/*Signal handler for processing SIGINT (Ctrl+C) signals*/
void handleCtrlCSignal(int signalReceived)
{
    if (signalReceived == SIGINT)
    {
        printf("Received SIGINT Signal! ");
     cleanupAndTerminateGracefully();
    }
}

int main(int argumentCount, char *argumentValues[])

{   
 /*Initializing a 'Log File' to record process information such as 'PID' and 'Last Activity Time'*/
  LogFile_creation("./server.txt","w+");
 /*Recording the data in the log file*/
  logProcessInfo("server.txt","w+");
  // Sending a 'SIGINT' signal from the 'watchdog process' to the 'motorx process'.
  signal(SIGINT, handleCtrlCSignal);
   /*Establishing a shared memory region&*/
    memoryPointer = allocateSharedMemory(memoryObjectName );
    /* Initializing a socket */
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    /* Managing Errors */
    if (socketDescriptor < 0) 
        perror("Failed to open the socket");
    /* Configure the server address */

    bzero((char *) & serverSocketAddress, sizeof(serverSocketAddress));
    serverSocketAddress.sin_family = AF_INET;
    serverSocketAddress.sin_addr.s_addr = INADDR_ANY;
    serverSocketAddress.sin_port = htons(2998);

    /* Associate the socket with the IP address */
    if (bind(socketDescriptor, (struct sockaddr *) &serverSocketAddress, sizeof(serverSocketAddress)) < 0) 
        perror("Failed to bind the socket.");
            
    //Server is ready! Waits for new client to request
    printf ("Server is ready! Waiting for a new client request...\n");
    listen(socketDescriptor,5);

    /* Handling the incoming request */
    clientDescriptor = sizeof(clientSocketAddress);
    newSocketDescriptor = accept(socketDescriptor, (struct sockaddr *) &clientSocketAddress, &clientDescriptor);
    printf ("Handling the incoming request\n");

   /* Managing Errors and Recovery */
    if (newSocketDescriptor < 0)
        perror("Error during accept operation");
}

/* Function to establish a named shared memory */
int* allocateSharedMemory(const char *memoryIdentifier){
/* Establishes a named shared memory object with 'O_CREAT | O_RDWR' flags and '0666' permissions */
  
  int sharedMemFD = shm_open(memoryIdentifier, O_CREAT | O_RDWR, 0666);

  /* Error Handling and Recovery */
  if(sharedMemFD == -1 ){  
    printf("Failed to create shared memory!");
    exit(1);
  }

  /* Adjusts the size of the shared memory object */
  if(ftruncate(sharedMemFD, capacityOfSharedMemory) == -1)
    printf("Trancate failed\n");

  /* Links the memory address of the calling process to the address of the shared memory */
  int *memoryPointer = (int *)mmap(NULL, capacityOfSharedMemory, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemFD, 0);

  /* Managing Errors */
  if (memoryPointer == MAP_FAILED) {
    printf("Mapping process to shared memory failed.\n");
  }

    /* Obtains the 'ptr' pointing to the shared memory address */
    return memoryPointer;
}

/* Signal Handler Function Definitions */
void handleSIGINT(int signal){
    if (signal == SIGINT){
        printf("Received SIGINT signal!\n");

/* Release the allocated resource */
/* Unmap the shared memory */

      if (munmap(memoryPointer, capacityOfSharedMemory) == -1) {
          perror("unmapMemory");
          exit(EXIT_FAILURE);
      }

      /* Close the descriptor for shared memory */
      if (close(*memoryPointer) == -1) {
          perror("closeResource");
          exit(EXIT_FAILURE);
      }

      /* Delete (unlink) the shared memory object */
      if (shm_unlink(memoryObjectName ) == -1) {
          perror("unlinkSharedMemory");
          exit(EXIT_FAILURE);
      }

         /* Close the network socket connection */

      if (close(socketDescriptor) == -1) {
          perror("Failed to close the socket.");
          exit(EXIT_FAILURE);
      }

        /* Terminate the process */
        pid_t pid = getpid();
        kill(pid, SIGKILL);

    }

}

