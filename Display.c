#include "Display.h"
#include "map.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>


bool mouse_on_button(float mx, float my, float bx, float by, float bw, float bh) {
    return (mx >= bx && mx <= bx + bw && my >= by && my <= by + bh);
}

void draw_input_box(FONT* font, float x, float y, float w, float h, char* nickname) {
    ALLEGRO_COLOR text_color = al_map_rgb(20, 20, 20);

    /* 닉네임이 없으면 안내 메시지, 있으면 실제 닉네임 출력 */
    if ((int)strlen(nickname) == 0) {
        al_draw_text(font->font, al_map_rgb(120, 120, 120), x + 10, y + 15, 0, "Enter your nickname...");
    }
    else {
        al_draw_text(font->font, text_color, x + 10, y + 15, 0, nickname);
    }
}

void draw_background() {
    al_clear_to_color(al_map_rgb(0, 0, 128));
    al_draw_filled_rectangle(0, GROUND_Y, SCREEN_W, SCREEN_H, al_map_rgb(0, 255, 0));
}

void draw_hud_ui(FONT* font, char* nickname, IMAGE* i, GameState* g) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Player name : %s", nickname);
    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    al_draw_text(font->HUD, text_color, 20, 20, 0, buffer);
    // 시간
    al_draw_bitmap(i->clock, 20, 40, 0);
    double time = g->time;
    int minutes = ((int)time / 60) % 60;
    int seconds = (int)time % 60;
    int milliseconds = (int)((time - (int)time) * 1000) / 10;
    snprintf(buffer, sizeof(buffer), " %02d : %02d : %02d", minutes, seconds, milliseconds);
    al_draw_text(font->HUD, text_color, 60, 45, 0, buffer);
    // 목숨
    al_draw_bitmap(i->coin, 20, 80, 0);
    al_draw_text(font->HUD, text_color, 55, 85, 0, " : ");
    char life_buffer[256];
    snprintf(buffer, sizeof(buffer), "%d", g->player.life);
    al_draw_text(font->HUD, al_map_rgb(255, 255 * (g->player.life) / MAX_LIFE, 255 * (g->player.life) / MAX_LIFE), 70, 85, 0, buffer);

    // 점수
    snprintf(buffer, sizeof(buffer), " KILL : %d", g->score);
    al_draw_text(font->HUD, text_color, 100, 85, 0, buffer);

}

void append_char(char* str, int max_len, char c) {
    int len = (int)strlen(str);

    /* 현재 문자열 길이가 최대 허용 길이보다 작을 때만 문자 추가 */
    if (len < max_len - 1) {
        str[len] = c;         // 새 문자 추가
        str[len + 1] = '\0';  // 문자열 끝 표시
    }
}


void draw_score_ui(FONT* font, IMAGE* i) {
    al_draw_scaled_bitmap(
        i->score_page,
        0, 0,
        al_get_bitmap_width(i->score_page),
        al_get_bitmap_height(i->score_page),
        0, 0,
        SCREEN_W,
        SCREEN_H,
        0
    );
    ALLEGRO_COLOR text_color = al_map_rgb(0, 0, 0);
    scores l[10];
    (void)freopen("ranking.txt", "r", stdin);  // 읽기 모드
    char buffer[256];
    // 파일에서 데이터 읽기
    for (int i = 0; i < 10; i++) {
        (void)scanf("%s", l[i].nickname);
        snprintf(buffer, sizeof(buffer), "%s", (char*)l[i].nickname);
        al_draw_text(font->font, text_color, 425, 250 + i * 33, 0, buffer);
    }
    for (int j = 0; j < 10; j++) {
        (void)scanf("%d", &l[j].score);
        snprintf(buffer, sizeof(buffer), "%d", l[j].score);
        al_draw_text(font->font, text_color, 675, 250 + j * 33, 0, buffer);
    }
    for (int k = 0; k < 10; k++) {
        (void)scanf("%d", &l[k].time);
        snprintf(buffer, sizeof(buffer), "%d", l[k].time);
        al_draw_text(font->font, text_color, 825, 250 + k * 33, 0, buffer);
    }
}

scores* scores_setting() {
    scores* list = (scores*)calloc(10, sizeof(scores));
    if (list == NULL) {
        return NULL;
    }
    (void)freopen("ranking.txt", "r", stdin);
    for (int i = 0; i < 10; i++) {
        (void)scanf("%s", list[i].nickname);
    }
    for (int i = 0; i < 10; i++) {
        (void)scanf("%d", &list[i].score);
    }
    for (int i = 0; i < 10; i++) {
        (void)scanf("%d", &list[i].time);
    }

    return list;
}


void draw_stage(GameState* g, IMAGE* i) {
    switch (g->level) {
    case STAGE1:
        al_draw_scaled_bitmap(
            i->background_stage_1,
            0, 0,
            al_get_bitmap_width(i->background_stage_1),
            al_get_bitmap_height(i->background_stage_1),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        break;
    case STAGE2:
        al_draw_scaled_bitmap(
            i->background_stage_2,
            0, 0,
            al_get_bitmap_width(i->background_stage_2),
            al_get_bitmap_height(i->background_stage_2),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        break;
    case STAGE3:
        al_draw_scaled_bitmap(
            i->background_stage_3,
            0, 0,
            al_get_bitmap_width(i->background_stage_3),
            al_get_bitmap_height(i->background_stage_3),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        break;
    case BOSS:
        al_draw_scaled_bitmap(
            i->background_stage_boss,
            0, 0,
            al_get_bitmap_width(i->background_stage_boss),
            al_get_bitmap_height(i->background_stage_boss),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );

        break;
    }
}

void draw_action(GameState* g, IMAGE* i) {
    float player_x = g->player.x;
    float player_y = g->player.y;
    if (((g->player.invincibleTime / 2) % 3) == 1)return;

    if (g->player.isJumping) {
        if (g->player.facingRight) al_draw_bitmap(i->right_jumping, player_x, player_y, 0);
        else al_draw_bitmap(i->left_jumping, player_x, player_y, 0);
    }
    else {
        if (g->player.facingRight == true) {
            if ((int)player_x % 44 <= 22) {
                al_draw_bitmap(i->right_walking_1, player_x, player_y, 0);
            }
            else {
                al_draw_bitmap(i->right_walking_2, player_x, player_y, 0);
            }
        }
        else {
            if ((int)player_x % 44 <= 22) {
                al_draw_bitmap(i->left_walking_1, player_x, player_y, 0);
            }
            else {
                al_draw_bitmap(i->left_walking_2, player_x, player_y, 0);
            }
        }
    }
}

void Init_image(IMAGE* i) {
    i->main_page = al_load_bitmap("start_page.png");
    i->score_page = al_load_bitmap("score_page.png");
    i->change_page = al_load_bitmap("change_page.png");
    i->gameover_page = al_load_bitmap("gameover_page.png");
    i->gameclear_page = al_load_bitmap("gameclear_page.png");

    i->background_stage_1 = al_load_bitmap("map_1.png");
    i->background_stage_2 = al_load_bitmap("map_2.png");
    i->background_stage_3 = al_load_bitmap("map_3.png");
    i->background_stage_boss = al_load_bitmap("map_4.png");

    i->right_walking_1 = al_load_bitmap("right_main_walk_1.png");
    i->right_walking_2 = al_load_bitmap("right_main_walk2.png");
    i->left_walking_1 = al_load_bitmap("left_main_walk_1.png");
    i->left_walking_2 = al_load_bitmap("left_main_walk_2.png");
    i->left_jumping = al_load_bitmap("left_main_jump.png");
    i->right_jumping = al_load_bitmap("right_main_jump.png");

    i->clock = al_load_bitmap("clock.png");
    i->coin = al_load_bitmap("hp.png");

    i->easy_enemy = al_load_bitmap("easy_enemy.png");
    i->easy_enemy_2 = al_load_bitmap("easy_enemy_2.png");
    i->hard_enemy = al_load_bitmap("hard_enemy.png");
    i->easy_enemy_right = al_load_bitmap("easy_enemy_right.png");
    i->easy_enemy_right_2 = al_load_bitmap("easy_enemy_right_2.png");
    i->hard_enemy_right = al_load_bitmap("hard_enemy_right.png");
    i->boss_enemy = al_load_bitmap("BOSS.png");

    i->paper = al_load_bitmap("paper.png");
    i->heart = al_load_bitmap("heart.png");
}

void Init_font(FONT* f) {
    f->font = al_create_builtin_font();
    f->HUD = al_load_ttf_font("C:/Windows/Fonts/Arial.ttf", 20, 0);
    f->font_title = al_load_ttf_font("C:/Windows/Fonts/Arial.ttf", 40, 0);
}

void draw_heart_paper(GameState* g, IMAGE* i) {
    Bullet* bullets = g->bullets;

    for (int j = 0; j < MAX_BULLETS; j++) {
        if (!bullets[j].used) {
            continue;
        }
        if (bullets[j].player == true) {
            al_draw_bitmap(i->heart, bullets[j].x, bullets[j].y, 0);
        }
        else if (bullets[j].player == false) {
            al_draw_bitmap(i->paper, bullets[j].x, bullets[j].y, 0);
        }
    }
}

void draw_enemy(GameState* g, IMAGE* i) {
    Enemy_manage* enemies = &(g->enemies);
    Boss* boss = &(g->boss);

    if (boss->isAlive == 1) {
        draw_boss_hp(g);
        al_draw_bitmap(i->boss_enemy, boss->x, boss->y, 0);
    }

    for (int j = 0; j < enemies->now_enemy; j++)
    {
        if (!enemies->enemy[j].isAlive)
            continue;
        if (!enemies->enemy[j].use)
            continue;
        if (enemies->enemy[j].type == 0) {// 근접일 경우
            if (enemies->enemy[j].facingRight == true) {
                if ((int)(enemies->enemy[j].x) % 44 <= 22) {
                    al_draw_bitmap(i->easy_enemy_right_2, enemies->enemy[j].x, enemies->enemy[j].y, 0);
                }
                else {
                    al_draw_bitmap(i->easy_enemy_right, enemies->enemy[j].x, enemies->enemy[j].y, 0);
                }
            }
            else {
                if ((int)(enemies->enemy[j].x) % 44 <= 22) {
                    al_draw_bitmap(i->easy_enemy_2, enemies->enemy[j].x, enemies->enemy[j].y, 0);
                }
                else {
                    al_draw_bitmap(i->easy_enemy, enemies->enemy[j].x, enemies->enemy[j].y, 0);
                }
            }
        }
        else if (enemies->enemy[j].type == 1) {
            {
                if (enemies->enemy[j].x < g->player.x) {
                    al_draw_bitmap(i->hard_enemy_right, enemies->enemy[j].x, enemies->enemy[j].y, 0);
                }
                else al_draw_bitmap(i->hard_enemy, enemies->enemy[j].x, enemies->enemy[j].y, 0);
            }

        }
    }
}

void Update_scene(GameState* g, Scene* scene) {
    if (g->level == OVER) {
        *scene = SCENE_OVER;
    }
    g->frame += 1;
    if (g->frame > 70) {
        g->frame = 0;
        *scene = SCENE_GAME;
    }
}

void draw_scene_change(GameState* g, FONT* f, Scene* scene, IMAGE* i) {
    switch (g->level) {
    case STAGE1:
        al_draw_scaled_bitmap(
            i->change_page,
            0, 0,
            al_get_bitmap_width(i->change_page),
            al_get_bitmap_height(i->change_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        al_draw_text(f->font_title, al_map_rgb(0, 0, 0), SCREEN_W / 2, 323,
            ALLEGRO_ALIGN_CENTER, "1");
        break;
    case STAGE2:
        al_draw_scaled_bitmap(
            i->change_page,
            0, 0,
            al_get_bitmap_width(i->change_page),
            al_get_bitmap_height(i->change_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        al_draw_text(f->font_title, al_map_rgb(0, 0, 0), SCREEN_W / 2, 323,
            ALLEGRO_ALIGN_CENTER, "2");
        break;
    case STAGE3:
        al_draw_scaled_bitmap(
            i->change_page,
            0, 0,
            al_get_bitmap_width(i->change_page),
            al_get_bitmap_height(i->change_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        al_draw_text(f->font_title, al_map_rgb(0, 0, 0), SCREEN_W / 2, 323,
            ALLEGRO_ALIGN_CENTER, "3");
        break;
    case BOSS:
        al_draw_scaled_bitmap(
            i->change_page,
            0, 0,
            al_get_bitmap_width(i->change_page),
            al_get_bitmap_height(i->change_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        al_draw_text(f->font_title, al_map_rgb(0, 0, 0), SCREEN_W / 2, 323,
            ALLEGRO_ALIGN_CENTER, "BOSS");
        break;
    }

}


void draw_over(GameState* g, IMAGE* i, FONT* f) {
    ALLEGRO_COLOR text_color = al_map_rgb(0, 0, 0);
    char buffer[256];

    if (g->level == BOSS && g->boss.hp == 0) {
        al_draw_scaled_bitmap(
            i->gameclear_page,
            0, 0,
            al_get_bitmap_width(i->gameclear_page),
            al_get_bitmap_height(i->gameclear_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        snprintf(buffer, sizeof(buffer), "%d", g->score);
        al_draw_text(f->font, text_color, 523, 531, 0, buffer);
        snprintf(buffer, sizeof(buffer), "%d", ((int)(g->time)));
        al_draw_text(f->font, text_color, 761, 531, 0, buffer);
    }
    else {
        al_draw_scaled_bitmap(
            i->gameover_page,
            0, 0,
            al_get_bitmap_width(i->gameover_page),
            al_get_bitmap_height(i->gameover_page),
            0, 0,
            SCREEN_W,
            SCREEN_H,
            0
        );
        snprintf(buffer, sizeof(buffer), "%d", g->score);
        al_draw_text(f->font, text_color, 523, 531, 0, buffer);
        snprintf(buffer, sizeof(buffer), "%d", ((int)(g->time)));
        al_draw_text(f->font, text_color, 761, 531, 0, buffer);
    }

}

void draw_boss_hp(GameState* g) {
    al_draw_filled_rectangle(g->boss.x, g->boss.y - 30, g->boss.x + BOSS_WEIGHT, g->boss.y - 30 - 50, al_map_rgb(128, 128, 128));
    al_draw_filled_rectangle(g->boss.x + 10, g->boss.y - 40, g->boss.x + 10 + 185, g->boss.y - 40 - 30, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(g->boss.x + 10, g->boss.y - 40, g->boss.x + 10 + (int)(185 * (g->boss.hp) / BOSS_HP), g->boss.y - 40 - 30, al_map_rgb(255, 0, 0));
}
