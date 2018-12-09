all: gtkEx4

gtkEx4: gtkEx4.c builder.ui
	gcc `pkg-config --cflags gtk+-3.0` -o gtkEx4 gtkEx4.c `pkg-config --libs gtk+-3.0`

clean:
	rm gtkEx4
