#pragma once
#include "DxLib.h"
#include "HitBox.h"
#include "Animation2D.h"
#include "LimitGauge.h"

enum PlayerState
{
	RUNNING,
	JUMP1,
	JUMP2,
	SLIDING,
	ATTACK_GROUND,
	ATTACK_AIR,
	DIVE
};

class Player
{
private:
	int x, y;           // 座標
	int width, height;  // サイズ
	int yVelocity;      // 上下速度
	int jumpCount;      // ジャンプ回数
	int groundY;        // 地面の高さ
	PlayerState state;
	Gauge specialGauge;

	HitBox hitBox;      // 当たり判定

	char key[256];
	char oldKey[256];	//２段ジャンプのやつ

	bool isAttacking;
	int attackTimer;
	int attackCoolTime;
	int attackRadius;		//攻撃関連
	int sweetRadius;

	bool isHit = false;
	int hitTimer = 0;
	static const int HIT_DURATION = 60;

	int hp = maxHp;
	static const int maxHp = 100;

	Animation2D animRun;
	Animation2D animJump;
	Animation2D animSlide;
	Animation2D animSlashGround;
	Animation2D animSlashAir;
	Animation2D animDive;

public:
	Player(int startX, int startY, int ground);

	void Update();
	void Draw(int offsetX = 0, int offsetY = 0);
	void Jump();
	void Attack();
	void StartSlide();
	void EndSlide();
	void Dive();

	void AddSpecialGauge(int value);
	bool CanUseSpecial() const;
	void UseSpecial();
	void Reset(int startX, int groundY);

	int GetX() const;
	int GetWidth() const;
	int GetSweetCenterX() const;
	int GetSweetCenterY() const;

	HitBox& GetHitBox();

	bool OnHit();

	int GetHP() const { return hp; }
	int GetMaxHP() const { return maxHp; }

	bool IsAttacking() const { return isAttacking; }
	int GetAttackingRadius() const { return attackRadius; }
	int GetCenterX() const { return x + width / 2; }
	int GetCenterY() const { return y + height / 2; }
	int GetSweetRadius() const { return sweetRadius; }

	// 攻撃判定用の中心（プレイヤー前方にずらす）
	int GetAttackCenterX() const { return x + width / 2 + 30; }
	int GetAttackCenterY() const { return y + height / 2; }

	bool IsHit() const;

	bool IsAttackStart() const { return isAttacking && attackTimer == 10; }

};