all: gtkExEditor

gtkExEditor: gtkExEditor.c
	gcc `pkg-config --cflags gtk+-3.0` -o gtkExEditor gtkExEditor.c `pkg-config --libs gtk+-3.0`

clean:
	rm gtkExEditor
