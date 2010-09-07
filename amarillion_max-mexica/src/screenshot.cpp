#include "screenshot.h"
#include <allegro.h>
#include <string.h>
#include <stdio.h>

/* Returns Zero on failure, non-zero on success. */
int take_screen_shot( char *filename )
{
   BITMAP *vis; // visible screen
   PALETTE pal; // current palette
   int ret = -1;     // return value

   get_palette( pal );
   vis = create_sub_bitmap( screen, 0, 0, SCREEN_W, SCREEN_H );
   if( vis != NULL )
   {
      ret = save_bitmap( filename, vis, pal );
      destroy_bitmap( vis );
   }

   return ret == 0;
}

void screenshot()
{

    char filename[14];
    int count = 0;

    // search for a unused filename of the format ssNNNNNN.pcx
    do
    {
       sprintf( filename, "ss%06d.pcx", count++ );
    } while ( strlen(filename) == 12 && exists(filename) );

    if( strlen(filename) == 12 )
    {
       take_screen_shot(filename);
    }
    else
    {
       /* Error: All filenames taken */
    }

}
