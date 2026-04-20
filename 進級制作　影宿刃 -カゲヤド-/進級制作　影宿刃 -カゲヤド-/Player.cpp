#include "Player.h"
#include "DxLib.h"
#include "LimitGauge.h"

Player::Player(int startX, int startY, int ground)
	: hitBox(startX - 10, startY, 40, 50),animRun("Resource/Player_run.png",6,5), animJump("Resource/Player_jump.png", 1, 1, false), animSlide("Resource/Player_sliding.png", 1, 1, false),animSlashGround("Resource/Player_Slash.png", 1,1, false), animSlashAir("Resource/Player_JumpSlash.png", 1,1, false), animDive("Resource/Player_Dive.png", 1, 1, false)
{
	x = startX;
	y = startY;
	width = 40;
	height = 50;
	yVelocity = 0;
	jumpCount = 0;
	groundY = ground;
	state = RUNNING;
	hp = maxHp;

	isAttacking = false;
	attackTimer = 0;
	attackCoolTime = 0;
	attackRadius = 60;
	sweetRadius = 25;
}

// 毎フレーム更新
void Player::Update()
{
	GetHitKeyStateAll(key);

	// 重力
	yVelocity += 2;
	y += yVelocity;

	bool onGround = false;

	if (CheckHitKey(KEY_INPUT_DOWN))		//debug
	{
		hp -= 100;
	}
	// 地面判定
	if (y + height >= groundY)
	{
		y = groundY - height;
		yVelocity = 0;
		jumpCount = 0;
		onGround = true;

		if (state != SLIDING)
			state = RUNNING;
	}

	if (state == RUNNING && onGround && key[KEY_INPUT_X])
	{
		StartSlide();
	}

	// スライド終了
	if (state == SLIDING && !key[KEY_INPUT_X])
	{
		EndSlide();
	}

	// 空中で急降下
	if ((state == JUMP1 || state == JUMP2) && key[KEY_INPUT_X])
	{
		Dive();
	}

	// スライド中以外
	if (state != SLIDING)
	{
		// ジャンプ（2段）
		if (key[KEY_INPUT_SPACE] && !oldKey[KEY_INPUT_SPACE] && jumpCount < 2)
		{
			Jump();
		}

		// 攻撃
		if (key[KEY_INPUT_Z] && !oldKey[KEY_INPUT_Z])
		{
			Attack();
		}
	}

	if (isAttacking)
	{
		attackTimer--;

		if (attackTimer <= 0)
		{
			isAttacking = false;
			state = RUNNING;
		}
	}

	if (attackCoolTime > 0)
	{
		attackCoolTime--;
	}

	if (isHit) //被弾管理
	{
		hitTimer--;
		if (hitTimer <= 0)
		{
			isHit = false;
		}
	}

	// HitBox位置更新
	hitBox.SetPos(x - 5, y);

	if (state == RUNNING)  // 走っているときだけ更新
		animRun.Update();
	else if (state == JUMP1 || state == JUMP2)
		animJump.Update();
	else if (state == SLIDING)
		animSlide.Update();

	if (state == ATTACK_GROUND)
		animSlashGround.Update();
	else if (state == ATTACK_AIR)
		animSlashAir.Update();
	if (state == DIVE)
		animDive.Update();

	specialGauge.Update();

	memcpy(oldKey, key, sizeof(key));
}

// 描画
void Player::Draw(int offsetX, int offsetY)
{
	bool drawPlayer = true;
	if (isHit)
	{
		// 3フレームごとに表示/非表示
		drawPlayer = (hitTimer / 3) % 2 == 0;
	}

	specialGauge.Draw(x + width / 2 - 25, y - 20, 50, 8);

	if (drawPlayer)
	{
		if (state == RUNNING)
		{
			animRun.Draw(x + offsetX, y + offsetY, width, height);
		}
		else if (state == SLIDING)
		{
			animSlide.Draw(x + offsetX, y + offsetY, width, height);
		}
		else if (state == JUMP1 || state == JUMP2)
		{
			animJump.Draw(x + offsetX, y + offsetY, width, height);
		}
		else if (state == DIVE)
		{
			animDive.Draw(x + offsetX, y + offsetY, width, height);
		}
		else if (state == ATTACK_GROUND)
		{
			animSlashGround.Draw(x + offsetX, y + offsetY, width, height);
		}
		else if (state == ATTACK_AIR)
		{
			animSlashAir.Draw(x + offsetX, y + offsetY, width, height);
		}
		else
		{
			// スライドや攻撃中は今まで通り四角
			DrawBox(x, y, x + width, y + height, GetColor(255, 100, 100), TRUE);
		}
	}

#if DEBUG
	hitbox.Draw();
#endif
	int sweetX = GetSweetCenterX();
	int sweetY = GetSweetCenterY();

#if DEBUG
	if (isAttacking)
	{
		int centerX = GetAttackCenterX();
		int centerY = GetAttackCenterY();
		DrawCircle(centerX, centerY, attackRadius, GetColor(0, 200, 255), FALSE);
		DrawCircle(sweetX, sweetY, sweetRadius, GetColor(255, 200, 0), FALSE);
	}
#endif
}

// ジャンプ
void Player::Jump()
{
	yVelocity = -35;
	jumpCount++;
	state = (jumpCount == 1) ? JUMP1 : JUMP2;
}

// スライディング
void Player::StartSlide()
{
	state = SLIDING;

	int slideHeight = 20;

	// 足元固定
	y += height - slideHeight;
	height = slideHeight;

	hitBox.SetSize(width, height);
}

void Player::EndSlide()
{
	state = RUNNING;

	int normalHeight = 50;

	// 足元固定で戻す
	y -= normalHeight - height;

	height = normalHeight;

	hitBox.SetSize(width, height);
}

void Player::Dive()
{
	if (state == JUMP1 || state == JUMP2)
	{
		state = DIVE;
		yVelocity = 100; // 強制落下
		animDive.Reset();
	}
}

// 攻撃
void Player::Attack()
{
	if (isAttacking || attackCoolTime > 0) return;

	isAttacking = true;

	if (y + height >= groundY)
		state = ATTACK_GROUND;
	else
		state = ATTACK_AIR;

	attackTimer = 10;
	attackCoolTime = 17;

	animSlashGround.Reset();
	animSlashAir.Reset();
}

bool Player::OnHit()
{
	if (isHit) return false; // 無敵中は何もしない

	isHit = true;
	hitTimer = HIT_DURATION;

	hp -= 10;
	if (hp < 0) hp = 0;

	return true; // 被弾処理が行われた
}

HitBox& Player::GetHitBox()
{
	return hitBox;
}

bool Player::IsHit() const
{
	return isHit;
}


int Player::GetX() const
{
	return x;
}

int Player::GetWidth() const
{
	return width;
}		//攻撃判定

int Player::GetSweetCenterX() const
{
	return x + width / 2 + 40;
}

int Player::GetSweetCenterY() const
{
	return y + height / 2;
}		//会心判定

void Player::AddSpecialGauge(int value)
{
	specialGauge.Add(value);
}

bool Player::CanUseSpecial() const
{
	return specialGauge.IsFull();
}

void Player::UseSpecial()
{
	specialGauge.Reset();
}

void Player::Reset(int startX, int groundY)
{
	x = startX;
	y = 0;
	width = 40;
	height = 50;
	yVelocity = 0;
	jumpCount = 0;
	groundY = groundY;
	state = RUNNING;
	hp = maxHp;

	isAttacking = false;
	attackTimer = 0;
	attackCoolTime = 0;

	hitBox.SetPos(x - 10, y);
	hitBox.SetSize(width, height);

	animRun.Reset();
	animJump.Reset();
	animSlide.Reset();
	animSlashGround.Reset();
	animSlashAir.Reset();
	animDive.Reset();

	isHit = false;
	hitTimer = 0;

	specialGauge.Reset();
}