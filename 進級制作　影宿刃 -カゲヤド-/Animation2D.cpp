#include "Animation2D.h"
#include "DxLib.h"

// コンストラクタ
Animation2D::Animation2D(const char* path, int frameCount, int frameDelay, bool loop)
	: frameCount(frameCount), frameDelay(frameDelay), loop(loop),
	currentFrame(0), frameTimer(0)
{
	imageHandle = LoadGraph(path); // PNGなら透過も読み込まれる
	if (imageHandle == -1) imageHandle = 0;
}

// デストラクタ
Animation2D::~Animation2D()
{
	if (imageHandle != 0) DeleteGraph(imageHandle);
}

// フレーム更新
void Animation2D::Update()
{
	if (frameCount <= 1) return;

	frameTimer++;
	if (frameTimer >= frameDelay)
	{
		frameTimer = 0;
		currentFrame++;
		if (currentFrame >= frameCount)
		{
			if (loop) currentFrame = 0;
			else currentFrame = frameCount/* - 1*/;
		}
	}
}

// サイズ指定描画（透過対応）
void Animation2D::Draw(int x, int y, int drawWidth, int drawHeight)
{
	if (imageHandle == 0) return;

	int imgWidth, imgHeight;
	GetGraphSize(imageHandle, &imgWidth, &imgHeight);

	int frameWidth = imgWidth / frameCount;
	int frameHeight = imgHeight;

	if (currentFrame >= frameCount) return;

	// ブレンドモードON（透過反映）
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	// 1フレームだけ切り出して拡大描画
	DrawRectExtendGraph(
		x, y,                      // 描画先 左上
		x + drawWidth, y + drawHeight,  // 描画先 右下
		frameWidth * currentFrame, 0,   // 元画像 左上
		frameWidth, frameHeight,         // 元画像 幅・高さ
		imageHandle,
		TRUE // α有効
	);

	// ブレンドモード戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// リセット
void Animation2D::Reset()
{
	currentFrame = 0;
	frameTimer = 0;
}