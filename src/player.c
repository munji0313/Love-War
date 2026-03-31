#include "player.h"
#include "Display.h"
#include "bullet.h"
#include "game.h"
#include "map.h"
#include "audio.h"

unsigned char key[ALLEGRO_KEY_MAX];
static double last_step_time = 0;

void Init_keyboard()
{
    memset(key, 0, sizeof(key));
}

void Update_keyboard(ALLEGRO_EVENT* event)
{
    switch (event->type)
    {
    case ALLEGRO_EVENT_TIMER:
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= ~KEY_SEEN;
        break;

    case ALLEGRO_EVENT_KEY_DOWN:
        key[event->keyboard.keycode] = KEY_SEEN | KEY_DOWN;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event->keyboard.keycode] &= ~KEY_DOWN;
        break;
    }
}
void Init_player(GameState* g) {
    // player 구조체 속 멤버 변수들 초기화
    Player* player = &g->player;

    player->x = SCREEN_W / 2 - PLAYER_WIDE / 2;   // 플레이어 바닥 중앙 위치
    player->y = GROUND_Y - PLAYER_HEIGHT;
    player->dy = 0;
    player->isJumping = false;
    // reset game에만 player.life = 3 해주고 나머지는 생명력 초기화 X
    //if (g->level == STAGE1) player->life = 3;
    //player->life = 3;
    player->isAlive = true;
    player->isInvincible = false;
    player->invincibleTime = 0;
    player->shotTimer = 0;  // 총알 쿨타임 초기화
    player->facingRight = true;  // 초기 방향
    player->isJumping = false;

}


void Update_player(GameState* g)
{
    Player* player = &g->player;

    if (key[ALLEGRO_KEY_LEFT]) {
        player->facingRight = false;

        //----------------------------------------------------
        double now = al_get_time();
        if (now - last_step_time > 0.2 && !player->isJumping) {
            al_play_sample(sound_step, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            last_step_time = now;
        }
        //-----------------------------------------------------

        if (player->x <= 0) {
            player->x = 0;
        }
        else {
            player->x -= PLAYER_SPEED;
        }

        Check_collision_X(g);
    }


    if (key[ALLEGRO_KEY_RIGHT]) {
        player->facingRight = true;

        //----------------------------------------------------
        double now = al_get_time();
        if (now - last_step_time > 0.2 && !player->isJumping) {
            al_play_sample(sound_step, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            last_step_time = now;
        }
        //---------------------------------------------------------

        if (player->x + PLAYER_WIDE >= SCREEN_W) {
            player->x = SCREEN_W - PLAYER_WIDE;
        }
        else {
            player->x += PLAYER_SPEED; // else 추가 계속 움직이지않고 멈춤
        }
        Check_collision_X(g);
    }

    bool onPlatform = false; // 발판 위에 있는지 확인


    for (int i = 0; i < MAX_OBJECTS; i++) {
        map obj = g->stage_map.objects[i];

        // 크기가 0이면 실제 없는 객체 건너뜀
        if (obj.w == 0 || obj.h == 0) continue;


        if (player->x < obj.x + obj.w &&
            player->x + PLAYER_WIDE > obj.x &&
            player->y + PLAYER_HEIGHT == obj.y)
        {
            onPlatform = true;   // 발판 위에 있음
        }
    }

    if (player->y + PLAYER_HEIGHT < GROUND_Y && !onPlatform) {
        player->isJumping = true;
    }

    // 점프 ------------------------------------
    if (key[ALLEGRO_KEY_UP] && !player->isJumping) {
        player->dy = -12;
        player->isJumping = true;

        // 점프 사운드
        al_play_sample(sound_jump, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    if (player->isJumping) {
        player->dy += GRAVITY;
        player->y += player->dy;

        Check_collision_Y(g);

        if (player->y >= GROUND_Y - PLAYER_HEIGHT) {
            player->y = GROUND_Y - PLAYER_HEIGHT;
            player->dy = 0;
            player->isJumping = false;
        }
        if (player->y >= GROUND_Y) {
            player->dy = 0;
        }
    }

    //------------------------------------

    if (player->shotTimer > 0) {
        player->shotTimer--;
    }

    else if (key[ALLEGRO_KEY_Z]) {
        player->shotTimer = 5;
        
        //--------------------------------------------------
        al_play_sample(sound_heart, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        //---------------------------------------------------

        Player_fire_bullets(g, player->x, player->y + 42, player->facingRight);
    }

    // life 및 invincible 확인 ----------------------

    Check_close_enemy_collision(g);

    if (player->isInvincible && player->invincibleTime > 0) {
        player->invincibleTime--;
    }
    else {
        player->isInvincible = false;
    }
}

// 부활 - 무적모드 2초 지속 (Update_player()에서 invincibleTime 처리)
void Revival(GameState* g) {
    g->player.isInvincible = true;
    g->player.invincibleTime = 2 * FPS;
}

void Is_attacked(GameState* g) {
    Player* player = &(g->player);
    if (player->isInvincible) {
        return;
    }
    if (player->life) {
        player->life--;

        //-----------------------------------------------------------
        al_play_sample(sound_attacked, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        //-------------------------------------------------------------

        if (player->life > 0) {
            Revival(g);
        }
        else {  // 남은 목숨이 없을 경우
            // 게임 종료
            g->game_over = true;
        }
    }
}

void Check_close_enemy_collision(GameState* g) {
    Enemy* enemy = g->enemies.enemy;
    int now_enemy = g->enemies.now_enemy;

    for (int i = 0; i < now_enemy; ++i) {
        if (!enemy[i].isAlive) continue;

        // enemy와 충돌 판정
        if (g->player.x < enemy[i].x + ENEMY_WEIGHT &&
            g->player.x + PLAYER_WIDE > enemy[i].x &&
            g->player.y < enemy[i].y + ENEMY_HEIGHT &&
            g->player.y + PLAYER_HEIGHT > enemy[i].y) {

            // 무적 상태 아닐 경우 Is_attacked() 호출
            if (!g->player.isInvincible) {
                Is_attacked(g);
            }
        }
    }
}

void Check_collision_X(GameState* g) {
    Player* player = &g->player;

    for (int i = 0; i < MAX_OBJECTS; ++i) {
        map obj = g->stage_map.objects[i];

        if (obj.w == 0 || obj.h == 0) continue;

        if (player->x < obj.x + obj.w &&
            player->x + PLAYER_WIDE > obj.x &&
            player->y < obj.y + obj.h &&
            player->y + PLAYER_HEIGHT > obj.y)
        {
            if (player->facingRight) {
                player->x = obj.x - PLAYER_WIDE;
            }

            else {
                player->x = obj.x + obj.w;
            }
        }
    }
}
void Check_collision_Y(GameState* g)
{
    Player* player = &g->player;

    // 모든 장애물 검사
    for (int i = 0; i < MAX_OBJECTS; i++) {
        map obj = g->stage_map.objects[i];

        // 없는 장애물은 무시
        if (obj.w == 0 || obj.h == 0) continue;


        if (player->x  < obj.x + obj.w &&
            player->x + PLAYER_WIDE > obj.x &&
            player->y < obj.y + obj.h &&
            player->y + PLAYER_HEIGHT > obj.y)
        {

            if (player->dy > 0) {

                player->y = obj.y - PLAYER_HEIGHT;


                player->dy = 0;


                player->isJumping = false;
            }
            else if (player->dy < 0) {

                player->y = obj.y + obj.h;

                player->dy = 0;

                player->isJumping = false;
            }
        }
    }
}
