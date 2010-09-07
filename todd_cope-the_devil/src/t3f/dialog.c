#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>

typedef struct
{
	
	char * path;
	char * title;
	char * patterns;
	int mode;
	
} T3F_DIALOG_ARGS;

static bool (*t3f_internal_dialog_finish_proc)(char * fn) = NULL;
static ALLEGRO_NATIVE_DIALOG * t3f_dialog = NULL;

void * t3f_file_select_thread_proc(void * p)
{
	const ALLEGRO_PATH * rp = NULL;
	al_show_native_file_dialog(t3f_dialog);
	rp = al_get_native_file_dialog_path(t3f_dialog, 0);
	if(rp)
	{
		if(t3f_internal_dialog_finish_proc)
		{
			t3f_internal_dialog_finish_proc((char *)al_path_cstr(rp, '/'));
		}
	}
	al_destroy_native_dialog(t3f_dialog);
	return NULL;
}

void t3f_file_select(bool (*proc)(char *), char * path, char * title, char * patterns, int mode)
{
	T3F_DIALOG_ARGS args;
	args.path = path;
	args.title = title;
	args.patterns = patterns;
	args.mode = mode;
	t3f_internal_dialog_finish_proc = proc;
	ALLEGRO_PATH * initial = al_create_path(args.path);
	
	t3f_dialog = al_create_native_file_dialog(initial, args.title, args.patterns, args.mode);
	if(t3f_dialog)
	{
		al_run_detached_thread(t3f_file_select_thread_proc, &args);
	}
}
