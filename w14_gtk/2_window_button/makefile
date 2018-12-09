all: gtkEx2 gtkEx2Enhanced

gtkEx2: gtkEx2.c
	gcc `pkg-config --cflags gtk+-3.0` -o gtkEx2 gtkEx2.c `pkg-config --libs gtk+-3.0`

gtkEx2Enhanced: gtkEx2Enhanced.c
	gcc `pkg-config --cflags gtk+-3.0` -o gtkEx2Enhanced gtkEx2Enhanced.c `pkg-config --libs gtk+-3.0`

clean:
	rm gtkEx2 gtkEx2Enhanced
