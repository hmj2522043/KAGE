#pragma once
#include "DxLib.h"

class DrawManager
{
private:
	int groundY;
	int bgHandle;
	int groundHandle;

public:
	DrawManager();

	void DrawBackGround(int offsetX = 0, int offsetY = 0);
	void DrawGround(int offsetX = 0, int offsetY = 0);

	int GetGroundY() const { return groundY; }
};