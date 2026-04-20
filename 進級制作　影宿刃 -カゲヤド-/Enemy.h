#pragma once
#include "DxLib.h"
#include "HitBox.h"
#include "Animation2D.h"

class Enemy
{
private:
	int x, y;
	int width, height;
	int speed;
	int hp;
	int maxHp;
	HitBox hitbox;
	bool isActive;

	Animation2D animEnemy;
	int deadImage[2];
	int deadType;
	int deathTimer;
	bool isDying;
public:
	void Reset();
	Enemy();

	bool scoreAdded;

	void Spawn(int screenwidth,int groundY,int distance);
	void Update();
	void Draw(int offsetX = 0, int offsetY = 0);
	void TakeDamage(int damage);

	void Kill();

	int GetX() const { return hitbox.GetX(); }
	int GetY() const { return hitbox.GetY(); }
	int GetWidth() const { return hitbox.GetWidth(); }
	int GetHeight() const { return hitbox.GetHeight(); }


	bool IsActive() const { return isActive; }
	bool IsDying() const { return isDying; }
	HitBox& GetHitBox() { return hitbox; }
};