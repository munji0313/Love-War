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
#include "audio.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

static int prev_level = -1;

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
    Button quit_button = { 447,364,387,67, "QUIT", false };

    bool running = true;
    bool redraw = true;
    float mouse_x = 0.0f, mouse_y = 0.0f;  // 현재 마우스 좌표

    Scene scene = SCENE_MENU;
    char nickname[MAX_NICKNAME] = "";

    GameState game;
    IMAGE image;
    FONT font;
    scores* score_list = scores_setting();

    long play_time_check = 0;
    Reset_game(&game);
    game.level = STAGE1;
    unsigned char key[ALLEGRO_KEY_MAX];
    Init_keyboard();
    Init_image(&image);
    Init_font(&font);
    Init_audio();
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
                    al_set_audio_stream_playing(basic_bgm, true);
                    if (mouse_on_button(mouse_x, mouse_y, start_button.x, start_button.y, start_button.w, start_button.h)) {
                        if ((int)strlen(nickname) > 0) {
                            Reset_game(&game);
                            strcpy(game.my_score.nickname, nickname);
                            scene = SCENE_GAME;
                            play_time_check = al_get_time();

                            // 메뉴 BGM 끄기
                            al_set_audio_stream_playing(basic_bgm, false);

                            // stage1 BGM 켜기
                            al_seek_audio_stream_secs(stage_bgm[0], 0.0);
                            al_set_audio_stream_playing(stage_bgm[0], true);

                            // prev_level 초기화
                            prev_level = STAGE1;
                        }
                    }
                    if (mouse_on_button(mouse_x, mouse_y, exit_button.x, exit_button.y, exit_button.w, exit_button.h)) running = false;
                    if (mouse_on_button(mouse_x, mouse_y, score_button.x, score_button.y, score_button.w, score_button.h)) scene = SCENE_SCORE;
                }
                else if (scene == SCENE_SCORE) {
                    if (mouse_on_button(mouse_x, mouse_y, back_button.x, back_button.y, back_button.w, back_button.h)) scene = SCENE_MENU;
                }
                else if (scene == SCENE_OVER) {
                    al_set_audio_stream_playing(sound_gameover, true);
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
                        play_time_check = al_get_time();

                        // 메뉴 BGM 끄기
                        al_set_audio_stream_playing(basic_bgm, false);

                        // stage1 BGM 켜기
                        al_seek_audio_stream_secs(stage_bgm[0], 0.0);
                        al_set_audio_stream_playing(stage_bgm[0], true);

                        // prev_level 초기화
                        prev_level = STAGE1;
                    }
                }
            }
            break;
        case ALLEGRO_EVENT_KEY_UP:
            key[ev.keyboard.keycode] = false;
            break;
        case ALLEGRO_EVENT_TIMER:
            if (scene == SCENE_MENU) {
                static bool menu_bgm_playing = false;
                if (!menu_bgm_playing) {
                    al_seek_audio_stream_secs(basic_bgm, 0.0);
                    al_set_audio_stream_playing(basic_bgm, true);
                    menu_bgm_playing = true;
                }
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
                game.time = al_get_time() - play_time_check;
            }
            else if (scene == SCENE_CHANGE) {
                static int prev_level = -1;

                if (game.level != prev_level) {

                    // 모든 BGM 끄기
                    for (int i = 0; i < 4; i++) {
                        al_set_audio_stream_playing(stage_bgm[i], false);
                    }

                    // 새 BGM
                    if (game.level >= STAGE1 && game.level <= BOSS) {
                        int idx = game.level;

                        al_seek_audio_stream_secs(stage_bgm[idx], 0.0);
                        al_set_audio_stream_playing(stage_bgm[idx], true);
                    }

                    prev_level = game.level;
                }

                Update_scene(&game, &scene);
            }
            else if (scene == SCENE_OVER) {
                static bool over_bgm = false;

                if (!over_bgm) {
                    for (int i = 0; i < 4; i++) {
                        al_set_audio_stream_playing(stage_bgm[i], false);
                    }

                    al_play_sample(sound_gameover, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

                    over_bgm = true;
                }

                if (game.score_count == 0) {
                    Compare_ranking(&game);
                    game.score_count = 1;
                }
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
                draw_over(&game, &image, &font);
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
    audio_deinit();

    return 0;
}
