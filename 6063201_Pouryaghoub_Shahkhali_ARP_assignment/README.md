# ARP-dron-Assignment1
Initial framework for the first assignment in Advanced and Robot Programming (ARP).
This foundation offers essential features for the Command and Inspection operations, both executed using the ncurses library for straightforward graphical interfaces. The organization of the repository includes:

The src directory, housing the source code for the Command, Inspection, and Master processes.
The include directory, containing all necessary data structures and methods for the ncurses system, facilitating the creation of the two GUIs. This directory can be overlooked if you're not aiming to enhance the graphical elements of the interfaces, as it already equips you with all needed functionalities.
The bin directory, designated for storing the compiled executable files.
# ncurses installation
To install the ncurses library, simply open a terminal and type the following commands:
```console
sudo apt-get install libncurses-dev
```
or
```console
sudo apt-get install libncurses5-dev libncursesw5-dev
```

# Compiling and running
The processes for Command and Inspection rely on the ncurses library, which must be linked at the time of compilation. Additionally, the Inspection process incorporates the mathematical library for extra computations. Hence, the compilation steps are as follows:
1. for the **Inspection process**:
	```console
	gcc src/inspection_console.c -o bin/inspection -lncurses -lm -lrt
	```
2. for the **Master process**:
	```console
	gcc src/master.c -o bin/master -lncurses -lm -lrt
	```
3. for the **watchDog process**:
	```console
	gcc src/watchDog.c -o bin/watchdog -lncurses -lm -lrt
	```
	
After compiling, **assuming you have Konsole installed in your system** as per the professor's indications, you can **simply run the Master executable**, which will be responsible of spawning the two GUIs:
```console
./bin/master
```

#Problem-solving and diagnostic procedures

If you encounter any unusual activity upon starting the application, such as buttons not appearing in the GUI or misalignment of graphical elements, a simple resizing of the terminal window should rectify the issue.
