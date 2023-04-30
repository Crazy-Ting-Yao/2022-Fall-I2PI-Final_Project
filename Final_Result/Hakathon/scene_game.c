#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "scene_win.h"

#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
static ALLEGRO_SAMPLE_ID SUSBGM;
int game_main_Score = 0;
bool game_over = false;
extern int power_up_duration;
/* Internal variables*/
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
bool debug_mode = false;
bool cheat_mode = false;
static float progress;
/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
//static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void render_init_screen(void);
static void draw_hitboxes(void);
static void init(void) {
	game_over = false;
	game_main_Score = 0;
    basic_map = create_map(mapfiles[mapselection]);
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
    ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		for (int i = 0; i < GHOST_NUM; i++) {
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);
			if (!ghosts[i])
				game_abort("error creating ghost\n");
		}
	}
	GAME_TICK = 0;
    if(mapselection==1){
        stop_bgm(SUSBGM);
        SUSBGM = play_bgm(SUSMusic, music_volume);
    }
	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
    if (!power_up_timer)
        game_abort("Error on create timer\n");
    freeze_timer = al_create_timer(1.0f);
    if (!freeze_timer)
        game_abort("Error on freeze timer\n");
    al_set_timer_count(freeze_timer, 0);
	
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, '.',ghosts);
        game_main_Score+=10;
        basic_map->beansCount--;
        break;
    case 'P':
        pacman_eatItem(pman, 'P',ghosts);
        al_set_timer_count(power_up_timer, 0);
        al_start_timer(power_up_timer);
        basic_map->beansCount--;
        game_main_Score+=50;
        break;
	default:
		break;
	}
    basic_map->map[Grid_y][Grid_x]=' ';
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		if((!cheat_mode) && RecAreaOverlap(getDrawArea(pman->objData, GAME_TICK_CD), getDrawArea(ghosts[i]->objData, GAME_TICK_CD))){
            if(ghosts[i]->status==FLEE){
                ghost_collided(ghosts[i]);
            }
            else{
                game_log("collide with ghost\n");
                stop_bgm(SUSBGM);
                al_rest(1.0);
                pacman_die();
                al_set_timer_count(pman->death_anim_counter, 0);
                al_start_timer(pman->death_anim_counter);
                game_over = true;
                break;
            }
		}
	}
    if(al_get_timer_count(power_up_timer)>=power_up_duration){
        al_stop_timer(power_up_timer);
        pman->powerUp=false;
        for(int i=0;i<GHOST_NUM;i++)
            ghost_toggle_FLEE(ghosts[i], false);
    }
}

static void update(void) {
    if(!basic_map->beansCount){
        game_change_scene(scene_win_create());
        return;
    }
    if(al_get_timer_count(freeze_timer)>=2){
        al_stop_timer(freeze_timer);
        al_set_timer_count(freeze_timer, 0);
    }
    else if(al_get_timer_count(freeze_timer)>=1){
        return;
    }
    if (game_over){
        if(al_get_timer_count(pman->death_anim_counter)>=(12*8)) {
            game_change_scene(scene_menu_create());
        }
		return;
	}
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);
	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	//500-750 800
    al_draw_rectangle(500, 10, 750, 40, al_map_rgb(255, 255, 255), 2);
    progress = 1-(float)basic_map->beansCount/basic_map->beansNum;
    al_draw_filled_rectangle(500, 10, progress*250+500, 40, al_map_rgb(255, 255, 255));
    al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            475,
            16,
            ALLEGRO_ALIGN_RIGHT,
            "Progress:"
        );
    char buf[20];
    sprintf(buf, "%s%d", "SCORE: ",game_main_Score);
    al_draw_text(
            menuFont,
            al_map_rgb(255, 255, 255),
            16,
            16,
            ALLEGRO_ALIGN_LEFT,
            buf
        );
	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++)
		ghost_draw(ghosts[i]);
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}
/*
static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}
*/

static void destroy(void) {
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
    stop_bgm(SUSBGM);
    delete_map(basic_map);
    pacman_destroy(pman);
    al_destroy_timer(power_up_timer);
    al_destroy_timer(freeze_timer);
    for(int i=0;i<GHOST_NUM;i++){
        ghost_destory(ghosts[i]);
    }
    free(ghosts);
}

static void on_key_down(int key_code) {
    if(key_code==ALLEGRO_KEY_ESCAPE) game_change_scene(scene_menu_create());
    else if(key_code==keysfordir[1]) pacman_NextMove(pman, UP);
    else if (key_code==keysfordir[2]) pacman_NextMove(pman, LEFT);
    else if (key_code==keysfordir[3]) pacman_NextMove(pman, DOWN);
    else if (key_code==keysfordir[4]) pacman_NextMove(pman, RIGHT);
    else if (key_code==keysfordir[0]) debug_mode = !debug_mode;
}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);

	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
