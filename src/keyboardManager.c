// KeyboardManager.c
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
// Function prototypes
void handleSIGINT(int signal);
void cleanupAndTerminate();


// Global variables
int communicationChannel_fd;
UserInterface *userInterfaceBuffer;


// Drone properties
double droneMass = 1.0;              // Mass of the drone (in kilograms)
double airFrictionCoefficient = 0.1; // Coefficient of air friction
double timeInterval = 0.1;           // Simulation time step (in seconds)
double forceX = 0;                   // Force in the x direction
double forceY = 0;                   // Force in the y direction

// Initial state
double initialXPosition = 5.0;   // Initial position in the x direction (in meters)
double initialYPosition = 20.0;  // Initial position in the y direction (in meters)
double initialXVelocity = 0.0;   // Initial velocity in the x direction (in meters per second)
double initialYVelocity = 0.0; // Initial velocity in the y direction (in meters per second)

void controlDroneMotion(CartesianPoint *currentDronePosition, char movementCommand);

 // This function could be responsible for calculating or updating the physical dynamics, 
 //such as position, velocity, and acceleration, based on control inputs or sensor data
 void integratgeDroneDynamic()
{
    // Calculate accelerations
    
    double xAcceleration = (forceX - airFrictionCoefficient * initialXVelocity) / droneMass;
    double yAcceleration = (forceY - airFrictionCoefficient * initialYVelocity) / droneMass;

    // Update velocities using accelerations and time interval
    initialXVelocity += xAcceleration * timeInterval;
    initialYVelocity += yAcceleration * timeInterval;

    // Update positions using velocities, accelerations, and time interval
    initialXPosition += initialXVelocity * timeInterval + 0.5 * xAcceleration * pow(timeInterval, 2);
    initialYPosition += initialYVelocity * timeInterval + 0.5 * yAcceleration * pow(timeInterval, 2);

    // Check and handle boundary conditions for X
    if ((initialXPosition < DRONE_X_LOWER_BOUND) || (initialXPosition > DRONE_X_UPPER_BOUND))
    {
        initialXVelocity = -(initialXVelocity ); // Reverse and reduce X velocity
        initialXPosition = (initialXPosition < DRONE_X_LOWER_BOUND) ? DRONE_X_LOWER_BOUND : DRONE_X_UPPER_BOUND; // Correct the position
    }

    // Check and handle boundary conditions for Y
    if ((initialYPosition < DRONE_Y_LOWER_BOUND) || (initialYPosition > DRONE_Y_UPPER_BOUND))
    {
        initialYVelocity = -(initialYVelocity ); // Reverse and reduce Y velocity
        initialYPosition = (initialYPosition < DRONE_Y_LOWER_BOUND) ? DRONE_Y_LOWER_BOUND : DRONE_Y_UPPER_BOUND; // Correct the position
    }
}
int main() {
    
        /*Establishing a communication channel through shared memory to facilitate real-time interaction and data exchange between processes.*/
   LogFile_creation("./keyboardManager.txt", "w+");
    // Log initial process information
    logProcessInfo("keyboardManager.txt", "w+");

    // Handle SIGINT signal with cleanupAndTerminate function
    signal(SIGINT, handleSIGINT);
    // Establish a communication channel through shared memory
    communicationChannel_fd = shm_open("/User_interface", O_RDWR, 0666);
    userInterfaceBuffer = (UserInterface *)mmap(NULL, sizeof(UserInterface), PROT_READ | PROT_WRITE, MAP_SHARED, communicationChannel_fd, 0);

    // Handle SIGINT signal
    signal(SIGINT, handleSIGINT);

    // Main loop to process keyboard inputs
    for (;;) {
        char key = userInterfaceBuffer->lastKeyPressed;
        controlDroneMotion(&(userInterfaceBuffer->dronePosition), key);
        usleep(100000); // Adjust for desired update rate
    }

    // Cleanup and close shared resources
    cleanupAndTerminate();
    return 0;
}

// Function to control the movement of the drone based on user input

void controlDroneMotion(CartesianPoint *currentDronePosition, char movementCommand)
{
    // Adjust forces based on user input
    switch (movementCommand)
    {
    case 'e':
        // Move left
        forceX--;
        break;
    case 'c':
        // Move right
        forceX++;
        break;
    case 's':
        // Move down
        forceY--;
        break;
    case 'f':
        // Move up
        forceY++;
        break;
    case 'r':
        // Move diagonally up-right
        forceX += -sqrt(2) / 2;
        forceY += sqrt(2) / 2;
        break;
    case 'x':
        // Move diagonally down-right
        forceX += sqrt(2) / 2;
        forceY += -sqrt(2) / 2;
        break;
    case 'v':
        // Move diagonally up-left
        forceX += sqrt(2) / 2;
        forceY += sqrt(2) / 2;
        break;
    case 'w':
        // Move diagonally down-left
        forceX += -sqrt(2) / 2;
        forceY += -sqrt(2) / 2;
        break;
    case 'd':
        // Stop all motion
        forceX = 0;
        forceY = 0;
        initialXVelocity = 0;
        initialYVelocity = 0;
        // Log the drone's process information
        logProcessInfo("aerial_drone.txt", "w+");
        break;
    }

    // Solve the dynamic equation by the Taylor series expansion
    integratgeDroneDynamic();

    // Update location
    currentDronePosition->xCoordinate = initialXPosition;
    currentDronePosition->yCoordinate = initialYPosition;
}
void cleanupAndTerminate() {
    printf("Cleaning up and terminating.\n");
    munmap(userInterfaceBuffer, sizeof(UserInterface));
    close(communicationChannel_fd);
    exit(EXIT_SUCCESS);
}

void handleSIGINT(int signal) {
    if (signal == SIGINT) {
        cleanupAndTerminate();
    }
}
