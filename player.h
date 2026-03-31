//수정시간 2026-03-31 00:02 재혁
#ifndef _PLAYER_H__
#define _PLAYER_H__
#include <allegro5/allegro5.h>


#define PLAYER_SPEED (3)  // 캐릭터 움직이는 속도
#define KEY_SEEN     (1)  // 
#define KEY_DOWN     (2)  // 
#define PLAYER_HEIGHT (100)  // 캐릭터 키
#define PLAYER_WIDE (70)    // 캐릭터 너비
#define GRAVITY (0.3)

typedef struct {
    float x, y;
    int life;    // 목숨 (3으로 초기화)
    float dy;
    bool isJumping;
    bool facingRight;
    bool isAlive; // 살아 있나?
    bool isInvincible;  // 무적인가??
    int invincibleTime;    // 무적 효과 남은 시간
    int shotTimer; // 총을 연속으로 못 쏘도록 하게 만드는 시간 
} Player;

typedef struct GameState GameState;
unsigned char key[ALLEGRO_KEY_MAX];
void Init_keyboard();
void Update_keyboard(ALLEGRO_EVENT* event);
void Init_player(GameState* g);
void Update_player(GameState* g);
void Revival(GameState* g); // 플레이어 부활
void Is_attacked(GameState* g); // 플레이어가 공격당했을 때 호출
void Check_collision_X(GameState* g);
void Check_collision_Y(GameState* g);
void Check_close_enemy_collision(GameState* g); // enemy와 충돌 판정
#endif