#include "HitBox.h"

// コンストラクタ
HitBox::HitBox(int startX, int startY, int w, int h)
	: x(startX), y(startY), width(w), height(h)
{
}

// 座標更新
void HitBox::SetPos(int newX, int newY)
{
	x = newX;
	y = newY;
}

// サイズ更新（左上基準）
void HitBox::SetSize(int w, int h)
{
	width = w;
	height = h;
}

// サイズ更新（足元固定で頭だけ縮める）
void HitBox::SetSizeBottomOrigin(int w, int h)
{
	y += (height - h); // 元の高さとの差分だけYを上にずらす
	width = w;
	height = h;
}

// Getter
int HitBox::GetX() const { return x; }
int HitBox::GetY() const { return y; }
int HitBox::GetWidth() const { return width; }
int HitBox::GetHeight() const { return height; }

// 当たり判定
bool HitBox::CheckCollision(const HitBox& other) const
{
	return !(x + width < other.x || x > other.x + other.width ||
		y + height < other.y || y > other.y + other.height);
}

// デバッグ描画
void HitBox::Draw() const
{
	DrawBox(x, y, x + width, y + height, GetColor(255, 0, 0), false); // FALSE → false
}