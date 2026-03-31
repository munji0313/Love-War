#include "boss.h"
#include "game.h"
#include "player.h"
#include "bullet.h"

/*-------------------보조 함수-----------------------*/


int boss_p[10] = { 10,10,10,20,20,100,100,75,75,75 };

void Boss_pattern(GameState* g) {
	Boss* boss = &(g->boss);
	float player_x = g->player.x;
	float player_y = g->player.y;
	float enemy_x = boss->x;
	float enemy_y = boss->y;


	Enemy_fire_bullets(g, enemy_x, enemy_y, player_x, player_y);
	if (boss->pattern % 5 == 0) {
		Create_enemy(g);
	}

	boss->shotTimer = boss_p[(boss->pattern++) % 10];
	if (boss->hp == 1 || boss->shotTimer > 0) {
		boss->shotTimer--;
	}
}



/*-------------------주요 함수-----------------------*/


void Init_boss(GameState* g) {

	Boss* boss = &(g->boss);

	boss->x = 1030;
	boss->y = 350;
	boss->hp = BOSS_HP;
	boss->isAlive = false;
	boss->shotTimer = 100;
	boss->pattern = 0;
	boss->invincibleTime = 0;


}

void Update_boss(GameState* g) {
	Boss* boss = &(g->boss);
	if (g->level == BOSS) {

		if (boss->isAlive == 1)
		{
			if (boss->shotTimer == 0) {

				Boss_pattern(g);
			}
			else boss->shotTimer--;

			if (boss->invincibleTime > 0) {
				boss->invincibleTime--;	//무적시간이 존재하면 시간 감소
			}
			//else if (Check_bullet_collision(g)) { //무적이 없을 시 총알 충돌 return. 맞았다면 무적시간.
			//	boss->invincibleTime = 3;
			//	boss->hp--;
			//}


			if (boss->hp == 0) {
				boss->isAlive = 0;
			}
		}
	}
}

