// [shared.h]
// you should put shared variables between files in this header.

#ifndef SCENE_SHARED_H
#define SCENE_SHARED_H
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>

// TODO: Shared constant variable (without initializing value).

// TODO: More shared resources or data that needed to be accessed
// across different scenes. initialize the value in 'shared.c'.;
extern ALLEGRO_SAMPLE* themeMusic;
extern ALLEGRO_SAMPLE* SUSMusic;
extern ALLEGRO_SAMPLE* PACMAN_MOVESOUND;
extern ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND;
extern ALLEGRO_SAMPLE* PACMAN_SUCCESS;
extern ALLEGRO_FONT* menuFont;
extern ALLEGRO_FONT* settingsFont;
extern ALLEGRO_FONT* EndFont;
extern ALLEGRO_TIMER* power_up_timer;
extern ALLEGRO_TIMER* freeze_timer;
extern int fontSize;
extern float music_volume;
extern float effect_volume;
extern bool gameDone;
extern int start_grid_x, start_grid_y;
extern int cage_grid_x, cage_grid_y;
extern int keysfordir[5];
extern int mapselection;
extern const char mapfiles[3][100];
// Initialize shared variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void shared_init(void);
// Free shared variables and resources.
void shared_destroy(void);

#endif
