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

Slider slider_init(int min_posx, int length, int posy, float thickness, float vol){
    Slider slider;
    slider.body.x = min_posx;
    slider.body.w = length;
    slider.body.y = posy;
    slider.body.h = thickness;
    slider.now_pos = (vol*length)+min_posx;
    slider.hovered=false;
    
    return slider;
}

void draw_slider(Slider slider){
    al_draw_filled_rectangle(slider.body.x, slider.body.y, slider.body.x+slider.body.w, slider.body.y+slider.body.h, al_map_rgb(255, 255, 255));
    al_draw_filled_circle(slider.now_pos, (float)slider.body.y+(slider.body.h/2), (slider.body.h*2), al_map_rgb(255,255,255));
}
void sound_alter(Slider slider,Slider slider2){
    music_volume = (float)(slider.now_pos-slider.body.x)/slider.body.w;
    effect_volume = (float)(slider2.now_pos-slider2.body.x)/slider2.body.w;
}
bool SliderHover(Slider slider, int mouse_x, int mouse_y) {
    return pnt_in_rect(mouse_x,mouse_y,slider.body);
}

