#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "t3f.h"
#include "music.h"

ALLEGRO_AUDIO_STREAM * t3f_stream = NULL;
static float t3f_music_volume = 1.0;

bool t3f_play_music(char * fn, double loop_start, double loop_end)
{
	if(t3f_stream)
	{
		t3f_stop_music();
	}
	al_stop_timer(t3f_timer);
	t3f_stream = al_load_audio_stream(fn, 4, 4096);
	if(!t3f_stream)
	{
		al_start_timer(t3f_timer);
		return false;
	}
	if(loop_end < 0.0)
	{
		loop_end = al_get_audio_stream_length_secs(t3f_stream);
	}
	al_set_audio_stream_loop_secs(t3f_stream, loop_start, loop_end);
	al_set_audio_stream_gain(t3f_stream, t3f_music_volume);
	if(loop_start >= 0.0 && loop_end > 0.0)
	{
		al_set_audio_stream_playmode(t3f_stream, ALLEGRO_PLAYMODE_LOOP);
	}
	al_attach_audio_stream_to_mixer(t3f_stream, al_get_default_mixer());
	al_start_timer(t3f_timer);
	return true;
}

void t3f_stop_music(void)
{
	al_stop_timer(t3f_timer);
	al_drain_audio_stream(t3f_stream);
	al_destroy_audio_stream(t3f_stream);
	t3f_stream = NULL;
	al_start_timer(t3f_timer);
}

void t3f_pause_music(void)
{
	al_set_audio_stream_playing(t3f_stream, false);
}

void t3f_resume_music(void)
{
	al_set_audio_stream_playing(t3f_stream, true);
}

void t3f_set_music_volume(float volume)
{
	t3f_music_volume = volume;
	if(t3f_stream)
	{
		al_set_audio_stream_gain(t3f_stream, t3f_music_volume);
	}
}

float t3f_get_music_volume(void)
{
	return t3f_music_volume;
}
