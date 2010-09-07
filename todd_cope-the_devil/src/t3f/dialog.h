#ifndef T3F_DIALOG_H
#define T3F_DIALOG_H

#include <allegro5/allegro5.h>

void t3f_file_select(bool (*proc)(char *), char * path, char * title, char * patterns, int mode);

#endif
