#include <ncurses.h>
#include <stdlib.h>
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
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
#include "../headers/User_interface.h"
#include "../headers/constant.h"
#include "../headers/log_functions.h"
#include "../headers/shared_memory.h"

int shm_descriptor;
UserInterface *shared_memory;
void processCtrlCSignal(int receivedSignal);

//Draw border to show the working area of the drone
void drawBorder(){
      // Draw horizontal borders
        attron(COLOR_PAIR(1)); // Use blue color for the border
        for (int i = 20 ; i < BORDER_WIDTH ; ++i) {
            mvaddch(5, i, '-');                           // Top border
            mvaddch(BORDER_HEIGHT - 1, i, '-');           // Bottom border
        }
        attroff(COLOR_PAIR(1));

        // Draw vertical borders
        attron(COLOR_PAIR(1)); // Use blue color for the border
        for (int i = 6 ; i < BORDER_HEIGHT; ++i) {
            mvaddch(i, 20, '\'');                          // Left border
            mvaddch(i, BORDER_WIDTH  - 1 , '\'');          // Right border
        }
        attroff(COLOR_PAIR(1));

}


// Function to initialize colors in NCurses
void initialize_colors() {
    if (has_colors()) {

        start_color();
        

        // Initialize a color pair using the RGB values
     
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
       
    }
}

// Function to draw a character on the screen based on the Character structure
void draw_character(DisplayObject *character) {
    wattron(stdscr, COLOR_PAIR(character->displayColor));
    mvaddch(character->boardRow, character->boardCol, character->displaySymbol);
    wattroff(stdscr, COLOR_PAIR(character->displayColor));
}

// Function to update targets and obstacles based on shared memory
void readUserKeyPressed(UserInterface *shared_memory) {
    // Move drone based on user input
    char ch = getch();
    shared_memory->lastKeyPressed = ch;
}

int main() {

  /*Creating a 'Log File' to record the process information like 'PID' and 'Last Activity Time'*/
  LogFile_creation("./window.txt","w+");
  /*Logging the data*/
  logProcessInfo("window.txt","w+");
  //a 'SIGINT' signal from the 'watchdog process' to the 'motorx process'.
  signal(SIGINT, processCtrlCSignal);

  // Utility variable to avoid trigger resize event on launch
    int first_resize = TRUE;

   initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Hide cursor
    timeout(200); // Set a timeout for getch() to simulate real-time behavior

    initialize_colors();

    shm_descriptor = shm_open("/User_interface", O_RDWR, 0666);
    shared_memory = (UserInterface *)mmap(NULL, sizeof(UserInterface), PROT_READ | PROT_WRITE, MAP_SHARED, shm_descriptor, 0);

    while (1) {
        clear(); // Clear the screen

        //Draw border
    drawBorder();

  // Draw drone
  DisplayObject drone_character = {
    .boardRow = shared_memory->dronePosition.xCoordinate,
    .boardCol = shared_memory->dronePosition.yCoordinate,
    .displaySymbol = '+',
    .displayColor = 1,
    .isActive = 1
};
draw_character(&drone_character);

        
        
        // Refresh the screen
        refresh();

        // Move drone based on user input
        readUserKeyPressed(shared_memory);
    }

    munmap(shared_memory, sizeof(UserInterface));
    close(shm_descriptor);
    endwin();
    return 0;
}


void performCleanupAndEnd()
{
    printf("Performing cleanup and ending.\n");

    /* End the program with success status */
    exit(EXIT_SUCCESS);
}

/*
    Handler for processing SIGINT (Ctrl+C) signals
*/
void processCtrlCSignal(int receivedSignal)
{
    if (receivedSignal == SIGINT)
    { /*Close the resource*/
        munmap(shared_memory, sizeof(UserInterface));
        close(shm_descriptor);
        printf("Received the Signal! ");
        performCleanupAndEnd();
    }
}


