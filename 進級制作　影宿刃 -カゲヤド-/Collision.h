#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "SpecialAttack.h"

class Collision
{
public:

	bool CheckPlayerHitEnemy
	(
		Player& player,
		Enemy& enemy
	);

	bool CheckPlayerAttackEnemy
	(
		Player& player,
		Enemy& enemy
	);

	bool CheckSpecialEnemy
	(
		SpecialAttack& special,
		Enemy& enemy
	);

	bool CheckSpecialBoss
	(
		SpecialAttack& special,
		Boss& boss
	);
};