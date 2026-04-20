#pragma once
#include "DxLib.h"

class HitBox
{
private:
	int x, y;
	int width, height;

public:

	HitBox() : x(0), y(0), width(0), height(0) {}

	HitBox(int startX, int startY, int w, int h);

	void SetPos(int newX, int newY);
	void SetSize(int w, int h);
	void SetSizeBottomOrigin(int w, int h);

	int GetX() const;
	int GetY() const;
	int GetWidth() const;
	int GetHeight() const;

	// 当たり判定
	bool CheckCollision(const HitBox& other) const;

	// デバッグ描画
	void Draw() const;
};