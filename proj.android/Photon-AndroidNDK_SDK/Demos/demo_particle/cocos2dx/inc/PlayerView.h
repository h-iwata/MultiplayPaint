#ifndef __PLAYER_VIEW_H
#define __PLAYER_VIEW_H

#include "cocos2d.h"

class PlayerView : public cocos2d::CCNode
{
public:
	static const int MAX_USERNAME_LEN = 128;

	PlayerView(int number, const char* name, int cellSize, float borderWidth);

	void setPos(int x, int y);
	void setColor(int color);
	void setColor(const cocos2d::ccColor3B&);
	int getColor();
	void setNumber(int number) { this->number = number; }
	const char* getName();
	int getX() {return x;}
	int getY() {return y;}
	int getNumber() { return number; }
	void updateView(int size);
	void fadeOpacity(float v);
	void resetOpacity();
private:
	cocos2d::CCSprite* sprite;
	int number;
	int x;
	int y;
	cocos2d::ccColor3B color;
	cocos2d::CCLabelBMFont* label;
	int cellSize;
	float borderWidth;
	std::string name;
	float opacity;
};

#endif