#include <ncurses.h>
#include <string.h>
#include <unistd.h> 
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef struct {
	chtype 	ls, rs, ts, bs, 
	 	tl, tr, bl, br;
}DRONE_ROBOT_BORDER;

typedef struct {
	int startx, starty;
	int height, width;
	DRONE_ROBOT_BORDER border;
}DRONE_ROBOT;

typedef struct {
    char message[100];
} BLACKBOARD_DATA;

typedef struct {
    float ee_x;
    float ee_y;
    int vx;
    int vy;
} SHARED_DATA;

int MAX_MESSAGE_LENGTH = 100;
int DRONE_ROBOT_X_LIM = 101;
int DRONE_ROBOT_Y_LIM = 41;

// Variable for the drone's structural configuration
DRONE_ROBOT drone;

// Variable for mouse event tracking
MEVENT event;
BLACKBOARD_DATA *blackboard;

// Set up the hoist structure and its parameters
void make_hoist() {

	drone.height = DRONE_ROBOT_Y_LIM;
	drone.width = DRONE_ROBOT_X_LIM;
	drone.starty = (LINES - drone.height)/2 + 4;	
	drone.startx = (COLS - drone.width)/2;

  drone.border.ls = ACS_VLINE;
  drone.border.rs = ACS_VLINE;
  drone.border.ts = ACS_HLINE;
  drone.border.bs = ACS_HLINE;
  drone.border.tl = ACS_ULCORNER;
  drone.border.tr = ACS_URCORNER;
  drone.border.bl = ACS_LLCORNER;
  drone.border.br = ACS_LRCORNER;
}

void draw_drone() {  
  
    int x, y, w, h;

  x = drone.startx;
  y = drone.starty;
  w = drone.width;
  h = drone.height;

    // Render the top corners and the horizontal framework
    mvaddch(y - 1, x - 2, drone.border.tl);
    mvaddch(y - 1, x + w + 1, drone.border.tr);
    mvhline(y - 1, x - 1, drone.border.ts, w + 2);

    // Render the bottom corners and the side frameworks
    mvaddch(y + h , x - 2, drone.border.bl);
    mvaddch(y + h , x + w + 1, drone.border.br);
    mvvline(y, x - 2, drone.border.ls, h);
    mvvline(y, x + w + 1 , drone.border.rs, h);

    // Render the reference ground surface
    mvhline(y + h , x - 1, drone.border.bs, w + 2);

    refresh();
}

// Display a message showing the actual coordinates of the end-effector above the hoist illustration
void draw_end_effector_msg(float x, float y, float vx, float vy) {

    for(int j = 0; j < COLS; j++) {
        mvaddch(drone.starty - 2, j, ' ');
    }

    char msg[100];
    sprintf(msg, "Drone velocity [%05.2f vx,%.2f vy]                                      Drone position [%05.2f, %.2f]", vx, vy, x, y);

    attron(A_BOLD);
    mvprintw(drone.starty - 2, (COLS - strlen(msg)) / 2 + 1, msg);
    attroff(A_BOLD);
}

// Render the end-effector of the dron inside its structure
void draw_drone_end_effector_at(float ee_x, float ee_y) {

    // First, empty all drawn content
     for(int j = 0; j < drone.width; j++) {
        for(int i = 0; i < drone.height; i++) {
            mvaddch(drone.starty + i, drone.startx + j, ' ');
        }
    }

    // // Translate real coordinates to the nearest lower integer...
    // int ee_x_int = floor(ee_x);
    // int ee_y_int = floor(ee_y);

    attron(A_BOLD | COLOR_PAIR(1));
    mvaddch(drone.starty + ee_y, drone.startx + ee_x, '+');
    attroff(A_BOLD | COLOR_PAIR(1));
}

// Helper function to verify if the end-effector is within permissible boundaries
void check_ee_within_limits(float* ee_x, float* ee_y) {

    // Verifies alignment along the horizontal axis
    if(*ee_x <= 0) {
        *ee_x = 0 ;
    }
    else if(*ee_x >= DRONE_ROBOT_X_LIM) {
        *ee_x = DRONE_ROBOT_X_LIM-1;
    }
   
    // Verifies alignment along the vertical axis
    if(*ee_y <= 0) {
        *ee_y = 0 ;
    }
    else if(*ee_y >= DRONE_ROBOT_Y_LIM) {
        *ee_y = DRONE_ROBOT_Y_LIM-1;
    }
}

void init_console_ui() {

    // Start the ncurses mode
    initscr();		
	start_color();

    // Turn off line buffering...
	cbreak();

    // Turn off character echoing and cursor blinking
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK,   COLOR_YELLOW);

    // Render User Interface components
    draw_end_effector_msg(0, 0, 0, 0);

    // Enable listening for input (keyboard and mouse events ...)
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);

    refresh();
}

void update_console_ui(float *ee_x, float *ee_y, int *vx, int *vy, BLACKBOARD_DATA *blackboard) {

    // Verify if the upcoming position of the end-effector is within the defined boundaries
    check_ee_within_limits(ee_x, ee_y);

    // Render the new position of the end-effector
    draw_hoist_end_effector_at(*ee_x,*ee_y);

    // Modify the text message to reflect the current position of the end-effector
    draw_end_effector_msg(*ee_x, *ee_y, *vx, *vy);
    
    // Show the information from the blackboard in the ncurses interface window
    mvprintw(2, 0, "Message on the Blackboard: %s", blackboard->message);

    refresh();

}

void reset_console_ui() {

    // Erase the display screen
    erase();

    // Redraw the User Interface components
    make_hoist();

    // Render User Interface components
    draw_hoist();
    draw_end_effector_msg(0, 0, 0 ,0);

    refresh();
}
