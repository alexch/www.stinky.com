# For gcc
CC= gcc
# For ANSI compilers
#CC= cc

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g
INCLUDES = -I. -I./Game -I./Game/Unix
LIBS = -L. -lgame -lcurses -ltermcap

RM= /bin/rm -f

GAME = Board.o Game.o Player.o Display.o Piece.o MemoryClass.o \
	RandomClass.o Globals.o TetrisParams.o \
	SimpleGame.o

# OBJS = $(GAME:%=obj/%)
OBJS = $(GAME) Assert.o

.c.o: 
	$(CC) -c $(CFLAGS) $<

.cc.o:
	g++ -c $< -o $@ $(INCLUDES) $(CPPFLAGS) 

$(GAME):
	g++ -c ./Game/$*.cc -o $@ $(INCLUDES) $(CPPFLAGS) -DUNIX -DGPQ

Assert.o:
	g++ -c ./Game/Unix/$*.c -o $@ $(INCLUDES) $(CPPFLAGS) -DUNIX -DGPQ

#-- simple crap --

tetris: tetris.o util.o stubs.o libgame.a
	g++ tetris.o util.o stubs.o -lm $(LIBS) -g -o ./tetris.temp
	mv tetris.temp tetris
	

libgame.a: $(OBJS)
	ar ruv libgame.a $(OBJS)
	ranlib libgame.a

all: tetris query post-query imagemap jj phf

tar: query post-query imagemap jj phf 
	$(RM) post-query.o util.o imagerect.o jj.o phf.o

post-query: post-query.o util.o 
	$(CC) post-query.o util.o -o ../cgi-bin/post-query

query: query.o util.o
	$(CC) query.o util.o -o ../cgi-bin/query

imagemap: imagemap.o util.o
	$(CC) imagemap.o util.o -o ../cgi-bin/imagemap

jj: jj.o util.o
	$(CC) jj.o util.o -o ../cgi-bin/jj

phf: phf.o util.o
	$(CC) phf.o util.o -o ../cgi-bin/phf

change-passwd: change-passwd.o util.o
	$(CC) change-passwd.o util.o -o ../cgi-bin/change-passwd


clean:
	rm -f *.o ../cgi-bin/post-query ../cgi-bin/query ../cgi-bin/change-passwd ../cgi-bin/phf ../cgi-bin/jj ../cgi-bin/imagemap


game: game.o util.o
	$(CC) game.o util.o -o ../cgi-bin/game





