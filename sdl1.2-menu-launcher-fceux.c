

/* 

 ========
 SDL 1.2 Menu Launcher for FCEUX 
 ========


 ========
 Under Linux:
 1)apt-get install libsdl-ttf2.0-dev   libsdl-image1.2-dev  libsdl-ttf2.0-dev  libghc6-sdl-ttf-dev 
 2)arial.ttf shall be into the directory: 
    http://www.font-police.com/classique/sans-serif/arial.ttf 

 ========
 - To compile: 
    gcc -lncurses -lSDL te.c  -lSDL_ttf
    then:
    ./a.out

 ======== Vers.:
 - For 1.2 SDL: ttf is no longer being used to be more compatible.
 
*/




#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
//#include <unistd.h>
#include <string.h>

#define PAMAX 250

#include "SDL/SDL.h" 
#include "SDL/SDL_image.h" 
#include "SDL/SDL_ttf.h" 



// for fexist
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>


// #include "SDL/SDL_opengl.h" 

// #include <SDL.h> 
// #include <SDL_image.h> 
// #include <SDL_ttf.h> 

// #include <SDL.h>
// #undef main


// passed with 
// c:\appl\MinGW\bin\mingw32-gcc.exe -lsdl  -o c:\temp\ndata\test.exe   c:\temp\ndata\hello.c -lpdcurses -lSDL -lSDL_ttf -lSDL_image -mwindows 
// 
//////////////////// CORE

static int compare_fun( const void *p, const void *q){
  const char *l = p ; 
  const char *r = q ; 
  int cmp; 
  cmp = strcmp( l, r );
  return cmp; 
}





int fexist(char *a_option){
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}









// tested and ok
char * string_replace2( char  *original, char  *pattern, char  *replacement) {
  size_t const replen = strlen(replacement);
  size_t const patlen = strlen(pattern);
  size_t const orilen = strlen(original);

  size_t patcnt = 0;
  const char * oriptr;
  const char * patloc;

  // find how many times the pattern occurs in the original string
  for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
  {
    patcnt++;
  }

  {
    // allocate memory for the new string
    size_t const retlen = orilen + patcnt * (replen - patlen);
    char * const returned = (char *) malloc( sizeof(char) * (retlen + 1) );

    if (returned != NULL)
    {
      // copy the original string, 
      // replacing all the instances of the pattern
      char * retptr = returned;
      for (oriptr = original; patloc = strstr(oriptr, pattern); oriptr = patloc + patlen)
      {
        size_t const skplen = patloc - oriptr;
        // copy the section until the occurence of the pattern
        strncpy(retptr, oriptr, skplen);
        retptr += skplen;
        // copy the replacement 
        strncpy(retptr, replacement, replen);
        retptr += replen;
      }
      // copy the rest of the string.
      strcpy(retptr, oriptr);
    }
    return returned;
  }
}





SDL_Surface* screen, *logo;
SDL_Rect logorect;
SDL_Surface* fontSurface;
SDL_Color fColor;
SDL_Rect fontRect;
SDL_Event Event;


//TTF_Font* font;

//Initialize the font, set to white
void fontInit(){
//         TTF_Init();
//         font = TTF_OpenFont("arial.ttf", 12);
//         fColor.r = 255;
//         fColor.g = 255;
//         fColor.b = 255;
}



int sprite_x = 4; 
int sprite_y = 4; 

void sdlrectangle(int y1, int x1, int y2, int x2, int color) {
	SDL_Rect rect = {x1 ,y1 ,  (x2-x1),  (y2-y1) };
	SDL_FillRect(screen, &rect, color);
}

void sdlrectanglered(int y1, int x1, int y2, int x2, int color) {
	SDL_Rect rect = {x1 ,y1 ,  x2-x1,  y2-y1 };
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 240, 10, 0));
        //SDL_Flip(screen);
}

void sdlprint(int y, int x, char *c){
       // fontSurface = TTF_RenderText_Solid(font, c, fColor);
       // fontRect.x = x;
        //fontRect.y = y;
       // SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
        //SDL_BlitSurface(fontSurface, NULL, screen, &fontRect);
}



SDL_Color sdlcolor[10]; 





/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
//int main( int argc, char* args[] ) 
int main( int argc, char *argv[])
{ 

   chdir( "/rootdisk/games/nes" );

    char textit[1000];
    char line[250];
    char cmdi[250];
    char filetarget[250];
    strncpy( textit, "hello world" , 1000 );

    char idata[1240][250];
    int gamenumber = -1;
    int gamenumbermax = 0;
    unsigned filemax = 0;
    unsigned n=0;
    DIR *dirp;
    struct dirent *dp;
    unsigned notfoundfile = 0;


    void informe(){
       printf( "=======\n"  );
       printf( " List items : %d/%d \n" , gamenumber, gamenumbermax );
       printf( " Current item : %d\n" , gamenumber );
       printf( " Current value : %s\n" , idata[ gamenumber ] );
       printf( "=======\n"  );
    }

    printf( " ----------------- \n" );
    printf( " : LOADING FILES : ... \n" );
    printf( " ----------------- \n" );

    n = 0 ; 
    filemax = 0; 
    dirp = opendir( "." );
    while  ((dp = readdir( dirp )) != NULL  &&  
             n < sizeof idata / sizeof idata[ 0 ]) {

        strncpy( line , dp->d_name , 250 );

        if ( strcmp( dp->d_name, "." ) != 0 ) 
        if ( strcmp( dp->d_name, ".." ) != 0 ) 
        if ( line[ strlen( dp->d_name )-1 ] == 'p' ) 
        if ( ( fexist( string_replace2( dp->d_name , ".bmp" , ".nes" )) == 1 )
        || ( strstr( dp->d_name , "firefox.bmp" ) != 0 )
        || ( strstr( dp->d_name , "youtube.bmp" ) != 0 )
        )
        {
            printf( " %d => %s (found *.nes) \n" , n , dp->d_name );
            strncpy( idata[ n++ ] , dp->d_name , 250 );
            filemax++;
            gamenumber++;
            gamenumbermax++;
        } else
        {  
           notfoundfile++;
        }
    }
    closedir( dirp );
    printf( " Final : %d => %s (out of %d) \n" , filemax , idata[ filemax-1 ], notfoundfile );

    if ( n > 1 )
      qsort( idata, n , sizeof idata[0], compare_fun );

    for( n = 0 ; n <= filemax ; n++)
       printf( " List sorted : %d => %s \n" , n , idata[ n ] );

    gamenumbermax--;
    gamenumber = 0; 
    informe();



    if ( argc == 2)
      if  ( strcmp( argv[1] , "--list" ) ==  0 )
      {
          printf( " List shown! Bye! (--list) \n" );
          return 0;
      }







    //Initialize SDL 
    //SDL_Init( SDL_INIT_EVERYTHING ); 
    logorect.x = 100 ;
    logorect.y = 120 ;

    // Initialize the SDL library with the Video subsystem
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

    //Create the screen
    screen = SDL_SetVideoMode(1024, 768, 0, SDL_SWSURFACE | SDL_FULLSCREEN);
    //screen = SDL_SetVideoMode(320, 240, 0, SDL_SWSURFACE );

    //Initialize fonts
    fontInit();

    //Print to center of screen
    //printF("Hello World", screen->w/2 - 11*3, screen->h/2);
    // printF("Hello World", screen->w/2 - 11*3, screen->h/2);


    sdlprint( 1 , 1,  textit );
    SDL_Flip(screen);
    SDL_Rect rect = {0 ,0 ,  50, 50 };

    int sdlcolor = 0;
    SDL_Rect pos ;
    int i;

   /*
    sdlcolor[0] = SDL_MapRGB(screen->format,255,255,255);
    sdlcolor[1] = SDL_MapRGB(screen->format,255,0,0); 
   */

   void launcher_firefox(){
          system(  " iceweasel www.google.com "  ); 
   }
   void launcher_youtube(){
          system(  " iceweasel www.youtube.com "  ); 
   }
   void launcher_fceux(){
                          strncpy(  cmdi , " sleep 1s ; fceux " , PAMAX );
                          strncat( cmdi , " \"" , PAMAX - strlen( cmdi ) -1 );
                          strncat( cmdi , string_replace2( idata[ gamenumber ] , ".bmp" , ".nes" ) , PAMAX - strlen( cmdi ) -1 );
                          strncat( cmdi , "\" ; sleep 1s " , PAMAX - strlen( cmdi ) -1 );


                          printf( "CMD: %s\n" , cmdi ); 
                          system(  cmdi ); 
   }

  //ZoomPicture (screen, picture, SMOOTHING_OFF);
  //ZoomPicture (screen, picture, SMOOTHING_ON);
  // #include "zoompic.c"

    void showgamepicturestretched()
    {
        SDL_Rect stretchRect;
        stretchRect.x=0; stretchRect.y=0;
        stretchRect.w=1024; 
        stretchRect.h=768;

        // gamenumber--;

        if ( gamenumber <= -1 ) gamenumber = gamenumbermax;
        logo = SDL_LoadBMP( idata[ gamenumber ] );
  
      //SDL_BlitSurface( logo , NULL , screen , &logorect );
  //    SDL_BlitSurface( logo , NULL , screen , &stretchRect );
  // SDL_SoftStretch( logo , NULL, screen , &stretchRect );
        SDL_Surface * optimizedsurface = NULL;
       optimizedsurface=SDL_ConvertSurface(logo , screen->format , 0 );

      SDL_BlitSurface( optimizedsurface , NULL , screen , &stretchRect );

        informe();
    }
    showgamepicturestretched();



    void showgamepicture()
    {
        // gamenumber--;
        if ( gamenumber <= -1 ) gamenumber = gamenumbermax;
        logo = SDL_LoadBMP( idata[ gamenumber ] );
        logorect.x=0; logorect.y=0;
        SDL_BlitSurface( logo , NULL , screen , &logorect );
        informe();
    }
    showgamepicture();


    do {

        sdlrectanglered( sprite_y, sprite_x, sprite_y+3, sprite_x+3, 0);
        SDL_Flip(screen);

    // Process the events

        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {

                case SDL_KEYDOWN:

                    switch (Event.key.keysym.sym) {

                        case SDLK_DELETE:
                          strncpy( textit, "" , 1000 );
                          break;


                        case SDLK_UP:
                          sprite_y--;
                          break;
                        case SDLK_DOWN:
                          sprite_y++;
                          break;


                        case SDLK_u:
                          sdlcolor++;
                          break;

                        case SDLK_d:
                          sdlcolor--;
                          break;


                        case SDLK_t:
  			  sdlrectangle( 0, 0, 30, 200, SDL_MapRGB(screen->format,155,155,155));  
	                  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,155,155,155));  
                          sdlprint( 300, 5 , textit);
                          break;



                        case SDLK_LEFT:
                           gamenumber--;
                           if ( gamenumber <= -1 ) gamenumber = gamenumbermax;
                             logo = SDL_LoadBMP( idata[ gamenumber ] );
                             SDL_BlitSurface( logo , NULL , screen , &logorect );
                          informe();
                          break;

                        case SDLK_RIGHT:
                           gamenumber++;
                           if ( gamenumber >= gamenumbermax ) gamenumber = 0; 
                              logo = SDL_LoadBMP( idata[ gamenumber ] );
                              SDL_BlitSurface( logo , NULL , screen , &logorect );
                          informe();
                          break;

                        case SDLK_RETURN:
                          //strncpy( textit, "You pressed Return" , 1000 );
                          //sdlprint( 5, 5 , textit);
                          informe();
                          screen = SDL_SetVideoMode(1024, 768, 0, SDL_SWSURFACE );
                          
                          if ( strcmp( idata[gamenumber], "001firefox.bmp"  ) == 0 )
                               launcher_firefox();
                          else if ( strcmp( idata[gamenumber], "002youtube.bmp"  ) == 0 )
                               launcher_youtube();
                          else 
                               launcher_fceux();

                          screen = SDL_SetVideoMode(1024, 768, 0, SDL_SWSURFACE | SDL_FULLSCREEN);
                          informe();
                          showgamepicture();
                          //Event.type = SDL_QUIT;
                          break;

                        case SDLK_a:
                           logo = SDL_LoadBMP("./a.bmp");
                           SDL_BlitSurface( logo , NULL , screen , &logorect );
                          break;

                        case SDLK_b:
                           logo = SDL_LoadBMP("./b.bmp");
                           SDL_BlitSurface( logo , NULL , screen , &logorect );
                          break;

                        case SDLK_c:
                           logo = SDL_LoadBMP("./c.bmp");
                           SDL_BlitSurface( logo , NULL , screen , &logorect );
                          break;

                        case SDLK_p:
                        case SDLK_l:
                           logo = SDL_LoadBMP("./mi.bmp");
                           SDL_BlitSurface( logo , NULL , screen , &logorect );
                          break;

                        case SDLK_o:
  			  sdlrectanglered( 0, 0, 60, 200, 0);
                          break;


                        case SDLK_r:
  			  sdlrectangle( 0, 0, 30, 200, SDL_MapRGB(screen->format,0,55,95));  
                          //sdlprint( 5, 5 , textit);
                          break;


                    // Escape forces us to quit the app
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            Event.type = SDL_QUIT;
                        break;

                        default:
                        break;
                    }
                break;

            default:
            break;
        }
    }
    SDL_Delay(10);
    } while (Event.type != SDL_QUIT);


    //Quit SDL 
    SDL_Quit(); 
    
    return 0; 
}





