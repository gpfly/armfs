CC := arm-linux-gcc
all:
	$(CC) -o login.cgi login.c -lcgi -Lcgilib/ -Iinclude/
	make -C rain/
	make -C camera/
	make -C smog/
	make -C humiture/
	make -C fan/
	make -C irrigate/
	make -C light/
	make -C soil/
	make -C photo/
	make -C beep/
	make -C reel/
clean:
	make -C rain/     clean
	make -C camera/   clean
	make -C smog/     clean
	make -C humiture/ clean
	make -C fan/      clean
	make -C irrigate/ clean
	make -C light/ clean
	make -C soil/ clean
	make -C photo/ clean
	make -C beep/ clean
	make -C reel/ clean
	rm -f *.cgi
