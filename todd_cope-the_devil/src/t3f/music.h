#ifndef T3F_MUSIC_H
#define T3F_MUSIC_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>

extern ALLEGRO_AUDIO_STREAM * t3f_stream;
bool t3f_play_music(char * fn, double loop_start, double loop_end);
void t3f_stop_music(void);
void t3f_pause_music(void);
void t3f_resume_music(void);
void t3f_set_music_volume(float volume);
float t3f_get_music_volume(void);

#endif
