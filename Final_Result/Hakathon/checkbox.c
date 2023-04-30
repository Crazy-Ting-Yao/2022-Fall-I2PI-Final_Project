#include "checkbox.h"
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

Checkbox initCheckbox(float x, float y, float w, float h, const char* default_image_path, const char* checked_image_path) {

    Checkbox checkbox;
    memset(&checkbox, 0, sizeof(Checkbox));

    checkbox.default_img = load_bitmap(default_image_path);
    if (checked_image_path) {
        checkbox.checked_img = load_bitmap(checked_image_path);
    }
    else checkbox.checked_img = load_bitmap(default_image_path);
    if (!checkbox.default_img) {
        game_log("failed loading checkbox image!");
    }
    checkbox.checked=cheat_mode ? true:false;
    checkbox.body.x = x;
    checkbox.body.y = y;
    checkbox.body.w = w;
    checkbox.body.h = h;

    return checkbox;
}

void drawCheckbox(Checkbox checkbox) {
    ALLEGRO_BITMAP* _img = checkbox.checked? checkbox.checked_img : checkbox.default_img;
    al_draw_scaled_bitmap(
        _img,
        0, 0,
        al_get_bitmap_width(_img), al_get_bitmap_height(_img),
                          checkbox.body.x, checkbox.body.y,
                          checkbox.body.w, checkbox.body.h, 0
    );
}
bool CheckboxHover(Checkbox checkbox, int mouse_x, int mouse_y) {
    //    [HACKATHON 3-6]
    //    TODO: Check if mouse is hovering on the button
    //    Uncomment and fill the code below
    return pnt_in_rect(mouse_x,mouse_y,checkbox.body);
}
