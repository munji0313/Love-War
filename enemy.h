//수정시간 2026-03-31 00:01 재혁

// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#define JUMP_HEIGHT (100)
#define ENEMY_WEIGHT (50)
#define ENEMY_HEIGHT (100)


#include <stdlib.h>
#include <stdbool.h>

typedef struct GameState GameState;

typedef struct {
    float x, y, jump;
    int type;    // 근접 0 or 원거리 1
    bool isAlive; // live 1, dead 0
    bool use; //use 1, unuse 0
    bool facingRight;
    int shotTimer; //근접 0, 원거리의 경우 랜덤 값으로 설정
    float dy;
    int isJumping;
    bool onPlatform;
    bool spawn;
} Enemy;

typedef struct {

    Enemy enemy[1000];
    int max_enemy; //최대적, 맵 난이도에 따라 조정가능
    int now_enemy;  //현재까지 소환된 적의 수
    int cnt_enemy;  //맵 안에 존재하는 적의 수
    int spawn_cnt;  // 적 소환 쿨타임
    long cycle;

} Enemy_manage;

void Init_enemymanage(GameState* g);
void Create_enemy(GameState* g);
void Update_enemy(GameState* g);
void Render_enemy(GameState* g);


int Random_i(int min, int max);
float Random_f(float min, float max);

#endif