# ARP: Assignment 1
# PS Team. ARP-Assignment1
Mohammad Ali Pouryaghoub Shahkhali - S6063201

Amirmohammad Saberi  - S5840276

Group name: PS
## How to run
### Instructions to execute the program
In the shell, initiate the program by running the run.sh script found in the main directory:
    ```bash
    ./run.sh
    ```

## How does it work
## Overview
This C program provides a comprehensive simulation of a drone's dynamics in a 2D environment, focusing on movement and boundary interactions. It is designed to model physical attributes of a drone and respond to user inputs, making it an excellent tool for understanding basic drone physics and control mechanisms.

## Key Features

### Drone,keyboardManager Properties
- **Physical Attributes**: The program defines essential properties of the drone, such as mass and air friction coefficient.
- **Forces**: Specifies the forces acting in the x and y directions to control the drone's movement.

### Motion Dynamics
- **Acceleration Calculation**: The function `integratgeDroneDynamic` calculates the drone's acceleration based on current forces and air friction.
- **Position and Velocity Updates**: This function also updates the drone's position and velocity according to the calculated accelerations.

### Boundary Interaction
- **Collision Handling**: The drone's velocity is adjusted when it reaches predefined spatial boundaries, simulating a collision by reversing its direction.

### User Control
- **User Input Interpretation**: The `controlDroneMotion` function maps user inputs to corresponding directional forces, enabling control over the drone's movement within the simulation.

### Main Loop
- **Communication Channel**: Establishes a shared memory channel for real-time interaction and data exchange.
- **Continuous Position Update**: The main loop of the program continuously updates the drone's position based on user inputs.
- **Signal Handling**: Includes a SIGINT signal handler for graceful termination of the program.


# Window.C

## Overview
This C-based application simulates a drone's movement within a visualized area, using the NCurses library for graphics. It's particularly focused on user interaction, shared memory management, and graceful handling of system signals. The simulation offers an intuitive way to visualize and control a drone's movement in a confined space.

## Key Features

### Shared Memory Management
- **Shared Memory Segment**: Establishes a shared memory segment using `shm_open` for efficient inter-process communication. This segment is crucial for storing and retrieving the drone's position and status.
- **Resource Management**: Carefully handles system resources, ensuring they are properly allocated and freed when the program ends.

### Visualization with NCurses
- **Drone Area Visualization**: Utilizes `drawBorder` function to visually represent the boundaries of the drone's operating area.
- **Drone Representation**: Employs `draw_character` to dynamically display the drone's current position, updating it in real time as the user interacts with the simulation.

### User Interface and Interaction
- **NCurses Library**: Leverages NCurses for handling the graphical user interface, making the application more engaging and interactive.
- **Real-Time User Input**: Processes user input in real time, allowing for immediate response and control over the drone's movement within the simulation.

### Signal Handling for Graceful Termination
- **SIGINT Handling**: Implements a SIGINT signal handler (`processCtrlCSignal`) to catch interrupt signals (Ctrl+C). This ensures the application terminates gracefully, cleaning up resources and shared memory.
- **Cleanup Operations**: Contains a dedicated `performCleanupAndEnd` function to execute necessary cleanup tasks before exiting the application.



# Server.C

## Overview
This project is a C-based server application that highlights key aspects of network communication and shared memory management. It is designed to handle client-server interactions and inter-process communication efficiently.

## Features

### Socket Management
- Sets up and manages a network socket.
- Binds the socket to an IP address and listens for client requests.
- Accepts incoming connections and handles client requests.

### Shared Memory Handling
- Establishes and manages a named shared memory segment.
- Facilitates inter-process communication and data sharing.

### Signal Handling
- Implements signal handlers for SIGINT (Ctrl+C) signals.
- Ensures a graceful shutdown process, including the release of resources and closing of network connections.

### Logging
- Includes functions to create log files and record server activity, aiding in monitoring and debugging.

# Watch_dog.C

## Overview
This C application is designed for monitoring the activity of various processes. It continually checks their activity status and automatically handles processes that have been inactive for a specified duration.

## Key Features

### Process Monitoring
- Monitors specific processes by reading their activity data from log files.
- Utilizes a file pointer to access and read process information.

### Inactivity Detection and Handling
- Employs the `checkTime` function to detect process inactivity over a 60-second interval.
- Gracefully handles inactive processes using SIGINT signals.

### Signal Handling
- Implements a SIGINT signal handler (`handleSIGINT`) for controlled process termination.

# Master.C

## Overview
This C application is designed for advanced process management in a Linux environment. It demonstrates the creation, execution, and monitoring of multiple child processes, along with robust error and signal handling.

## Key Features

### Process Creation and Execution
- Utilizes `fork` and `execvp` for process creation and execution.
- Manages multiple child processes, executing different programs based on command-line arguments.

### Error and Signal Handling
- Comprehensive error handling during process creation and execution.
- Implements a SIGINT handler for controlled and graceful termination of the application.

### Robust Process Management
- The `main` function creates several child processes, managing their execution and waiting for their completion.
- Integrates logging functionalities for monitoring process activities.

### Automatic Process Termination
- Terminates processes automatically if they remain inactive beyond the specified time limit.

