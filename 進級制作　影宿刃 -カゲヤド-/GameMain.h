#pragma once
#include "Title.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include <vector>
#include "ScrollManager.h"
#include "DrawManager.h"
#include "SpecialAttack.h"
#include "Result.h"

enum class GameState
{
	TITLE,
	PLAY,
	RESULT
};

class GameMain
{
private:
	ScrollManager scrollManager;
	DrawManager drawManager;
	Player player;
	Boss boss;
	SpecialAttack special;
	std::vector<Enemy> enemies;

	int titleImage = -1;
	int resultImage = -1;

	GameState state = GameState::TITLE;
	bool startGame = false;
	bool isResult = false;

	//ÉRÉìÉ{ÅAóPó\
	int killCombo = 0;
	int comboTimer = 0;
	static const int COMBO_TIME_LIMIT = 180;

	//âÊñ êUìÆ
	int shakeTimer = 0;
	int shakeMagnitude = 2;

	//âÔêSéaÇË
	bool isCritical = false;
	int criticalTimer = 0;
	static const int CRITICAL_DURATION = 60;

	int soundAirSlash;
	int soundSlash;
	int soundSweet;
	int soundTouch;
	int soundSpecial;

	int bgmHandle;

	bool attackHit = false;
	bool wasAttacking = false;
	bool isGameOver = false;

	int score = 0;

	int frameCount = 0;
	int distance = 0;

	int maxEnemies = 5;     // èoåªè„å¿
	int nextIncreaseDist = 20;

	int nextBossDist = 90;

	int highScore = 0;
	int finalScore = 0;
	int maxCombo = 0;

	void ResetGame();
	void UpdateGamePlay();
	void DrawGamePlay();
	
public:
	GameMain();
	void Init();
	void AddScore(int value);
	void Update();
	void Draw();
};