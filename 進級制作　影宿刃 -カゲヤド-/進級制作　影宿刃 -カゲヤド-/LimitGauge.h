#pragma once
#include "DxLib.h"

class Gauge
{
private:
	int value;
	int maxValue;
	int timer;
	int fps;

public:
	Gauge(int maxValue = 100, int fps = 60);

	void Update();
	void Draw(int x, int y, int width, int height);
	void Reset() { value = 0; timer = 0; }
	int GetValue() const { return value; }
	void Add(int v);
	bool IsFull() const;
};