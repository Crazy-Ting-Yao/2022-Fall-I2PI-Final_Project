#ifndef checkbox_h
#define checkbox_h

#include "scene_menu_object.h"
#include "utility.h"
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct {
    RecArea body;
    ALLEGRO_BITMAP* default_img;
    ALLEGRO_BITMAP* checked_img;
    bool checked;
    bool hovered;
}Checkbox;
#endif /* checkbox_h */

Checkbox initCheckbox(float x, float y, float w, float h, const char* default_image_path, const char* checked_image_path);
void drawCheckbox(Checkbox checkbox);
bool CheckboxHover(Checkbox checkbox, int mouse_x, int mouse_y);
