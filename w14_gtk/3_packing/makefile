all: gtkEx3

gtkEx3: gtkEx3.c
	gcc `pkg-config --cflags gtk+-3.0` -o gtkEx3 gtkEx3.c `pkg-config --libs gtk+-3.0`

clean:
	rm gtkEx3
