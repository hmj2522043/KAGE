#include "GameMain.h"

GameMain::GameMain() : player(200, 0, drawManager.GetGroundY())
{
	enemies.resize(999);

	for (int i = 0; i < maxEnemies; i++)
	{
		enemies[i].Spawn(1280 + GetRand(400), drawManager.GetGroundY(), distance);
	}

	soundAirSlash = LoadSoundMem("Resource/Sound/AirSlash.wav");
	soundSlash = LoadSoundMem("Resource/Sound/Slash.wav");
	soundSweet = LoadSoundMem("Resource/Sound/Sweet.wav");
	soundTouch = LoadSoundMem("Resource/Sound/Touch.wav");
	soundSpecial = LoadSoundMem("Resource/Sound/Special.wav");
	bgmHandle = LoadSoundMem("Resource/Sound/BGM.wav");

	titleImage = LoadGraph("Resource/Title.png");
	resultImage = LoadGraph("Resource/Result.png");
}

void GameMain::Update()
{
	switch (state)
	{
	case GameState::TITLE:
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			state = GameState::PLAY;
			ResetGame();

			PlaySoundMem(bgmHandle, DX_PLAYTYPE_LOOP);
		}
		break;

	case GameState::PLAY:
		UpdateGamePlay();
		if (isGameOver)
		{
			finalScore = score
				+ distance * 100
				+ maxCombo * 500;

			if (finalScore > highScore)
			{
				highScore = finalScore;
			}

			StopSoundMem(bgmHandle);
			state = GameState::RESULT;
		}
		break;

	case GameState::RESULT:
		if (CheckHitKey(KEY_INPUT_X))
		{
			StopSoundMem(bgmHandle); // ←念のため追加
			state = GameState::TITLE;
		}
		break;
	}
}

void GameMain::Draw()
{
	switch (state)
	{
	case GameState::TITLE:
		if (titleImage != -1) DrawGraph(0, 0, titleImage, TRUE);
		break;

	case GameState::PLAY:
		DrawGamePlay();
		break;

	case GameState::RESULT:
		if (resultImage != -1) DrawGraph(0, 0, resultImage, TRUE);

		DrawFormatString(600, 350, GetColor(255, 255, 255),
			"SCORE : %d", finalScore);

		DrawFormatString(590, 450, GetColor(255, 255, 0),
			"HI SCORE : %d", highScore);

		break;
	}
}

void GameMain::UpdateGamePlay()
{
	scrollManager.Update();
	player.Update();
	boss.Update();

	if (wasAttacking && !player.IsAttacking())
	{
		if (!attackHit)
		{
			PlaySoundMem(soundAirSlash, DX_PLAYTYPE_BACK);
		}

		attackHit = false;
	}

	if (distance >= nextIncreaseDist)
	{
		maxEnemies++;
		nextIncreaseDist += 30;
	}

	wasAttacking = player.IsAttacking();

	if (CheckHitKey(KEY_INPUT_C))
	{
		if (!special.IsActive() && player.CanUseSpecial())
		{
			PlaySoundMem(soundSpecial, DX_PLAYTYPE_BACK);
			special.Start(player.GetX() + player.GetWidth(), drawManager.GetGroundY());

			player.UseSpecial();
		}
	}
	special.Update();

	if (CheckHitKey(KEY_INPUT_UP))		//debug
	{
		distance += 10;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))		//debug
	{
		player.AddSpecialGauge(100);
	}

	if (distance >= nextBossDist)
	{
		boss.Spawn(1280, drawManager.GetGroundY()); // 画面右外から出現
		nextBossDist += 90;
	}

	if (isGameOver) return;
	for (int i = 0; i < maxEnemies; i++)
	{
		auto& enemy = enemies[i];
		enemy.Update();


		if (player.GetHP() <= 0)
		{
			isGameOver = true;
		}

		if (enemy.IsDying() && !enemy.scoreAdded)
		{
			AddScore(100 + killCombo * 5);
			enemy.scoreAdded = true;
		}

		if (boss.IsDying() && !boss.scoreAdded)
		{
			AddScore(1000 + distance * 10);
			boss.scoreAdded = true;
		}

		// プレイヤー被弾
		if (player.GetHitBox().CheckCollision(enemy.GetHitBox()))
		{
			if (player.OnHit())
			{
				PlaySoundMem(soundTouch, DX_PLAYTYPE_BACK);
				killCombo = 0;
				comboTimer = 0;

				shakeTimer = 10;
				shakeMagnitude = 8;
			}
		}

		// 攻撃判定
		if (player.IsAttacking() && enemy.IsActive() && !enemy.IsDying())
		{
			int cx = player.GetAttackCenterX();
			int cy = player.GetAttackCenterY();
			int radius = player.GetAttackingRadius();
			int sweetRadius = player.GetSweetRadius();

			HitBox& eBox = enemy.GetHitBox();

			int closestX = max(eBox.GetX(), min(cx, eBox.GetX() + eBox.GetWidth()));
			int closestY = max(eBox.GetY(), min(cy, eBox.GetY() + eBox.GetHeight()));

			int dx = cx - closestX;
			int dy = cy - closestY;

			if (dx * dx + dy * dy <= radius * radius)
			{
				bool sweet = dx * dx + dy * dy <= sweetRadius * sweetRadius;
				enemy.TakeDamage(10);

				if (enemy.IsDying() && sweet) {
					player.AddSpecialGauge(5);
				}

				attackHit = true;

				killCombo++;
				if (killCombo > maxCombo)
				{
					maxCombo = killCombo;
				}
				comboTimer = COMBO_TIME_LIMIT;
				shakeTimer = 5;
				shakeMagnitude = 2;

				if (sweet) {
					PlaySoundMem(soundSweet, DX_PLAYTYPE_BACK);
				}
				else {
					PlaySoundMem(soundSlash, DX_PLAYTYPE_BACK);
				}
			}
		}

		if (special.IsActive() && enemy.IsActive() && !enemy.IsDying())//必殺Enemy判定
		{
			if (special.hitBox.CheckCollision(enemy.GetHitBox()))
			{
				enemy.TakeDamage(1);
			}
		}
		if (special.IsActive() && boss.IsActive())//必殺Boss判定
		{
			if (special.hitBox.CheckCollision(boss.GetHitBox()))
			{
				boss.TakeDamage(50);
			}
		}

		// 画面外に出たら再スポーン
		if (!enemy.IsActive())
		{
			enemy.Spawn(1280 + GetRand(500), drawManager.GetGroundY(), distance);
		}
	}

	// コンボタイマー
	if (comboTimer > 0)
	{
		comboTimer--;
	}
	else
	{
		killCombo = 0;
	}

	frameCount++;

	if (frameCount >= 60)
	{
		frameCount = 0;
		distance += 1; // 1m進む
	}
}

void GameMain::DrawGamePlay()
{
	int offsetX = 0;
	int offsetY = 0;

	if (shakeTimer > 0)
	{
		offsetX = GetRand(shakeMagnitude * 2) - shakeMagnitude;
		offsetY = GetRand(shakeMagnitude * 2) - shakeMagnitude;
		shakeTimer--;
	}


		// スクロール取得
		int scroll = scrollManager.GetScroll();
		drawManager.DrawBackGround((scroll / 4) + offsetX, offsetY);
		drawManager.DrawGround(scroll + offsetX, offsetY);
		
		player.Draw(offsetX, offsetY);
		boss.Draw(offsetX, offsetY);
		special.Draw();

		// 敵描画
		for (int i = 0; i < maxEnemies; i++)
		{
			enemies[i].Draw(offsetX, offsetY);
		}

		// コンボ表示
		if (killCombo > 1)
		{
			DrawFormatString(
				50,
				40,
				GetColor(255, 30, 30),
				"%d斬!!",
				killCombo
			);
			DrawFormatString(
				49,
				39,
				GetColor(10, 10, 10),
				"%d斬!!",
				killCombo
			);
		}

		// プレイヤーHPバー
		int hp = player.GetHP();
		int maxHp = player.GetMaxHP();

		int barX = 30;
		int barY = 660;
		int barWidth = 250;
		int barHeight = 20;

		// 背景
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(80, 80, 80), TRUE);

		// HP割合
		int hpWidth = barWidth * hp / maxHp;

		// HP色変化
		int color;

		if (hp > maxHp * 0.5)
		{
			color = GetColor(0, 255, 0); // 緑
		}
		else if (hp > maxHp * 0.2)
		{
			color = GetColor(255, 255, 0); // 黄色
		}
		else
		{
			color = GetColor(255, 0, 0); // 赤
		}

		// HP描画
		DrawBox(barX, barY, barX + hpWidth, barY + barHeight, color, TRUE);

		// 枠
		DrawString(barX - 20, barY + 2, "HP", GetColor(255, 255, 255));
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 255, 255), FALSE);

		// 数値表示
		DrawFormatString(
			barX + barWidth + 10,
			barY + 2,
			GetColor(255, 255, 255),
			"%d / %d",
			hp,
			maxHp
		);

		if (boss.IsActive()) //ボスHPバー
		{
			int hp = boss.GetHP();
			int maxHp = boss.GetMaxHP();
			int barX = 660;
			int barY = 660;

			int barWidth = 600;

			int hpWidth = barWidth * hp / maxHp;

			DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(80, 80, 80), TRUE);
			DrawBox(barX, barY, barX + hpWidth, barY + barHeight, GetColor(255, 50, 50), TRUE);
		}

		DrawFormatString(		//スコア
			1050,
			30,
			GetColor(255, 255, 255),
			"SCORE : %06d",
			score
		);

		DrawFormatString(1050, 60, GetColor(255, 255, 255),
			"DIST : %dm", distance);
}

void GameMain::AddScore(int value)
{
	score += value;
}

void GameMain::ResetGame()
{
	// スコア・距離・コンボなど初期化
	score = 0;
	distance = 0;
	killCombo = 0;
	comboTimer = 0;
	isGameOver = false;
	isResult = false;

	// プレイヤー初期化
	player.Reset(200, drawManager.GetGroundY());

	// ボス初期化
	boss.Reset();

	// 必殺技リセット
	special.Reset();

	// 敵リセット
	for (auto& enemy : enemies)
	{
		enemy.Reset();
	}

	// フレームカウント初期化
	frameCount = 0;
	maxEnemies = 5;
	nextIncreaseDist = 30;
	nextBossDist = 90;
}