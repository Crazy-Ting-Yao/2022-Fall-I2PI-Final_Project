#ifndef slider_h
#define slider_h
#include "game.h"
#include <stdio.h>
typedef struct{
    int now_pos;
    bool hovered;
    RecArea body;
}Slider;
Slider slider_init(int min_posx, int max_posx, int posy, float width, float vol);
bool SliderHover(Slider slider, int mouse_x, int mouse_y);
void draw_slider(Slider slider);
void sound_alter(Slider slider,Slider slider2);
#endif /* slider_h */
