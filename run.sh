#!/bin/bash
gcc src/master.c -lm -lrt  -o  bin/master
gcc src/watch_dog.c -lm -lrt  -o  bin/watch_dog
gcc src/drone.c -lm -lrt  -o  bin/drone
gcc src/keyboardManager.c -lm -lrt  -o  bin/keyboardManager
gcc src/server.c -lm -lrt  -o  bin/server
gcc src/window.c -lncurses -lm -lrt  -o  bin/window

./bin/master

