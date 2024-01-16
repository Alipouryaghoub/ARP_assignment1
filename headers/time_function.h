/* Function to verify the current activity status of a target process within a specified time limit, considering its last activity time */
#include <time.h>
#include<stdio.h>
bool validateCurrentSystemTime (int hStart, int mStart, int sStart, int durationSeconds){
     time_t t = time(NULL);  
     struct tm tm = *localtime(&t);
     /* Check if the process's last activity time is within the specified time limit */  
     bool staus = (tm.tm_hour - hStart)*3600 + (tm.tm_min - mStart)*60 + (tm.tm_sec - sStart) > durationSeconds;
     if(staus){
       printf("Time limit exceeded\n");
       return true;
     }
     return false;      
}