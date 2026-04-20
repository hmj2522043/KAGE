#pragma once

//
class ScrollManager
{
private:
	int scrollX;
public:
	ScrollManager();
	void Update();
	int GetScroll() const;
};