anim: animacaoRodovia.c 
	gcc -o anim.out animacaoRodovia.c -framework OPENGL -framework GLUT
	./anim.out &

anim2: animacaoRodoviaV2.c 
	gcc -o anim2.out animacaoRodoviaV2.c -framework OPENGL -framework GLUT
	./anim2.out &

3d: 3d.c 
	gcc -o 3d.out 3d.c -framework OPENGL -framework GLUT
	./3d.out &

ex: exemplo.c 
	gcc -o ex.out exemplo.c -lGLEW -framework OPENGL -framework GLUT /usr/local/lib/libGLEW.a -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
	./ex.out &

clear:
	rm *.out