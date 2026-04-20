#pragma once
#include "DxLib.h"

enum class Anime
{
	Idle,
	Run,
	Shot,
	Length,
};

class Animation2D
{
private:
	int imageHandle;      // 画像ハンドル
	int frameCount;       // フレーム数
	int frameDelay;       // フレーム切り替え待機フレーム
	bool loop;            // ループするか
	int currentFrame;     // 現在のフレーム
	int frameTimer;       // フレーム更新タイマー

public:
	Animation2D(const char* path, int frameCount, int frameDelay, bool loop = true);
	~Animation2D();

	void Update();

	// サイズ指定可能な描画
	void Draw(int x, int y, int drawWidth, int drawHeight);

	void Reset();

	int GetFrameCount() const { return frameCount; }
	int GetFrameDelay() const { return frameDelay; }
};