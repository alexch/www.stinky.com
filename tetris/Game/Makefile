TYPE = int

FLAGS = -DUNIX -DGPQ 
INCLUDEDIRS = -I. -I./Unix -I..
CFLAGS =  $(INCLUDEDIRS) $(FLAGS) -g
CPPFLAGS = $(INCLUDEDIRS) $(FLAGS) -lm -g

OBJS = Board.o Game.o Player.o Display.o Piece.o MemoryClass.o \
	RandomClass.o Assert.o Globals.o TetrisParams.o \
	SimpleGame.o

all: libgame.a

libgame.a: $(OBJS)
	ar ruv libgame.a $(OBJS)
	ranlib libgame.a

libsimplegame.a: $(SIMPLEOBJS)
	ar ruv libsimplegame.a $(SIMPLEOBJS)
	ranlib libsimplegame.a

clean:
	- rm $(OBJS) libgame.a *~ Mac/*~ Unix/*~ 2>/dev/null

.SUFFIXES: .cc $(SUFFIXES)

.cc.o:
	g++ -c $< -o $@ $(CPPFLAGS)

.c.o:
	gcc -c $< -o $@ $(CFLAGS)

Board.o: Board.h
Game.o: Game.h
Player.o:  Player.h
Display.o: Display.h
Piece.o:  Piece.h
MemoryClass.o: MemoryClass.h
RandomClass.o: RandomClass.h

Assert.o: ./Unix/Assert.c ./Unix/Assert.h
	gcc -c ./Unix/Assert.c -o $@ $(CFLAGS)

TetrisParams.o: TetrisParams.cc TetrisParams.h
