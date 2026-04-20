#include "ScrollManager.h"

ScrollManager::ScrollManager()
{
	scrollX = 0;
}

void ScrollManager::Update()
{
	scrollX += 5;
}

int ScrollManager::GetScroll() const
{
	return scrollX;
}