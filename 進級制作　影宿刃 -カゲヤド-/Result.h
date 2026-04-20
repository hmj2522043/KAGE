#pragma once
#include "DxLib.h"

class ResultScreen
{
private:
    int backgroundImage = -1; // 背景画像ハンドル
	int score = 0;
	int combo = 0;
	int distance = 0;

public:
    void LoadBackground(const char* path)
    {
        backgroundImage = LoadGraph(path);
    }

	void SetData(int s, int c, int d)
	{
		score = s;
		combo = c;
		distance = d;
	}

    int Update()
    {
        if (CheckHitKey(KEY_INPUT_SPACE)) // スペースでタイトルに戻る
        {
            return 1;
        }
        return 0;
    }

    void Draw()
    {
		ClearDrawScreen();
        if (backgroundImage != -1)
        {
            DrawGraph(0, 0, backgroundImage, TRUE);
        }
    }
};
