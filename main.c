// 수정시간 2026-03-31 00:01 재혁

#define _CRT_SECURE_NO_WARNINGS 
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Display.h"
#include "boss.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "map.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>


int main(void) {
    Init_game();
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_keyboard_event_source());

    // 버튼 
    Button start_button = { 479, 364, 317, 60, "START", false };
    Button exit_button = { 479, 439, 317, 60, "EXIT", false };
    Button score_button = { 479, 512, 317, 60, "SCORE", false };
    Button back_button = { 497,636,292,60,"BACK", false };
    Button quit_button = { 480,366,321,68, "QUIT", false };

    bool running = true;
    bool redraw = true;
    float mouse_x = 0.0f, mouse_y = 0.0f;  // 현재 마우스 좌표

    Scene scene = SCENE_MENU;
    char nickname[MAX_NICKNAME] = "";

    GameState game;
    IMAGE image;
    FONT font;
    scores* score_list = scores_setting();
    Reset_game(&game);
    game.level = STAGE1;
    unsigned char key[ALLEGRO_KEY_MAX];
    Init_keyboard();
    Init_image(&image);
    Init_font(&font);
    //srand((unsigned)time(NULL));
    al_start_timer(timer);
    ALLEGRO_EVENT ev;

    while (running) {

        al_wait_for_event(queue, &ev);
        switch (ev.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            running = false;
            break;
        case ALLEGRO_EVENT_MOUSE_AXES:
            mouse_x = (float)ev.mouse.x;
            mouse_y = (float)ev.mouse.y;
            break;
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
            if (ev.mouse.button == 1) {
                if (scene == SCENE_MENU) {
                    if (mouse_on_button(mouse_x, mouse_y, start_button.x, start_button.y, start_button.w, start_button.h)) {
                        if ((int)strlen(nickname) > 0) {
                            Reset_game(&game);
                            strcpy(game.my_score.nickname, nickname);
                            scene = SCENE_GAME;
                        }
                    }
                    if (mouse_on_button(mouse_x, mouse_y, exit_button.x, exit_button.y, exit_button.w, exit_button.h)) running = false;
                    if (mouse_on_button(mouse_x, mouse_y, score_button.x, score_button.y, score_button.w, score_button.h)) scene = SCENE_SCORE;
                }
                else if (scene == SCENE_SCORE) {
                    if (mouse_on_button(mouse_x, mouse_y, back_button.x, back_button.y, back_button.w, back_button.h)) scene = SCENE_MENU;
                }
                else if (scene == SCENE_OVER) {
                    if (mouse_on_button(mouse_x, mouse_y, quit_button.x, quit_button.y, quit_button.w, quit_button.h)) scene = SCENE_SCORE;
                }
            }
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            key[ev.keyboard.keycode] = true;

            /* ESC 키 처리 */
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                if (scene == SCENE_GAME) {
                    /* 게임 중 ESC -> 메뉴로 이동 */
                    scene = SCENE_MENU;
                }
                else if (scene == SCENE_SCORE) {
                    scene = SCENE_MENU;
                }
                else {
                    /* 메뉴에서 ESC -> 프로그램 종료 */
                    running = false;
                }
            }
            /* 메뉴 화면 + 입력창 활성 상태일 때 닉네임 입력 처리 */
            if (scene == SCENE_MENU) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
                    /* 마지막 글자 삭제 */
                    int len = (int)strlen(nickname);
                    if (len > 0) nickname[len - 1] = '\0';
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    /* 공백 입력 */
                    append_char(nickname, MAX_NICKNAME, ' ');
                }
                else if (ev.keyboard.keycode >= ALLEGRO_KEY_A && ev.keyboard.keycode <= ALLEGRO_KEY_Z) {
                    /* 영문자 입력 */
                    char c = 'A' + (ev.keyboard.keycode - ALLEGRO_KEY_A);

                    /* Shift가 눌리지 않았으면 소문자로 변환 */
                    if (!(ev.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT)) {
                        c = (char)tolower(c);
                    }

                    append_char(nickname, MAX_NICKNAME, c);
                }
                else if (ev.keyboard.keycode >= ALLEGRO_KEY_0 && ev.keyboard.keycode <= ALLEGRO_KEY_9) {
                    /* 숫자 입력 */
                    char c = '0' + (ev.keyboard.keycode - ALLEGRO_KEY_0);
                    append_char(nickname, MAX_NICKNAME, c);
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    /* Enter 입력 시 닉네임 검증 후 게임 시작 */
                    if ((int)strlen(nickname) > 0) {
                        Reset_game(&game);
                        strcpy(game.my_score.nickname, nickname);
                        scene = SCENE_GAME;
                    }
                }
            }
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[ev.keyboard.keycode] = false;
            break;
        case ALLEGRO_EVENT_TIMER:
            if (scene == SCENE_MENU) {
                start_button.mouse = mouse_on_button(mouse_x, mouse_y, start_button.x, start_button.y, start_button.w, start_button.h);
                exit_button.mouse = mouse_on_button(mouse_x, mouse_y, exit_button.x, exit_button.y, exit_button.w, exit_button.h);
            }
            else if (scene == SCENE_GAME) {
                if (game.stage_over == true) {
                    Update_game(&game);
                    scene = SCENE_CHANGE;
                }
                if (game.game_over == true) {
                    scene = SCENE_OVER;
                }
                Update_player(&game);
                Create_enemy(&game);
                Update_enemy(&game);
                Update_boss(&game);
                Update_bullets(&game, (FPS / 15));
                Check_bullet_collision(&game);
                game.time = al_get_time();
            }
            else if (scene == SCENE_CHANGE) {
                Update_scene(&game, &scene);
            }
            else if (scene == SCENE_OVER) {
                if (game.score_count == 0) {
                    Compare_ranking(&game);
                    game.score_count = 1;
                }
                //Compare_ranking(&game);
                //scene = SCENE_SCORE;
            }
            redraw = true;
            break;
        }

        Update_keyboard(&ev);
        if (redraw && al_is_event_queue_empty(queue)) {
            redraw = false;
            if (scene == SCENE_MENU) {
                al_draw_scaled_bitmap(
                    image.main_page,
                    0, 0,
                    al_get_bitmap_width(image.main_page),
                    al_get_bitmap_height(image.main_page),
                    0, 0,
                    SCREEN_W,
                    SCREEN_H,
                    0
                );
                draw_input_box(&font, 390, 300, 500, 60, nickname);
            }

            else if (scene == SCENE_GAME) {
                draw_stage(&game, &image);
                draw_action(&game, &image);
                draw_enemy(&game, &image);
                draw_heart_paper(&game, &image);
                draw_hud_ui(&font, nickname, &image, &game);
            }
            else if (scene == SCENE_SCORE) {
                draw_score_ui(&font, &image);
            }
            else if (scene == SCENE_CHANGE) {
                draw_scene_change(&game, &font, &scene, &image);
            }
            else if (scene == SCENE_OVER) {
                draw_over(&image);
            }
            al_flip_display();
        }

    }

    al_destroy_font(font.font);
    al_destroy_font(font.font_title);
    al_destroy_font(font.HUD);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(image.background_stage_1);

    return 0;
}