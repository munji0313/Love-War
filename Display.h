#ifndef _DISPLAY_H__
#define _DISPLAY_H__

#define _CRT_SECURE_NO_WARNINGS 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>


/* =========================================================
 * 게임 전역 상수 정의
 * ========================================================= */
#define SCREEN_W (1280)        // 화면 가로 크기
#define SCREEN_H (720)         // 화면 세로 크기
#define FPS (60.0)             // 초당 프레임 수
#define MAX_NICKNAME (20)      // 닉네임 최대 길이(널 문자 포함)
#define GROUND_Y (650)

 /* =========================================================
   * 버튼 구조체
   * 메뉴 화면에서 START, EXIT 버튼 표현에 사용
   * ========================================================= */
typedef struct Button {
    float x, y, w, h;     // 버튼의 위치(x, y)와 크기(width, height)
    char* text;     // 버튼에 표시할 문자열
    bool mouse;         // 마우스가 버튼 위에 올라가 있는지 여부
} Button;

typedef struct scores {
    int score;
    int time;
    char nickname[MAX_NICKNAME];

}scores;

typedef struct IMAGE {
    ALLEGRO_BITMAP* main_page;
    ALLEGRO_BITMAP* score_page;
    ALLEGRO_BITMAP* change_page;
    ALLEGRO_BITMAP* gameover_page;
    ALLEGRO_BITMAP* gameclear_page;

    ALLEGRO_BITMAP* background_stage_1;
    ALLEGRO_BITMAP* background_stage_2;
    ALLEGRO_BITMAP* background_stage_3;
    ALLEGRO_BITMAP* background_stage_boss;

    ALLEGRO_BITMAP* right_walking_1;
    ALLEGRO_BITMAP* right_walking_2;
    ALLEGRO_BITMAP* left_walking_1;
    ALLEGRO_BITMAP* left_walking_2;
    ALLEGRO_BITMAP* left_jumping;
    ALLEGRO_BITMAP* right_jumping;

    ALLEGRO_BITMAP* coin;
    ALLEGRO_BITMAP* clock;

    ALLEGRO_BITMAP* paper;
    ALLEGRO_BITMAP* heart;

    ALLEGRO_BITMAP* easy_enemy;
    ALLEGRO_BITMAP* easy_enemy_2;
    ALLEGRO_BITMAP* hard_enemy;
    ALLEGRO_BITMAP* easy_enemy_right;
    ALLEGRO_BITMAP* easy_enemy_right_2;
    ALLEGRO_BITMAP* hard_enemy_right;
    ALLEGRO_BITMAP* boss_enemy;
}IMAGE;

typedef struct FONT {
    ALLEGRO_FONT* font;
    ALLEGRO_FONT* HUD;
    ALLEGRO_FONT* font_title;
}FONT;

/* =========================================================
 * 장면(Scene)
 * 게임이 어떤 화면 상태인지 구분
 * ========================================================= */
typedef enum Scene {
    SCENE_MENU,   // 메뉴 화면
    SCENE_GAME,    // 실제 게임 화면
    SCENE_CHANGE,   // 전환 화면
    SCENE_SCORE,
    SCENE_OVER
} Scene;

typedef struct GameState GameState;

void Init_image(IMAGE* i);
void Init_font(FONT* f);

bool mouse_on_button(float mx, float my, float bx, float by, float bw, float bh);
void draw_input_box(FONT* font, float x, float y, float w, float h, char* nickname);
void draw_background();
void draw_hud_ui(FONT* font, char* nickname, IMAGE* i, GameState* g);
void append_char(char* str, int max_len, char c);
void draw_score_ui(FONT* font, IMAGE* i);
scores* scores_setting();
void draw_stage(GameState* g, IMAGE* i);
void draw_action(GameState* g, IMAGE* i);
void draw_heart_paper(GameState* g, IMAGE* i);
void draw_enemy(GameState* g, IMAGE* i);
void draw_scene_change(GameState* g, FONT* f, Scene* scene, IMAGE* i);
void Update_scene(GameState* g, Scene* scene);
void draw_over(GameState* g, IMAGE* i, FONT* f);
void draw_boss_hp(GameState* g);
#endif
