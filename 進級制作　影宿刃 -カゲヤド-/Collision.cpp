#include "Collision.h"

bool Collision::CheckPlayerHitEnemy
(
	Player& player,
	Enemy& enemy
)

{
	if (!enemy.IsActive())
	{
		return false;
	}

	return player.GetHitBox().CheckCollision
	(
		enemy.GetHitBox()
	);
}

bool Collision::CheckSpecialEnemy
(
	SpecialAttack& special,
	Enemy& enemy
)

{
	if (!special.IsActive())
	{
		return false;
	}

	if (!enemy.IsActive())
	{
		return false;
	}

	if (enemy.IsDying())
	{
		return false;
	}

	if (special.hitBox.CheckCollision(enemy.GetHitBox()))
	{
		enemy.TakeDamage(1);
		return true;
	}

	return false;
}

bool Collision::CheckSpecialBoss
(
	SpecialAttack& special,
	Boss& boss
)

{
	if (!special.IsActive())
	{
		return false;
	}

	if (!boss.IsActive())
	{
		return false;
	}

	if (special.hitBox.CheckCollision(boss.GetHitBox()))
	{
		boss.TakeDamage(50);
		return true;
	}

	return false;
}