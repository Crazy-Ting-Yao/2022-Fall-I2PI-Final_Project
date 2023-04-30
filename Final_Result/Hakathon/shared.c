// [shared.c]
// you should define the shared variable declared in the header here.

#include "shared.h"
#include "utility.h"
#include "game.h"
#include "scene_menu.h"

ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* SUSMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_SUCCESS = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* settingsFont = NULL;
ALLEGRO_FONT* EndFont = NULL;
ALLEGRO_TIMER* power_up_timer;
ALLEGRO_TIMER* freeze_timer;
int fontSize = 30;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;
int start_grid_x, start_grid_y;
int cage_grid_x, cage_grid_y;
int power_up_duration = 10;
int mapselection=0;
int keysfordir[5]={ALLEGRO_KEY_G,ALLEGRO_KEY_W,ALLEGRO_KEY_A,ALLEGRO_KEY_S,ALLEGRO_KEY_D};
const char mapfiles[3][100]={"Assets/test.txt","Assets/AMOGUS.txt","Assets/map_nthu.txt"};
/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
    settingsFont = load_font("Assets/Cubic.ttf", 45);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
    SUSMusic = load_audio("Assets/Music/amogus.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
    PACMAN_EATGHOST_SOUND = load_audio("Assets/Music/pacman_eatghost.ogg");
    EndFont = load_font("Assets/Minecraft.ttf", fontSize*2);
    PACMAN_SUCCESS = load_audio("Assets/Music/SuccessBGM.ogg");

}

void shared_destroy(void) {

	al_destroy_font(menuFont);
    al_destroy_font(settingsFont);
    al_destroy_font(EndFont);
	al_destroy_sample(themeMusic);
    al_destroy_sample(SUSMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
    al_destroy_sample(PACMAN_EATGHOST_SOUND);
    al_destroy_sample(PACMAN_SUCCESS);
}
