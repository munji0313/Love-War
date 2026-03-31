#define _CRT_SECURE_NO_WARNINGS 

#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>


int Init_game() {
    /* Allegro 시스템 초기화 */
    if (!al_init()) {
        printf("Allegro init failed\n");
        return -1;
    }
    /* 마우스 입력 시스템 설치 */
    if (!al_install_mouse()) {
        printf("Mouse install failed\n");
        return -1;
    }
    /* 키보드 입력 시스템 설치 */
    if (!al_install_keyboard()) {
        printf("Keyboard install failed\n");
        return -1;
    }
    /* 오디오 시스템 설치 */
    if (!al_install_audio()) {
        printf("Audio install failed\n");
    }

    /* 폰트 및 기본 도형 그리기 기능 초기화 */
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    return 0;
}

void Update_game(GameState* g) {

    Init_boss(g);

    if (g->level == STAGE1) {
        g->level = STAGE2;
        Init_stage2_map(g);
    }
    else if (g->level == STAGE2) {
        g->level = STAGE3;
        Init_stage3_map(g);
    }
    else if (g->level == STAGE3) {
        g->level = BOSS;
        g->boss.isAlive = true;
        Init_stageBOSS_map(g);
    }
    else if (g->level == BOSS) {
        g->level = OVER;
    }
    Init_player(g);

    Init_enemymanage(g);
    Init_bullets(g);

    g->stage_over = false;
}


void Reset_game(GameState* g) {
    /* 게임 진행값 초기화 */
    g->score = 0;
    g->game_over = false;
    g->time = 0;
    g->frame = 0;
    g->level = STAGE1;
    g->stage_over = false;
    g->player.life = MAX_LIFE;
    g->score_count = 0;
    Init_player(g);
    Init_enemymanage(g);
    Init_bullets(g);
    Init_boss(g);
    Init_stage1_map(g);
}

/**/
void Load_ranking(scores* l) {
    (void)freopen("ranking.txt", "r", stdin);  // 읽기 모드
    // 파일에서 데이터 읽기
    for (int i = 0; i < 10; i++) {
        (void)scanf("%s", l[i].nickname);
    }
    for (int j = 0; j < 10; j++) {
        (void)scanf("%d", &l[j].score);
    }
    for (int k = 0; k < 10; k++) {
        (void)scanf("%d", &l[k].time);
    }
}

void Save_ranking(scores* l) {
    FILE* fp = fopen("ranking.txt", "w");
    if (fp == NULL) {
        printf("파일 열기 실패\n");
        return;
    }
    // 랭킹 10개를 파일에 저장
    for (int i = 0; i < 10; i++) {
        fprintf(fp, "%s\n", l[i].nickname);
    }
    for (int j = 0; j < 10; j++) {
        fprintf(fp, "%d\n", l[j].score);
    }
    for (int k = 0; k < 10; k++) {
        fprintf(fp, "%d\n", l[k].time);
    }
    fclose(fp);  // 파일 닫기
}


void Compare_ranking(GameState* g) { // 현재 점수가 10등보다 낮으면 탈락
    scores l[10];
    Load_ranking(l);
    if (g->score < l[9].score) {
        return;
    }
    if (g->score == l[9].score) { // 점수가 같을 경우: 시간비교
        if ((int)g->time > l[9].time) {
            return;
        }
    }
    l[9].score = g->score;
    l[9].time = (int)g->time;
    strcpy(l[9].nickname, g->my_score.nickname);
    scores temp = l[9];
    for (int i = 8; i >= 0; i--) {
        if (temp.score > l[i].score) { // 점수가 더 크다면 값 이동
            l[i + 1] = l[i];
            if (i == 0) l[0] = temp;
        }
        else if (temp.score == l[i].score) { // 점수가 같으면 시간비교
            if (temp.time < l[i].time) {
                l[i + 1] = l[i]; // 기존 값 뒤로 이동
                if (i == 0) l[0] = temp;
            }
            else {
                l[i + 1] = temp;
                break;
            }
        }
        else {
            l[i + 1] = temp;
            break;
        }
    }
    Save_ranking(l);
}
