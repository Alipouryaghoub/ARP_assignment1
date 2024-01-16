
clean all for: master,server,drone,keyboardManager,window,watchdog

clean:
	rm bin/*

master: 

    gcc src/master.c -lm -lrt  -o  bin/master
server: 
	gcc src/server.c -lm -lrt  -o  bin/server

drone: 
	gcc src/drone.c -lm -lrt  -o  bin/drone

keyboard: 
	gcc src/keyboard_manager.c -lm -lrt  -o  bin/keyboardManager

window: 
	gcc src/window.c -lncurses -lm -lrt  -o  bin/window

watchdog: 
	gcc src/watch_dog.c -lm -lrt  -o  bin/watch_dog

run:
	./bin/master

