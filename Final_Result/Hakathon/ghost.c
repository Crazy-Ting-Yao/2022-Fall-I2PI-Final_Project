#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "ghost.h"
#include "map.h"
#include "pacman_obj.h"

/* global variables*/
// [ NOTE ]
// if you change the map .txt to your own design.
// You have to modify cage_grid_{x,y} to corressponding value also.
// Or you can do some change while loading map (reading .txt file)
// Make the start position metadata stored with map.txt.
/* shared variables. */
extern uint32_t GAME_TICK;
extern uint32_t GAME_TICK_CD;
extern const int block_width,  block_height;
extern int cage_grid_x, cage_grid_y;
extern ALLEGRO_SAMPLE* PACMAN_EATGHOST_SOUND;
static ALLEGRO_SAMPLE_ID PACMAN_EATGHOST_ID;
/* Internal variables */
static const int fix_draw_pixel_offset_x = -3;
static const int fix_draw_pixel_offset_y = -3;
static const int draw_region = 30;
extern int power_up_duration;
// [ NOTE - speed again ]
// Again, you see this notaficationd. If you still want to implement something 
// fancy with speed, objData->moveCD and GAME_TICK, you can first start on 
// working on animation of ghosts and pacman. // Once you finished the animation 
// part, you will have more understanding on whole mechanism.
static const int basic_speed = 2;

Ghost* ghost_create(int flag) {

	// NOTODO
	Ghost* ghost = (Ghost*)malloc(sizeof(Ghost));
	if (!ghost)
		return NULL;

	ghost->typeFlag = flag;
	ghost->objData.Size.x = block_width;
	ghost->objData.Size.y = block_height;

	ghost->objData.nextTryMove = NONE;
	ghost->speed = basic_speed;
	ghost->status = BLOCKED;

	ghost->flee_sprite = load_bitmap("Assets/ghost_flee.png");
	ghost->dead_sprite = load_bitmap("Assets/ghost_dead.png");

	switch (ghost->typeFlag) {
	case Blinky:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y+1;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
    case Pinky:
        ghost->objData.Coord.x = cage_grid_x;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_pink.png");
        ghost->move_script = &ghost_pink_move_script;
        break;
    case Inky:
        ghost->objData.Coord.x = cage_grid_x-1;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_blue.png");
        ghost->move_script = &ghost_blue_move_script;
        break;
    case Clyde:
        ghost->objData.Coord.x = cage_grid_x+1;
        ghost->objData.Coord.y = cage_grid_y;
        ghost->move_sprite = load_bitmap("Assets/ghost_move_orange.png");
        ghost->move_script = &ghost_orange_move_script;
        break;
	default:
		ghost->objData.Coord.x = cage_grid_x;
		ghost->objData.Coord.y = cage_grid_y;
		ghost->move_sprite = load_bitmap("Assets/ghost_move_red.png");
		ghost->move_script = &ghost_red_move_script;
		break;
	}
	return ghost;
}
void ghost_destory(Ghost* ghost) {
    al_destroy_bitmap(ghost->move_sprite);
    al_destroy_bitmap(ghost->flee_sprite);
    al_destroy_bitmap(ghost->dead_sprite);
    free(ghost);
}
void ghost_draw(Ghost* ghost) {
	// getDrawArea return the drawing RecArea defined by objData and GAME_TICK_CD
	RecArea drawArea = getDrawArea(ghost->objData, GAME_TICK_CD);
	// [TODO] below is for animation usage, change the sprite you want to use.
    if (ghost->status == FLEE) {
        if(al_get_timer_count(power_up_timer)<=(power_up_duration-3)){
            if((ghost->objData.moveCD % 60)<30)
                al_draw_scaled_bitmap(ghost->flee_sprite, 0, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
            else al_draw_scaled_bitmap(ghost->flee_sprite, 16, 0,
                                       16, 16,
                                       drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                       draw_region, draw_region, 0
                                       );
        }
        else{
            int drawint = (ghost->objData.moveCD % 60)/15;
            al_draw_scaled_bitmap(ghost->flee_sprite, drawint*16, 0,
                                      16, 16,
                                      drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                      draw_region, draw_region, 0
                                      );
        }
        
    }
    else if (ghost->status== GO_IN) {
        switch(ghost->objData.facing){
            case UP:
                al_draw_scaled_bitmap(ghost->dead_sprite, 32, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                
                break;
            case LEFT:
                al_draw_scaled_bitmap(ghost->dead_sprite, 16, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
            default:
            case RIGHT:
                al_draw_scaled_bitmap(ghost->dead_sprite, 0, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
            case DOWN:
                al_draw_scaled_bitmap(ghost->dead_sprite, 48, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
        }
    }
	else {
        switch(ghost->objData.facing){
            case UP:
                if((ghost->objData.moveCD % 60)<30)
                    al_draw_scaled_bitmap(ghost->move_sprite, 64, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                else al_draw_scaled_bitmap(ghost->move_sprite, 80, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                
                break;
            case LEFT:
                if((ghost->objData.moveCD % 60)<30)
                    al_draw_scaled_bitmap(ghost->move_sprite, 32, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                else al_draw_scaled_bitmap(ghost->move_sprite, 48, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
            default:
            case RIGHT:
                if((ghost->objData.moveCD % 60)<30)
                    al_draw_scaled_bitmap(ghost->move_sprite, 0, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                else al_draw_scaled_bitmap(ghost->move_sprite, 16, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
            case DOWN:
                if((ghost->objData.moveCD % 60)<30)
                    al_draw_scaled_bitmap(ghost->move_sprite, 96, 0,
                        16, 16,
                        drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                        draw_region, draw_region, 0
                    );
                else al_draw_scaled_bitmap(ghost->move_sprite, 112, 0,
                                           16, 16,
                                           drawArea.x + fix_draw_pixel_offset_x, drawArea.y + fix_draw_pixel_offset_y,
                                           draw_region, draw_region, 0
                                       );
                break;
        }
    }
}
void ghost_NextMove(Ghost* ghost, Directions next) {
	ghost->objData.nextTryMove = next;
}
void printGhostStatus(GhostStatus S) {

	switch(S){
	
	case BLOCKED: // stay inside the ghost room
		game_log("BLOCKED");
		break;
	case GO_OUT: // going out the ghost room
		game_log("GO_OUT");
		break;
	case FREEDOM: // free at the map
		game_log("FREEDOM");
		break;
	case GO_IN:
		game_log("GO_IN");
		break;
	case FLEE:
		game_log("FLEE");
		break;
	default:
		game_log("status error");
		break;
	}
}
bool ghost_movable(Ghost* ghost, Map* M, Directions targetDirec, bool room) {
	// [HACKATHON 2-3]
	// TODO: Determine if the current direction is movable.
	// Basically, this is a ghost version of `pacman_movable`.
	// So if you have finished (and you should), you can just "copy and paste"
	// and do some small alternation.

	int gx = ghost->objData.Coord.x, gy = ghost->objData.Coord.y;

	switch (targetDirec){
	case UP:
            gy--;
            break;
	case DOWN:
            gy++;
            break;
	case LEFT:
            gx--;
            break;
	case RIGHT:
            gx++;
            break;
	default:
		// for none UP, DOWN, LEFT, RIGHT direction u should return false.
		return false;
	}

	if (is_wall_block(M, gx, gy) || (room && is_room_block(M, gx, gy)))
		return false;
    
	return true;

}

void ghost_toggle_FLEE(Ghost* ghost, bool setFLEE) {
	// [TODO]
	// TODO: Here is reserved for power bean implementation.
	// The concept is "When pacman eats the power bean, only
	// ghosts who are in state FREEDOM will change to state FLEE.
	// For those who are not (BLOCK, GO_IN, etc.), they won't change state."
	// This implementation is based on the classic PACMAN game.
	// You are allowed to do your own implementation of power bean system.
		if(setFLEE){
			if(ghost->status == FREEDOM){
                ghost->status = FLEE;
                ghost->speed = 1;
			}
		}
        else{
            if(ghost->status == FLEE)
                ghost->status = FREEDOM;
                ghost->speed = 2;
		}
}

void ghost_collided(Ghost* ghost) {
	if (ghost->status == FLEE) {
        stop_bgm(PACMAN_EATGHOST_ID);
        PACMAN_EATGHOST_ID = play_audio(PACMAN_EATGHOST_SOUND, effect_volume);
        al_set_timer_count(freeze_timer, 1);
        al_start_timer(freeze_timer);
		ghost->status = GO_IN;
		ghost->speed = 4;
	}
}

void ghost_move_script_GO_IN(Ghost* ghost, Map* M) {
	// Description
	// `shortest_path_direc` is a function that returns the direction of shortest path.
	// Check `map.c` for its detail usage.
	// For GO_IN state.
	ghost->objData.nextTryMove = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, cage_grid_x, cage_grid_y);
}
void ghost_move_script_GO_OUT(Ghost* ghost, Map* M) {
	// Description
	// Here we always assume the room of ghosts opens upward.
	// And used a greedy method to drag ghosts out of room.
	// You should modify here if you have different implementation/design of room.
	if(M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'B'||M->map[ghost->objData.Coord.y][ghost->objData.Coord.x] == 'R')
		ghost_NextMove(ghost, UP);
	else
		ghost->status = FREEDOM;
}
void ghost_move_script_FLEE(Ghost* ghost, Map* M, const Pacman * const pacman) {
	// [TODO]
	Directions shortestDirection = shortest_path_direc(M, ghost->objData.Coord.x, ghost->objData.Coord.y, pacman->objData.Coord.x, pacman->objData.Coord.y);
    static Directions proba[4]; // possible movement
    int cnt = 0;
    for (Directions i = 1; i <= 4; i++){
        if (i!=shortestDirection&&ghost_movable(ghost, M, i, ghost->status==FREEDOM))     proba[cnt++] = i;
    }
    if(cnt) ghost_NextMove(ghost, proba[random()%cnt]);
    else ghost_NextMove(ghost, shortestDirection);
}
