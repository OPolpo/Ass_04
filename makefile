CC = g++
CFLAGS = -g -Wall
SRC = ass_panda_pt1.cxx
PROG = run
LIBS = -L/usr/lib/panda3d -lglut -lGLU -lSDL -lpanda -lpandafx -lpandaexpress -lpandabullet -lp3dtoolconfig -lp3dtool -lp3pystub -lp3direct -lp3framework -lBulletCollision -lBulletDynamics -lLinearMath -lpthread
INCL = -I/usr/include/python2.7 -I/usr/include/panda3d -I/usr/local/include/bullet 

$(PROG): $(SRC)
	$(CC) $(SRC) -o $(PROG) $(CFLAGS) $(INCL) $(LIBS)

clean: 
	rm -f $(PROG)
