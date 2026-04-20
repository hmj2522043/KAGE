#include "Boss.h"
#include "DxLib.h"

Boss::Boss()
	: x(0), y(0), width(200), height(200),
	hp(100), maxHp(100),
	isActive(false),
	hitBox(0, 0, 100, 100),
	animIdle("Resource/BossGreed.png", 6, 8)
{
	scoreAdded = false;
	isInvincible = false;
	invincibleTimer = 0;
}

void Boss::Spawn(int screenWidth, int groundY)
{
	x = screenWidth;
	y = groundY - height - 100;

	hp = maxHp;
	isActive = true;

	hitBox.SetPos(x, y);
	hitBox.SetSize(width, height);
	isDying = false;
}

void Boss::Update()
{
	if (!isActive) return;

	// 左に移動して画面に入ってくる
	int targetX = 1080; // 最終停止位置（例）
	if (x > targetX)
	{
		x -= 5;
		if (x < targetX) x = targetX;
	}

	animIdle.Update();

	if (isInvincible)
	{
		invincibleTimer--;
		if (invincibleTimer <= 0)
			isInvincible = false;
	}

	hitBox.SetPos(x, y);
}

void Boss::Draw(int offsetX, int offsetY)
{
	if (!isActive) return;

	animIdle.Draw(x + offsetX, y + offsetY, width, height);
#if DEBUG
	hitbox.Draw();
#endif
}

void Boss::TakeDamage(int damage)
{
	if (isInvincible || !isActive) return;

	hp -= damage;

	isInvincible = true;
	invincibleTimer = INVINCIBLE_TIME;

	if (hp <= 0)
	{
		isActive = false;
	}
}

void Boss::Reset() {
	hp = maxHp;
	isActive = false;
}