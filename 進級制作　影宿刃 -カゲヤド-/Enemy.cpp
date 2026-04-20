#include"Enemy.h"
#include"GameMain.h"

Enemy::Enemy() : x(0), y(0), width(40), height(40), speed(8), hitbox(0, 0, 40, 40), isActive(false), animEnemy("Resource/Enemy1.png", 4, 8)
{
	maxHp = 3;
	hp = maxHp;
	scoreAdded = false;

	deadImage[0] = LoadGraph("Resource/Enemy_D1.png");
	deadImage[1] = LoadGraph("Resource/Enemy_D2.png");
}

void Enemy::Spawn(int screenWidth, int groundY,int distance)
{
	x = screenWidth + 50;
	speed = GetRand(3) + 5 + (distance / 100);

	int minY = 0;
	int maxY = groundY - height;
	y = GetRand(maxY - minY + 1) + minY;

	isActive = true;

	hp = maxHp;

	hitbox.SetSize(width, height);
	hitbox.SetPos(x, y);

	isDying = false;
	deathTimer = 0;
}

void Enemy::Update()
{
	if (!isActive)return;

	if (isDying)
	{
		deathTimer--;

		if (deathTimer <= 0)
		{
			isDying = false;
			isActive = false;
		}

		return;
	}

	x -= speed;

	hitbox.SetPos(x, y);

	animEnemy.Update();

	if (x + width < 0)
		isActive = false;
}

void Enemy::Draw(int offsetX, int offsetY)
{
	if (!isActive)return;

	if (isDying)
	{
		DrawExtendGraph(
			x + offsetX, y + offsetY,
			x + width + offsetX, y + height + offsetY,
			deadImage[deadType],
			TRUE
		);
	}
	else
	{
		animEnemy.Draw(x + offsetX, y + offsetY, width, height);
	}

#if DEBUG
	hitbox.Draw();
#endif
}

void Enemy::TakeDamage(int damage)
{
	if (isDying) return;

	hp -= damage;

	if (hp <= 0)
	{
		Kill();
	}
}

void Enemy::Kill()
{
	isDying = true;
	deathTimer = 30;

	deadType = GetRand(1);

	hitbox.SetSize(0, 0);
	hitbox.SetPos(-1000, -1000);

	scoreAdded = false;
}

void Enemy::Reset() {
	isActive = false;
	scoreAdded = false;
}