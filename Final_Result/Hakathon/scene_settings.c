// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
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
#include "slider.h"
#include "checkbox.h"
// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.
/* Define your static vars / function prototypes below. */
// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.
// The only function that is shared across files.
static bool dragging=false;
static bool dragging_effect=false;
static bool pressed=false;
static bool selected[4] = {false,false,false,false};
static RecArea keys[4];
Slider volume;
Slider volume_effect;
Checkbox checkbox;

static void init() {
    checkbox = initCheckbox(200, 40, 50 ,50, "Assets/unchecked.png", "Assets/checked.png");
    volume = slider_init(300, 400, 210, 5, music_volume);
    volume_effect = slider_init(300, 400, 360, 5, effect_volume);
    for(int i=0;i<4;i++){
        keys[i].x=(SCREEN_W/5*(i+1))-50;
        keys[i].y=550;
        keys[i].w=100;
        keys[i].h=50;
    }
}

static void draw() {

    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_slider(volume);
    draw_slider(volume_effect);
    sound_alter(volume,volume_effect);
    drawCheckbox(checkbox);
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/2, 40,
        ALLEGRO_ALIGN_CENTER, "Cheat Mode");
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/2, 120,
        ALLEGRO_ALIGN_CENTER, "BGM Volume" );
    char buf[8];
    sprintf(buf,"%.1lf%%", music_volume*100);
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), 250, 185,
            ALLEGRO_ALIGN_RIGHT, buf );
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/2, 270,
        ALLEGRO_ALIGN_CENTER, "Effect Volume");
    char buf2[8];
    sprintf(buf2,"%.1lf%%", effect_volume*100);
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), 250, 335,
            ALLEGRO_ALIGN_RIGHT, buf2);
    al_draw_text(
        settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/2, 410, ALLEGRO_ALIGN_CENTER, "ALTER KEYS");
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5, 480, ALLEGRO_ALIGN_CENTER, "UP");
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*2, 480, ALLEGRO_ALIGN_CENTER, "LEFT");
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*3, 480, ALLEGRO_ALIGN_CENTER, "DOWN");
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*4, 480, ALLEGRO_ALIGN_CENTER, "RIGHT");
    for(int i=0;i<4;i++){
        if(selected[i]){
            al_draw_filled_rectangle(keys[i].x+2, keys[i].y+2, keys[i].x+keys[i].w-2, keys[i].y+keys[i].h-2, al_map_rgb(155, 155, 155));
        }
        al_draw_rectangle(keys[i].x, keys[i].y, keys[i].x+keys[i].w, keys[i].y+keys[i].h, al_map_rgb(255, 255, 255), 4);
        char code[2];
        if(keysfordir[i+1]>=1&&keysfordir[i+1]<=26){code[0]=(char)(keysfordir[i+1]+'A'-1);
            code[1]='\0';
            al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*(i+1), 550, ALLEGRO_ALIGN_CENTER, code);
        }
        else if(keysfordir[i+1]==ALLEGRO_KEY_UP)
            al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*(i+1), 550, ALLEGRO_ALIGN_CENTER, "↑");
        else if(keysfordir[i+1]==ALLEGRO_KEY_DOWN)
            al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*(i+1), 550, ALLEGRO_ALIGN_CENTER, "↓");
        else if(keysfordir[i+1]==ALLEGRO_KEY_RIGHT)
            al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*(i+1), 550, ALLEGRO_ALIGN_CENTER, "→");
        else if(keysfordir[i+1]==ALLEGRO_KEY_LEFT)
            al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/5*(i+1), 550, ALLEGRO_ALIGN_CENTER, "←");
    }
    
    al_draw_text( settingsFont, al_map_rgb(255, 255, 255), SCREEN_W/2,
        SCREEN_H - 150, ALLEGRO_ALIGN_CENTER, "PRESS \"ESC\" to return");
    
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
    //    [HACKATHON 3-7]
    //    TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
    //    Uncomment and fill the code below
    volume.hovered = SliderHover(volume, mouse_x, mouse_y);
    volume_effect.hovered = SliderHover(volume_effect, mouse_x, mouse_y);
    checkbox.hovered = CheckboxHover(checkbox, mouse_x, mouse_y);
    if(dragging) {
        volume.now_pos = mouse_x;
        if(volume.now_pos<volume.body.x) volume.now_pos = volume.body.x;
        else if(volume.now_pos>(volume.body.x+volume.body.w)) volume.now_pos = volume.body.x+volume.body.w;
    }
    else if(dragging_effect) {
        volume_effect.now_pos = mouse_x;
        if(volume_effect.now_pos<volume.body.x) volume_effect.now_pos = volume_effect.body.x;
        else if(volume_effect.now_pos>(volume_effect.body.x+volume_effect.body.w)) volume_effect.now_pos = volume_effect.body.x+volume_effect.body.w;
    }
}
/*

//    [HACKATHON 3-8]
//    TODO: When btnSettings clicked, switch to settings scene
//  `game_change_scene` is defined in `game.h`.
//  You can check line 121 `scene_menu.c` to see how to use this function.
//  And find the corresponding function for `scene_setting`
//     Utilize the member defined in struct `Button`.
//  The logic here is
//  `if clicked the mouse` && `mouse position is in the area of button`
//           `Enter the setting scene`
//    Uncomment and fill the code below
*/
static void on_mouse_down() {
    if (volume.hovered){
        dragging=true;
        volume.now_pos = mouse_x;
    }
    else if (volume_effect.hovered){
        dragging_effect=true;
        volume_effect.now_pos = mouse_x;
    }
    else if (checkbox.hovered){
        pressed = true;
    }
    else{
        for(int i=0;i<4;i++){
            if(pnt_in_rect(mouse_x,mouse_y,keys[i])) {
                if(selected[i]){
                    selected[i]=false;
                    break;
                }
                for(int j=0;j<4;j++){
                    selected[j]=false;
                }
                selected[i]=true;
                break;
            }
        }
    }
}
static void on_mouse_up() {
    dragging=false;
    dragging_effect=false;
    if(checkbox.hovered&&pressed){
        checkbox.checked = checkbox.checked ? false:true;
        cheat_mode = checkbox.checked ? true : false;
        if (cheat_mode)
            printf("cheat mode on\n");
        else
            printf("cheat mode off\n");
    }
}

static void destroy() {
    //al_destroy_bitmap(gameTitle);
    //    [HACKATHON 3-10]
    //    TODO: Destroy button images
    //    Uncomment and fill the code below
    al_destroy_bitmap(checkbox.checked_img);
    al_destroy_bitmap(checkbox.default_img);
}

static void on_key_down(int keycode) {
    if(keycode==ALLEGRO_KEY_ESCAPE){
        game_change_scene(scene_menu_create());
        return;
    }
    for(int i=0;i<4;i++){
        if(selected[i]){
            if((keycode<=ALLEGRO_KEY_Z&&keycode>=ALLEGRO_KEY_A)||(keycode<=ALLEGRO_KEY_DOWN&&keycode>=ALLEGRO_KEY_LEFT)){
                for(int j=0;j<4;j++){
                    if(keysfordir[j+1]==keycode){
                        keysfordir[j+1]=keysfordir[i+1];
                        keysfordir[i+1] = keycode;
                        return;
                    }
                }
                keysfordir[i+1]=keycode;
            }
            return;
        }
    }
}
Scene scene_settings_create(void) {

    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    scene.name = "Settings";
    scene.initialize = &init;
    scene.draw = &draw;
    scene.destroy = &destroy;
    scene.on_key_down = &on_key_down;
    scene.on_mouse_move = &on_mouse_move;
    scene.on_mouse_down = &on_mouse_down;
    scene.on_mouse_up = &on_mouse_up;
    // -------------------------------------



    // TODO: Register more event callback functions such as keyboard, mouse, ...
    game_log("Settings scene created");
    return scene;
}
