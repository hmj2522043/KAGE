#pragma once
#include "DxLib.h"
#include "HitBox.h"

class SpecialAttack
{
private:
	int x, y;
	int width, speed;
	int groundY;
	bool isActive;
	int imageHandle;

public:
	void Reset();
	HitBox hitBox;

	SpecialAttack();

	void Start(int startX, int gY);
	void Update();
	void Draw();

	int GetX() const;
	int GetWidth() const;
	int GetHeight() const;
	bool IsActive() const;
};