#include "./../include/inspection_utilities.h"

SHARED_DATA *shared_data;

int main(int argc, char const *argv[])
{
    // Variable used to prevent triggering the resize event upon application launch
    int first_resize = TRUE;

    // Coordinates of the robotic end-effector
    float ee_x, ee_y;

    // Setting up the velocity parameters
    int vx = 0;
    int vy = 0;

    // Set up Shared Memory for the Blackboard System
    int shm_fd = shm_open("/my_shared_memory", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd, sizeof(SHARED_DATA));
    shared_data = mmap(NULL, sizeof(SHARED_DATA), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Set up User Interface 
    init_console_ui();

    // Continuous loop
    while(TRUE)
  {  
        // Receive mouse/resize commands in non-blocking mode...
        int cmd = getch();

        // If the user resizes the screen, refresh the UI
        if(cmd == KEY_RESIZE) {
            if(first_resize) {
                first_resize = FALSE;
            }
            else {
                reset_console_ui();
            }
        }
        
        // Keyboard commands ...
        switch (cmd)
        {
            // Each key press applies a force of 1N for 1 second, resulting in an acceleration of 1 m/s².
            // We can deduce that pressing any key increases the velocity by 1 m/s in the respective direction, as per the equation Δv = a.
            case '7':
                vx--;
                vy--;
                break;
            case '9':
                vx++;
                vy--;
                break;
            case '1':
                vx--;
                vy++;
                break;
            case '3':
                vx++;
                vy++;
                break;
            case '4':
                vx--;
                break;
            case '6':
                vx++;
                break;
            case '8':
                vy--;
                break;
            case '2':
                vy++;
                break;
            default:
                break;
        }
        // Given that v = Δx, we can infer that in each second, we have: ee_x' = ee_x + vx and similarly for vy.
        ee_x += vx;
        ee_y += vy;
        sleep(1);
        
        // Refresh the UI
        update_console_ui(&ee_x, &ee_y, &vx, &vy, blackboard);
  }

    // Perform cleanup
    endwin();
    munmap(blackboard, sizeof(BLACKBOARD_DATA));
    shm_unlink("/my_shared_memory");

    return 0;
}
