//수정시간 2026-03-31 00:01 재혁
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "game.h"

int Max_enemies[4] = { 5, 10, 10, 500 };

/*-------------------보조 함수-----------------------*/
int Random_i(int min, int max) {
	return (int)min + rand() % (max - min + 1);
}

float Random_f(float min, float max) {
	return min + rand() % (int)(max - min + 1);
}

void Chk_e_collision_X(GameState* g, int target) {
	Enemy* e = &(g->enemies.enemy[target]);

	for (int i = 0; i < MAX_OBJECTS; ++i) {
		map obj = g->stage_map.objects[i];

		if (obj.w == 0 || obj.h == 0) continue;

		if (e->x < obj.x + obj.w &&
			e->x + PLAYER_WIDE > obj.x &&
			e->y < obj.y + obj.h &&
			e->y + PLAYER_HEIGHT > obj.y)
		{

			if (g->player.y <= e->y) {
				e->dy = -12;
				e->isJumping = true;
			}

			if (e->facingRight) {
				e->x = obj.x - PLAYER_WIDE;
			}

			else {
				e->x = obj.x + obj.w;
			}
		}

	}
}

void Chk_e_collision_Y(GameState* g, int target)
{
	Enemy* e = &(g->enemies.enemy[target]);

	// 모든 장애물 검사
	for (int i = 0; i < MAX_OBJECTS; i++) {
		map obj = g->stage_map.objects[i];

		// 없는 장애물은 무시
		if (obj.w == 0 || obj.h == 0) continue;


		if (e->x < obj.x + obj.w &&
			e->x + PLAYER_WIDE > obj.x &&
			e->y < obj.y + obj.h &&
			e->y + PLAYER_HEIGHT > obj.y)
		{
			if (e->dy > 0) {

				e->y = obj.y - PLAYER_HEIGHT;
				e->dy = 0;
				e->isJumping = false;
			}
			else if (e->dy < 0) {

				e->y = obj.y + obj.h;
				e->dy = 0;
				e->isJumping = false;
			}
		}
	}
}


void Move_enemy(GameState* g, int target) {

	Enemy* e = &(g->enemies.enemy[target]);
	float player_x = g->player.x;
	float player_y = g->player.y;

	e->onPlatform = false;

	if (e->x > player_x) //플레이어가 왼쪽에 있을 시
	{
		e->facingRight = false;
		e->x -= 2;
		Chk_e_collision_X(g, target);
	}
	else if (e->x < player_x) //플레이어가 오른쪽에 있을 시 
	{
		e->facingRight = true;
		e->x += 2;
		Chk_e_collision_X(g, target);
	}

	for (int i = 0; i < MAX_OBJECTS; i++) {
		map obj = g->stage_map.objects[i];

		// 크기가 0이면 실제 없는 객체 건너뜀
		if (obj.w == 0 || obj.h == 0) continue;

		if (e->x < obj.x + obj.w &&
			e->x + PLAYER_WIDE > obj.x &&
			e->y + PLAYER_HEIGHT == obj.y)
		{
			e->onPlatform = true;   // 발판 위에 있음
		}
	}



	if (e->y + PLAYER_HEIGHT < GROUND_Y && !e->onPlatform) {
		e->isJumping = true;
	}

	if (e->isJumping) {
		e->dy += 0.5;
		e->y += e->dy;

		Chk_e_collision_Y(g, target);

		if (e->y >= GROUND_Y - PLAYER_HEIGHT) {
			e->y = GROUND_Y - PLAYER_HEIGHT;
			e->dy = 0;
			e->isJumping = false;
		}
		if (e->y >= GROUND_Y) {
			e->dy = 0;
		}
	}


}

/*-------------------주요 함수-----------------------*/

void Init_enemymanage(GameState* g) {
	Enemy_manage* em = &(g->enemies);

	em->max_enemy = Max_enemies[g->level];

	em->now_enemy = 0;
	em->spawn_cnt = 30;
	em->cnt_enemy = 0;
}


void Create_enemy(GameState* g) { //적 생성

	Enemy_manage* em = &(g->enemies);
	//spawn 카운터가 0이 되고 현재 까지 생성한 적이 스테이지 별 할당 적 숫자에 도달하기 전이면 생성
	if (em->spawn_cnt == 0 && em->now_enemy < em->max_enemy) {

		Enemy* new_enemy = &(em->enemy[(em->now_enemy)++]);

		new_enemy->isAlive = 1;
		new_enemy->use = 1;
		new_enemy->isJumping = false;
		new_enemy->facingRight = 0;
		new_enemy->dy = 0;
		new_enemy->spawn = true;

		int spawn_type[4] = { 0, 2, 1, 2 };
		int now_spawn_type = spawn_type[g->level];
		if (now_spawn_type == 2) now_spawn_type = Random_i(0, 1);

		new_enemy->type = now_spawn_type;

		if (new_enemy->type) new_enemy->shotTimer = Random_i(90, 100);
		else new_enemy->shotTimer = 0;

		//spawn position random setting
		float i_x, i_y;
		i_x = Random_f(100, 1000);
		i_y = Random_f(100, 300);

		new_enemy->x = i_x;
		new_enemy->y = i_y;

		em->cnt_enemy++;
		em->spawn_cnt = Random_i(80, 100);
	}
	em->spawn_cnt--;

}


void Update_enemy(GameState* g) { //적 비트맵 변화
	Enemy_manage* enemies = &(g->enemies);
	//printf("cnt_enemy : %d, now_enemy : %d\n", g->enemies.cnt_enemy, g->enemies.now_enemy);


	for (int i = 0; i < enemies->now_enemy; i++)
	{
		Enemy* e = &(enemies->enemy[i]);

		//사망이라면 스킵
		if (e->isAlive == 0) continue;

		Check_bullet_collision(g);

		float player_x = g->player.x;
		float player_y = g->player.y;
		float enemy_x = g->enemies.enemy[i].x;
		float enemy_y = g->enemies.enemy[i].y;

		//printf("%d enemey // x : %.1f, y : %.1f\n", i, e->x, e->y);

		if (!e->spawn)
		{
			if (e->type) {
				//원거리의 경우 제자리에서 총알 발사
				if (e->shotTimer == 0) {
					Enemy_fire_bullets(g, enemy_x, enemy_y, player_x, player_y);

					e->shotTimer = Random_i(100, 200);
				}
				else e->shotTimer--;

			}
			else    Move_enemy(g, i);//근접의 경우 플레이어 위치값을 토대로 움직임
		}
		else {
			Move_enemy(g, i);

			if (e->isJumping == false) e->spawn = false;
		}

	}
}


void Render_enemy(GameState* g) { //적 화면에 표시
	Enemy_manage* enemies = &(g->enemies);

	for (int i = 0; i < enemies->now_enemy; i++)
	{
		if (!enemies->enemy[i].isAlive)
			continue;
		if (!enemies->enemy[i].use)
			continue;


		//al_draw_bitmap(sprites.enemy[em->enemy[i].type], em->enemy[i].x, em->enemy[i].y, 0);
		//그림 데이터 구조체 만들어서 sprites를 바꿀것
	}


}