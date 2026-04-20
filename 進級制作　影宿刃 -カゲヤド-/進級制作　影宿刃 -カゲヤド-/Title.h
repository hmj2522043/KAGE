#pragma once
#include "DxLib.h"

class TitleScene
{
private:
	int backgroundImage = -1; // 背景画像ハンドル

public:
	void LoadBackground(const char* path)
	{
		backgroundImage = LoadGraph(path);
	}

	void Update(bool& startGame)
	{
		if (CheckHitKey(KEY_INPUT_SPACE)) // スペースでゲーム開始
		{
			startGame = true;
		}
	}

	void Draw()
	{
		if (backgroundImage != -1)
		{
			DrawGraph(0, 0, backgroundImage, TRUE);
		}
	}
};