// 수정시간 2026-03-31 00:01 재혁
#ifndef GAME_H
#define GAME_H
#define MAX_LIFE (20)

#include "boss.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "Display.h"
#include "map.h"

typedef enum Level {
    STAGE1,
    STAGE2,
    STAGE3,
    BOSS,
    OVER
} Level;

/* =========================================================
 * 게임 전체 상태 구조체
 * 플레이어, 총알, 적, 점수
 * ========================================================= */
typedef struct GameState {
    Player player;                          // 플레이어
    Enemy_manage enemies;             // 적 배열
    Boss boss;
    Bullet bullets[MAX_BULLETS];
    Level level;
    scores my_score;
    stage_map stage_map;
    double time;
    int score;                              // 현재 점수
    bool game_over;                         // 게임 오버 여부
    bool stage_over;
    int frame;
    int score_count;
} GameState;
// =========================================================


int Init_game();
void Update_game(GameState* g);
void Reset_game(GameState* g);
void Compare_ranking(GameState* g);
void Save_ranking(scores* l);
void Load_ranking(scores* l);
#endif