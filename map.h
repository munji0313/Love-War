#ifndef _MAP_H__
#define _MAP_H__

#define MAX_OBJECTS (20)

#include <stdbool.h>
#include <allegro5/allegro.h>

typedef struct Map {
	float x, y, w, h;
}map;

typedef struct Stage_Map {
	map objects[MAX_OBJECTS];
}stage_map;

typedef struct GameState GameState;

void Init_stage1_map(GameState* g);
void Init_stage2_map(GameState* g);
void Init_stage3_map(GameState* g);
void Init_stageBOSS_map(GameState* g);
#endif
