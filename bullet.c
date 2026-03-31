// 수정시간 2026-03-31 00:02 재혁
#include "bullet.h"
#include "game.h"

void Init_bullets(GameState* g) {
    Bullet* bullets = g->bullets;
    for (int i = 0; i < MAX_BULLETS; ++i) {
        bullets[i].used = false;
    }
}

void Player_fire_bullets(GameState* g, float x, float y, bool facingRight) {
    Bullet* bullets = g->bullets;
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].used) {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].dx = facingRight ? BULLET_SPEED : -BULLET_SPEED; // x방향 이동
            bullets[i].dy = 0;
            bullets[i].player = true;
            bullets[i].used = true;
            break;
        }
    }
}

void Enemy_fire_bullets(GameState* g, float x, float y, float playerx, float playery) {
    Bullet* bullets = g->bullets;
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].used) {
            bullets[i].x = x;
            bullets[i].y = y;
            float diffx = playerx - x;
            float diffy = playery - y;
            float dist = sqrtf(diffx * diffx + diffy * diffy);

            if (dist == 0) {
                dist = 1;
            }
            bullets[i].dx = BULLET_SPEED * (diffx / dist);
            bullets[i].dy = BULLET_SPEED * (diffy / dist);

            bullets[i].player = false;
            bullets[i].used = true;

            break;
        }
    }
}

// deltaTime : 1.0 / 초당 프레임 수
void Update_bullets(GameState* g, float deltaTime) {
    Bullet* bullets = g->bullets;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].used) {
            bullets[i].x += bullets[i].dx * deltaTime;
            bullets[i].y += bullets[i].dy * deltaTime;

            // 화면 밖이면 제거
            if (bullets[i].x < 0 || bullets[i].x > SCREEN_W || bullets[i].y < 0 || bullets[i].y > SCREEN_H) {
                bullets[i].used = false;
            }
            // 땅에 닿으면 제거
            if (bullets[i].y > GROUND_Y) {
                bullets[i].used = false;
            }

            // 지형지물에 닿으면 제거
            for (int j = 0; j < MAX_OBJECTS; ++j) {
                map obj = g->stage_map.objects[j];

                if (obj.w == 0 || obj.h == 0) continue;

                if ((bullets[i].x < obj.x + obj.w &&
                    bullets[i].x > obj.x) &&
                    (bullets[i].y < obj.y + obj.h &&
                        bullets[i].y > obj.y)) {

                    bullets[i].used = false;
                }
            }
        }
    }
}

void Check_bullet_collision(GameState* g) {
    Bullet* bullets = g->bullets;
    Player* player = &g->player;
    Enemy* enemy = g->enemies.enemy;
    Enemy_manage* enemies = &g->enemies;
    int now_enemy = g->enemies.now_enemy;
    Boss* boss = &g->boss;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!bullets[i].used) continue;

        bool hit = false; // enemy 피격 확인 후 boss 피격 검사 건너뛰기 위한 변수

        // player가 쏜 총알
        if (bullets[i].player) {

            // enemy 충돌 체크
            for (int j = 0; j < now_enemy; ++j) {
                if (!enemy[j].isAlive) continue;

                float dx = bullets[i].x - (enemy[j].x + PLAYER_WIDE / 2);
                float dy = bullets[i].y - (enemy[j].y + PLAYER_HEIGHT / 2);
                if (dx * dx + dy * dy < ENEMY_COLLISION_DIST * ENEMY_COLLISION_DIST) {
                    bullets[i].used = false;
                    enemy[j].isAlive = false;
                    enemies->cnt_enemy--;
                    g->score += 10;

                    if (enemies->cnt_enemy == 0 && boss->isAlive == false && enemies->now_enemy == enemies->max_enemy) {
                        g->stage_over = true;
                    }

                    hit = true;
                    break;
                }
            }

            if (hit) continue; // enemy가 맞았다면 해당 총알은 검사 끝.

            // boss 충돌 체크
            if (boss->isAlive) {
                float dx = bullets[i].x - (boss->x + BOSS_WEIGHT / 2);
                float dy = bullets[i].y - (boss->y + BOSS_HEIGHT / 2);

                if (dx * dx + dy * dy < BOSS_COLLISION_DIST * BOSS_COLLISION_DIST) { 
                    bullets[i].used = false;
                    boss->hp--;

                    if (boss->hp <= 0) {
                        boss->isAlive = false;
                        g->score += 1000;
                        g->game_over = true;
                    }
                }
            }
        }

        // enemy / boss가 쏜 총알
        else {
            if (player->isInvincible) { // 무적 상태면 continue
                continue;
            }

            float dx = bullets[i].x - (player->x + PLAYER_WIDE / 2);
            float dy = bullets[i].y - (player->y + PLAYER_HEIGHT / 2);

            if (dx * dx + dy * dy < COLLISION_DIST * COLLISION_DIST) {
                Is_attacked(g);
                bullets[i].used = false;
            }
        }
    }
}