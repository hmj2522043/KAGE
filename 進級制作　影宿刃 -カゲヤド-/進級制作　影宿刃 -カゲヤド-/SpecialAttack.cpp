#include "SpecialAttack.h"

SpecialAttack::SpecialAttack()
{
	x = 0;
	width = 80;
	speed = 20;
	isActive = false;
	groundY = 720;

	hitBox = HitBox(0, 0, width, groundY);

	imageHandle = LoadGraph("Resource/Special.png");
	if (imageHandle == -1) imageHandle = 0;
}

void SpecialAttack::Start(int startX, int gY)
{
	x = startX;
	groundY = gY;
	isActive = true;

	hitBox.SetPos(x, 0);
	hitBox.SetSize(width, groundY);
}

void SpecialAttack::Update()
{
	if (!isActive) return;

	x += speed;
	hitBox.SetPos(x, 0);
	if (x > 1280) isActive = false;
}

void SpecialAttack::Draw()
{
	if (!isActive) return;
	if (imageHandle != 0)
	{
		// 画像描画：赤四角と同じサイズに拡大
		DrawExtendGraph(x, 0, x + width, groundY, imageHandle, TRUE);
	}
	else
	{
		// 画像がない場合は赤四角で代用
		DrawBox(x, 0, x + width, groundY, GetColor(255, 0, 0), TRUE);
	}
}

int SpecialAttack::GetX() const { return x; }
int SpecialAttack::GetWidth() const { return width; }
int SpecialAttack::GetHeight() const { return groundY; }
bool SpecialAttack::IsActive() const { return isActive; }

void SpecialAttack::Reset() {
	isActive = false;
}