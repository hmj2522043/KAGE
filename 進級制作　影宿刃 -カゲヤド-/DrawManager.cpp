#include "DrawManager.h"

DrawManager::DrawManager()
{
	groundY = 650;

	// 背景画像の読み込み
	bgHandle = LoadGraph("Resource/BackGround.png");
	if (bgHandle == -1) {
		MessageBox(nullptr, "背景画像の読み込みに失敗しました", "Error", MB_OK);
		ExitProcess(0);
	}
	groundHandle = LoadGraph("Resource/Ground.png");
	if (groundHandle == -1) {
		MessageBox(nullptr, "地面画像の読み込みに失敗しました", "Error", MB_OK);
		ExitProcess(0);
	}
}

void DrawManager::DrawBackGround(int scrollX, int offsetY)
{
	const int screenW = 1280;
	const int screenH = 720;

	int x = scrollX % screenW;

	DrawExtendGraph(
		-x, offsetY,
		screenW - x, screenH + offsetY,
		bgHandle,
		TRUE
	);

	DrawExtendGraph(
		screenW - x, offsetY,
		screenW * 2 - x, screenH + offsetY,
		bgHandle,
		TRUE
	);
}

// 地面描画はそのまま
void DrawManager::DrawGround(int scrollX, int offsetY)
{
	int tileWidth = 200;
	int tileHeight = 120;

	int screenWidth = 1280;
	int numTiles = screenWidth / tileWidth + 2;

	// スクロール位置をタイル幅でループ
	int offset = scrollX % tileWidth;

	for (int i = 0; i < numTiles; i++)
	{
		int x = i * tileWidth - offset;

		DrawExtendGraph(
			x,
			groundY + offsetY,
			x + tileWidth,
			groundY + tileHeight + offsetY,
			groundHandle,
			TRUE
		);
	}
}