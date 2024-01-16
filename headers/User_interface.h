// User_interface.h
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

// Represents a 2D point with coordinates x and y
typedef struct {
    double xCoordinate;  // X-coordinate
    double yCoordinate;  // Y-coordinate
} CartesianPoint;

// Represents the state of the User_interface including drone, targets, obstacles, and the last pressed key
typedef struct {
    CartesianPoint dronePosition;        // Position of the drone on the User_interface
    char lastKeyPressed;                 // Last key pressed
} UserInterface;

// Represents a character on the User_interface with row, column, symbol, color pair, and activity status
typedef struct {
    double boardRow;    // Row position on the User_interface
    double boardCol;    // Column position on the User_interface
    char displaySymbol; // Symbol representing the character
    short displayColor; // Color pair associated with the character
    int isActive;       // Activity status of the character
} DisplayObject;

#endif // USER_INTERFACE_H
