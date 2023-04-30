#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
static int animate_pos = 800;
ALLEGRO_BITMAP* pman_image;
static ALLEGRO_SAMPLE_ID WINBGM;
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.
/* Define your static vars / function prototypes below. */
// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
// The only function that is shared across files.
void init(void) {
    stop_bgm(WINBGM);
    WINBGM = play_bgm(PACMAN_SUCCESS, music_volume);
    animate_pos = 800;
    pman_image = load_bitmap("Assets/pacman_move.png");
}
static void draw() {
    animate_pos-=45;
    if(animate_pos<-150) {
        game_change_scene(scene_menu_create());
        return;
    }
    al_draw_text(
        EndFont,
        al_map_rgb(255, 255, 255),
        SCREEN_W/2,
        110,
        ALLEGRO_ALIGN_CENTER,
        "WIN"
    );
    if(((animate_pos+150)%180)>90){
        al_draw_scaled_bitmap(pman_image, 32, 0, 16, 16, animate_pos, SCREEN_H/2, 150, 150, 0);
    }
    else{
        al_draw_scaled_bitmap(pman_image, 48, 0, 16, 16, animate_pos, SCREEN_H/2, 150, 150, 0);
    }
    for(int i=50;i<=animate_pos;i+=100)
        al_draw_filled_circle(i, SCREEN_H/2+75, 20, al_map_rgb(234, 178, 38));
    al_rest(0.1);
}

static void destroy() {
    stop_bgm(WINBGM);
    al_destroy_bitmap(pman_image);
}

Scene scene_win_create(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.initialize = &init;
    scene.name = "Win";
    scene.draw = &draw;
    scene.destroy = &destroy;
    game_log("Win scene created");
    return scene;
}
