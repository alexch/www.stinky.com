#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <time.h>

#include "SimpleGame.h"
#include "Board.h"
#include "Piece.h"

#include "Globals.h"

#define TETRIS_FILES_DIR  "/tmp"
#define ACCESS_FILE "/home/alex/tetris/tetris.games"
#define LOG_FILE "/home/alex/tetris/tetris.log"

typedef struct {
    char name[128];
    char val[128];
} entry;

extern "C" {
void getword(char *word, char *line, char stop);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);
};

void new_game_name(char *);
void print_buttons(char *, char *);

#define kEntries 10
int graphics = 1;

main(int argc, char *argv[]) {
    entry entries[kEntries];
    register int x,m=0;
    char *cl;
    char szGame[256], szMove[256];
    char * szScriptName;
    SimpleGame * game;
    int val;
    int fGameOver = 0;


    ios::sync_with_stdio();
    cerr = cout;

    InitGlobals();
    InitPieces();

    gParams[param_center_piece] = 1;

    printf("Content-type: text/html%c%c",10,10);  
    printf("<TITLE>Tetris</TITLE>\n");
    printf("<BODY><H1><IMG SRC=\"tetris.gif\">Tetris");
    printf("<FONT> by <a href=\"http://www.stinky.com/alex\">Alex Chaffee</a></FONT>\n");
    cout << "</H1>" << endl;
    cout << "<FONT SIZE=-1>"
 "<I><B>Tetris</B>, invented by Alexey Pazhitnov, "
 "is a trademark of AcademySoft and ELORG.</I>\n";
    cout << "</FONT>\n";

   cout << "<P><STRONG>Disclaimer:</STRONG> This has got to be one of the"
" most useless things I've ever done." <<endl;
    cout << "<HR>";


    szScriptName = getenv("SCRIPT_NAME");

    cl = getenv("QUERY_STRING");

    Boolean fNew = TRUE;

    if(cl == NULL || *cl == '\0')
	fNew = TRUE;		      /* Nothing? It's a new game.      */

    else {

      for(x=0;cl[0] != '\0' && x<kEntries ;x++) {
	m=x;
	getword(entries[x].val,cl,'&');
	plustospace(entries[x].val);

	unescape_url(entries[x].val);
	getword(entries[x].name,entries[x].val,'=');
	
	if (!strcasecmp(entries[x].name, "game"))
        {
		strcpy(szGame, entries[x].val);
		fNew = FALSE;
	}
	
	if (!strcasecmp(entries[x].name, "move")) {
		strcpy(szMove, entries[x].val);
		fNew = FALSE;
	}

	if (!strcasecmp(entries[x].name, "graphics"))
	  graphics = atoi(entries[x].val);
	
      }


    }

    if (fNew) {

      new_game_name(szGame);
      game = new SimpleGame();
      game->Init(time(NULL));

      /* increment access count */
	 

     /* NEED SEMAPHORING */

      ifstream access(ACCESS_FILE);
      int cAccess = 0;
      access >> cAccess;
      access.close();
      ofstream out_access(ACCESS_FILE);
//      out_access.seekp(0);
      out_access << ++cAccess << endl;
      out_access.close();

     
      time_t t;
      time(&t);
      char szTime[50];
      strcpy(szTime, ctime(&t));
      szTime[strlen(szTime)-1] = '\0';

      ofstream log(LOG_FILE, ios::app);
      log << cAccess << '\t'
	  <<  szTime << '\t' 
	  << getenv("REMOTE_HOST") << endl;
      log.close();


      cout << "This is the " << cAccess;
      if (cAccess < 20) cAccess = 10;
      switch (cAccess%10) {
      case 1:
	cout << "st";
	break;
      case 2:
	cout << "nd";
	break;
      case 3:
	cout << "rd";
	break;
      default:
	cout << "th";
	break;
      }
      cout << " new game of Tetris.<p>";

    }
    else
    {
      
      /* load in game */
      char szFile[256];
      sprintf(szFile, TETRIS_FILES_DIR"/%s", szGame);
//    cout << "<tt>In File: " << szFile << "</tt><p>";
      ifstream in_file(szFile);

      game = new SimpleGame;
      game->Read(in_file);

      in_file.close();

      /* do move */
#define CASE(x)    if (!strcasecmp(szMove, x))

      CASE("left") {
	val = game->Step(moveLeft);
      }
      CASE("right") {
	val = game->Step(moveRight);
      }
      CASE("rotate") {
	val = game->Step(moveRotate);
      }
      CASE("halt") {
	val = game->Step(moveHalt);
      }
      CASE("drop") {
	val = game->Step(moveHalt);
      }
      CASE("down") {
	val = game->Step(moveDown);
      }

      cout << "Game Time: " << game->time << "<P>";

    }
    switch (val) {
    case STEP_ILLEGAL:
      cout << "Illegal move!<BR>";
      break;
    case STEP_LINE:
      cout << "You just cleared a line!<BR>";
      break;
    case STEP_FULL:
      fGameOver = TRUE;
      break;
    default:
      break;
    }

    char szFile[256];
    sprintf(szFile, TETRIS_FILES_DIR"/%s", szGame);

    cout << "<A href=\"" << szScriptName << "?move=redraw&game=" << szGame <<
      "&graphics=";
    cout << "0\">Text</A> or ";
    cout << "<A href=\"" << szScriptName << "?move=redraw&game=" << szGame <<
      "&graphics=";
    cout << "1\">Graphics</A><BR>" << endl;



    if (fGameOver)
    {    
      cout <<"<H2>Game Over.</h2>"
	<< "Final Score: " << game->score << "<br>"
	<< "Time: " << game->time << "<br>"
	<< "Efficiency: ";
      cout.precision(2);
      cout << (float)game->score/(float)game->time << "<p>";
      
      cout << "<A HREF=\"http:" << szScriptName << "?graphics=" << graphics
	<< "\">New Game</A>";
    }
    else
    {
      print_buttons(szGame, szScriptName);
    }
    if (graphics==0)
      {
	cout << "<pre>\n";
	game->Print(cout);
	cout << "</pre>";
      }
    else
      {
	cout << "<center>";
	cout << "<br>";
	game->Print(cout, "<IMG SRC=\"black8x8.gif\">",
		    "<IMG SRC=\"red8x8.gif\">",
		    "<IMG SRC=\"blue8x8.gif\">",
                    "<IMG SRC=\"white8x8.gif\">",
		    "<BR>\n");
	cout << "<br>";
	cout << "</center>\n";
      }
 
    if (fGameOver) {
      /* delete file */
      unlink(szFile);
    }
    else
      {
//	cout << "<tt>Out File: " << szFile << "<tt><p>";
	ofstream out_file(szFile);
	out_file.seekp(0);

	game->Write(out_file);
	out_file << endl;
//	out_file << eof;  // hope this works
        out_file.close();

	/* Show buttons */
	print_buttons(szGame, szScriptName);
	cout << "<HR>";
      }
 	  
}


#include <sys/types.h>

void new_game_name(char *sz)
{
  sprintf(sz, "game%d", getpid());
}

void print_button(char * szGame, char * szScriptName, char *szMove, int graphics)
{
  printf("<A HREF=\"http:%s?", szScriptName);
  cout 
    << "game=" << szGame
    << "&move=" << szMove
    << "&graphics=" << graphics
    << "&pid=" << getpid()                // force a reload
    << "\">" << szMove << "</A>";
}

void print_buttons(char * szGame, char * szScriptName)
{
	printf("<B>Your Move:</B> \n");
	
	print_button(szGame, szScriptName, "Left", graphics);
	cout << ", ";
	print_button(szGame, szScriptName, "Rotate", graphics);
	cout << ", ";
	print_button(szGame, szScriptName, "Right", graphics);
	cout << ", ";
	print_button(szGame, szScriptName, "Down", graphics);
	cout << " one square, ";
	print_button(szGame, szScriptName, "Drop", graphics);
	cout << " piece to bottom, or ";

   cout << "<A HREF=\"http:" << szScriptName << "?graphics=" << graphics << "\">New Game</A>";
}

