#pragma once
#include "DxLib.h"
#include "HitBox.h"
#include "Animation2D.h"

class Boss
{
private:

	int x, y;
	int width, height;

	int hp;
	int maxHp;

	bool isActive;

	HitBox hitBox;

	Animation2D animIdle;

	bool isInvincible;
	int invincibleTimer;
	static const int INVINCIBLE_TIME = 20;
	bool isDying;

public:
	void Reset();
	Boss();

	bool scoreAdded;

	void Spawn(int screenWidth, int groundY);

	void Update();
	void Draw(int = 0, int offsetY = 0);

	void TakeDamage(int damage);

	bool IsActive() const { return isActive; }
	bool IsDying() const { return isDying; }

	HitBox& GetHitBox() { return hitBox; }

	int GetHP() const { return hp; }
	int GetMaxHP() const { return maxHp; }
};