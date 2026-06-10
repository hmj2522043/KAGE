#include "LimitGauge.h"

Gauge::Gauge(int maxValue, int fps)
	: value(0), maxValue(maxValue), timer(0), fps(fps)
{
}

void Gauge::Update()
{
	timer++;
	if (timer >= fps)
	{
		timer = 0;
		if (value < maxValue)
		{
			value++;
		}
	}
}

void Gauge::Draw(int x, int y, int width, int height)
{
	// 背景（灰色）
	DrawBox(x, y, x + width, y + height, GetColor(100, 100, 100), TRUE);

	// 現在値（青色）
	int fillWidth = width * value / maxValue;
	DrawBox(x, y, x + fillWidth, y + height, GetColor(0, 200, 255), TRUE);
}

void Gauge::Add(int v)
{
	value += v;

	if (value > maxValue)
	{
		value = maxValue;
	}
}

bool Gauge::IsFull() const
{
	return value >= maxValue;
}