//수정시간 2026-03-31 00:01 재혁
// boss.h
#ifndef BOSS_H
#define BOSS_H

#include <stdlib.h>
#include <stdbool.h>

#define BOSS_WEIGHT 205
#define BOSS_HEIGHT 300

typedef struct GameState GameState;

typedef struct {
    float x, y;
    int hp;
    bool isAlive;
    int shotTimer;
    int pattern;
    int invincibleTime;
} Boss;

void Boss_pattern(GameState* g);
void Init_boss(GameState* g);
void Update_boss(GameState* g);

#endif
