


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[128];
    char val[128];
} entry;

void getword(char *word, char *line, char stop);
char x2c(char *what);
void unescape_url(char *url);
void plustospace(char *str);

#define kEntries 10

main(int argc, char *argv[]) {
    entry entries[kEntries];
    register int x,m=0;
    char *cl;
    char szGame[256], szMove[256];

    printf("Content-type: text/html%c%c",10,10);

    if(strcmp(getenv("REQUEST_METHOD"),"GET")) {
        printf("This script should be referenced with a METHOD of GET.\n");
        printf("If you don't understand this, see this ");
        printf("<A HREF=\"http://www.ncsa.uiuc.edu/SDG/Software/Mosaic/Docs/fill-out-forms/overview.html\">forms overview</A>.%c",10);
        exit(1);
    }

    printf("<TITLE>Game</TITLE>\n");
    printf("<BODY><H1>This Is A Game</H1><p>");

    cl = getenv("QUERY_STRING");

    if(cl == NULL || *cl == '\0') {

/*
   Nothing? It's a new game.
*/
      strcpy(szGame, "123");

      printf("New Game!<p>");

      /* do more game initialization stuff */
    }
    else
    {
      for(x=0;cl[0] != '\0' && x<kEntries ;x++) {
	m=x;
	getword(entries[x].val,cl,'&');
	plustospace(entries[x].val);
	unescape_url(entries[x].val);
	getword(entries[x].name,entries[x].val,'=');
	
	if (!strcasecmp(entries[x].name, "game"))
	  strcpy(szGame, entries[x].val);
	
	if (!strcasecmp(entries[x].name, "move"))
	  strcpy(szMove, entries[x].val);
	
      }
      
      /* load in game */
      
      /* do move */
      if (*szMove == 'l')
	strcat(szGame, "L");
      else if (*szMove == 'r')
	strcat(szGame, "R");
      else
	strcat(szGame, "X");
      
    }

    /*
       format page
    */

    printf("<H2>Games are fun</H2>");
    printf("<p>Here's a board:<p>----<p>");

    printf("<H2>Your Move:</H2>");
    printf("<A HREF=\"http://laurel.cs.columbia.edu:690808/cgi-bin/game?");
    printf("game=%s&move=left\"", szGame);
    printf("\">Left</a>, ");
    printf("<A HREF=\"http://laurel.cs.columbia.edu:690808/cgi-bin/game?");
    printf("game=%s&move=right\"", szGame);
    printf("\">Right</a><p>");

    printf("<hr>");
    printf("<H2>Query Results</H2>");
    printf("You submitted the following name/value pairs:<p>%c",10);
    printf("<ul>%c",10);

    for(x=0; x <= m; x++)
        printf("<li> <code>%s = %s</code>%c",entries[x].name,
               entries[x].val,10);
    printf("</ul>%c",10);

    printf("<HR><a href=\"http://laurel.cs.columbia.edu:690808\">");
    printf("Super-Secret Spy Server</a><p>");
}


