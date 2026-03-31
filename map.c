#include "map.h"
#include "game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

void Init_stage1_map(GameState* g) {
	for (int i = 0; i < MAX_OBJECTS; i++) {
		g->stage_map.objects[i] = (map){ 0,0,0,0 };
	}
	g->stage_map.objects[0] = (map){ 911,439,195,12 };
	g->stage_map.objects[1] = (map){ 610,391,196,12 };
	g->stage_map.objects[2] = (map){ 292,346,193,12 };
	g->stage_map.objects[3] = (map){ 34,198,214,11 };
}
void Init_stage2_map(GameState* g) {
	for (int i = 0; i < MAX_OBJECTS; i++) {
		g->stage_map.objects[i] = (map){ 0,0,0,0 };
	}
	g->stage_map.objects[0] = (map){ 76,331,270,47 };
	g->stage_map.objects[1] = (map){ 93,356,238,44 };
	g->stage_map.objects[2] = (map){ 128,377,167,46 };
	g->stage_map.objects[3] = (map){ 159,409,84,31 };
	g->stage_map.objects[4] = (map){ 171,429,39,24 };

	g->stage_map.objects[5] = (map){ 439,517,322,117 };

	g->stage_map.objects[6] = (map){ 834,567,500,100 };
	g->stage_map.objects[7] = (map){ 1022,526,117,56 };
	g->stage_map.objects[8] = (map){ 1056,483,59,96 };

	g->stage_map.objects[9] = (map){ 926,310,251,72 };
	g->stage_map.objects[10] = (map){ 982,337,139,78 };
	g->stage_map.objects[11] = (map){ 1034,396,45,34 };
}
void Init_stage3_map(GameState* g) {
	for (int i = 0; i < MAX_OBJECTS; i++) {
		g->stage_map.objects[i] = (map){ 0,0,0,0 };
	}
	g->stage_map.objects[0] = (map){ 143, 396, 132, 95 };
	g->stage_map.objects[1] = (map){ 83, 440, 60, 52 };
	g->stage_map.objects[2] = (map){ 460, 459, 308 ,54 };
	g->stage_map.objects[3] = (map){ 585, 413, 95, 44 };
	g->stage_map.objects[4] = (map){ 718, 363, 264, 69 };
	g->stage_map.objects[5] = (map){ 795, 318, 85, 60 };
	g->stage_map.objects[6] = (map){ 914, 285, 286, 74 };
	g->stage_map.objects[7] = (map){ 983, 229, 35, 70 };
	g->stage_map.objects[8] = (map){ 1018, 183, 101, 110 };

}
void Init_stageBOSS_map(GameState* g) {
	for (int i = 0; i < MAX_OBJECTS; i++) {
		g->stage_map.objects[i] = (map){ 0,0,0,0 };
	}
	g->stage_map.objects[0] = (map){ 301, 583, 76 ,70 };
	g->stage_map.objects[1] = (map){ 192, 539, 106, 115 };
	g->stage_map.objects[2] = (map){ 54, 555, 135, 100 };
	g->stage_map.objects[3] = (map){ 455, 508, 210, 15 };
	g->stage_map.objects[4] = (map){ 457, 388, 171, 18 };
	g->stage_map.objects[5] = (map){ 305, 320, 155, 13 };
}
