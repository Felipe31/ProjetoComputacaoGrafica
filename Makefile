ex2: ex2.c 
	gcc -o ex2.out ex2.c -lGLEW -framework OPENGL -framework GLUT /usr/local/lib/libGLEW.a -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
	./ex2.out &

ex: exemplo.c 
	gcc -o ex.out exemplo.c -framework OPENGL -framework GLUT
	./ex.out &

clear:
	rm *.out