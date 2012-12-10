/*
     Validate.cc

     This validates the performance of an individual

     It is a modification of ShowTetris.cc




	ShowTetris.cc
	
	This function is activated by defining SHOWTETRIS when you
	compile main.cc. It initializes sgpc with mostly dummy values,
	then runs some random games with the given tree.
		
*/
#include <stdio.h>
#ifdef UNIX
#include "Unix/config.h"
#include <values.h>
#include <curses.h>
#else
#include <limits.h>
#include <console.h>
#endif
#include "Game.h"
#include "SmartPlayer.h"
#include "Display.h"
#include "../prob.h"
#include "proto.h"
#include "gpc.h"
#include "../prob_params.h"
#include "random.h"

extern int gThinkTime[];  // tree evals per piece
extern int gNumPieces;                   // num pieces

extern int gLinesCleared;

extern float gEfficiency;


#ifdef MACINTOSH
WindowPtr SetUpWindows (void);
#endif
void Validate(FILE * f, int games, char * params, int arc, char *argv);


void Validate(FILE * f, int games, char * params, int argc, char **argv)
{
    Game   *game;
    SmartPlayer	*player;
    Display *display;
    int	score;
    pop_struct *pop;
    tree    *thetree;
    int	i;
    float ratio;

	float validation = 0;
	float efficiency = 0;
	float lines_cleared = 0;
	float pieces_dropped = 0;


    
/*          EVS: now uses normal args -- for multipop/ADF
    argc = 4;
    argv = (char**)malloc( sizeof(char*) * argc);
    argv[0] = "ShowTetris";
    argv[1] = "1";
    argv[2] = "1";
    argv[3] = params;
*/

    startup(argc,argv,&NUMPOPS,&NUMGENS,&DEMES,&DEMEROWS,&DEMECOLS,&START_GEN,
      &CKPT_FILE,&POP,&GRID); 

    pop = POP;

    #ifdef MACINTOSH
    display = new WindowDisplay(SetUpWindows());
    #else
    display = new TextDisplay;

//    initscr();    // bunch of things for "curses" screen refresher
//    cbreak();
//    noecho();

//    nonl();  // must stay commented -- it messes things up
//    intrflush(stdscr,FALSE);
//    keypad(stdscr,TRUE);
    #endif

    if (argc > (4 + NUMPOPS))
      f = fopen(argv[argc-1], "r");


    if (f == NULL || feof(f))
    {
    	printf("No tree; using random player.\n");
//    	player = new RandomPlayer;    // evs WRITEME
    }
    else
    {
	player = new SmartPlayer;
//	thetree = read_tree(POP, 0, f); old, before ADFs

        for(i=0;i<NUMPOPS;i++) {
	  pop[i].population[0] = read_tree(POP, i, f);
	}

	close(f);

/*
	for(i=0;i<NUMPOPS;i++) {
	  printf("\nPOP %d tree:\n",i);
	  write_tree(pop,pop[i].population[0],"%d ",stdout);
	}
*/

	 //   printf("\n**the tree:**\n\n");  
	 //   write_tree(POP, thetree, "%d ", stdout);

//    	((SmartPlayer*)player)->SetTree(thetree);  old, before ADFs
        ((SmartPlayer*)player)->SetBrain(pop,0);
	gNumPops = NUMPOPS;
    }
    

    games = gParams[param_num_validate];


    for	(i=0, score = 0; i < games; i++)
    {
	int seed;
	int points;

	game = new Game( gParams[param_board_x], gParams[param_board_y] );
	seed = random_int(MAXINT);
//	points = game->Run( player, display, seed, TRUE );
	points = game->Run( player, NULL, seed, TRUE );
	delete game;
	
	//printf("\nScore: %ld", points);
//	fflush(stdout);
	score += points;

		efficiency += gEfficiency;
		lines_cleared += gLinesCleared;
		// gPiecesDropped starts counting at 0
		pieces_dropped += (gNumPieces + 1);
    }
    
    #ifndef MACINTOSH
//    endwin();
    #endif


	efficiency /= (float)gParams[param_num_validate];
	printf("Average Efficiency: %f\n",efficiency);

	lines_cleared /= (float)gParams[param_num_validate];
	printf("Average lines cleared: %f\n",lines_cleared);

	pieces_dropped /= (float)gParams[param_num_validate];
	printf("Average pieces dropped: %f\n",pieces_dropped);


    printf("\nAverage score: %f (%d games)\n", score/(float)games, games );
    printf("Move ranges:  FIX THS FOR NO HALT ALLOWED param\n");
    ratio = (float)gParams[param_val_max] /
            ((float)(moveMax-(!gParams[param_inc_drop])));
    printf("ratio %f\n",ratio);
    for(i=0; i<moveMax-(!gParams[param_inc_drop]);i++) {
      printf("%d %d - %d\n",i,(int)(i*ratio), (int)((i+1)*ratio) - 1);
    }

    show_game_statistics();
    printf("Times, for debug:");
    for(i=0;i<=gNumPieces; i++)printf("%d ",gThinkTime[i]);
    printf("\n");
}

#ifdef MACINTOSH

WindowPtr SetUpWindows (void)
{
    Rect    drawingWindowPosition = {120, 10, 300, 348};
    WindowPtr	wind;
    
    cshow(stdout);  // just to be sure
    wind = NewWindow(NULL, &drawingWindowPosition,
				"\pTetris", TRUE, noGrowDocProc,
				(WindowPtr)-1L,	FALSE, 0L);
    
/*
	//  Change some	aspects	of the default
	//  console window: its	size and its title.
	//
    console_options.top	= 40;
    console_options.left = 10;
    console_options.nrows = 5;
    console_options.ncols = 80;
    console_options.pause_atexit = 1;
	
    printf("Tetris!\n");
*/
    return wind;
    
}

#endif




/* EOF */
