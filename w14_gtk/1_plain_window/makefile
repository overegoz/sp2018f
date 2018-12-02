all: gtk1

gtk1: gtk1.c
	gcc gtk1.c -o gtk1 `pkg-config --cflags --libs gtk+-2.0`

clean:
	rm gtk1
