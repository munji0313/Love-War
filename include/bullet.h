#ifndef BULLET_H
#define BULLET_H
#include <stdbool.h>

#define MAX_BULLETS (100)
#define BULLET_SPEED (3)
#define COLLISION_DIST (16) 
#define ENEMY_COLLISION_DIST (25) 
#define BOSS_COLLISION_DIST (50) 

typedef struct {
    float x, y;
    float dx, dy;
    bool player;    // 총알의 발원지 (player / enemy)
    bool used;
} Bullet;

typedef struct GameState GameState;

void Init_bullets(GameState* g);
void Player_fire_bullets(GameState* g, float x, float y, bool facingRight);
void Enemy_fire_bullets(GameState* g, float x, float y, float playerx, float playery);
void Update_bullets(GameState* g, float deltaTime);
void Check_bullet_collision(GameState* g);

#endif
