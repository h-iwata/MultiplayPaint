#ifndef _LIST_CONTROLLER_H_
#define _LIST_CONTROLLER_H_

#include "cocos2d.h"

typedef void (cocos2d::CCObject::*ListControllerHandler)(const char*);

class ListController : public cocos2d::CCLayer
{
public:
	static ListController* create(float width, float height, cocos2d::CCObject* target, ListControllerHandler selector);

	ListController(float width, float height, cocos2d::CCObject* target, ListControllerHandler selector);
    ~ListController();

	bool addItem(const char* label);
	bool removeItem(const char* label);
	void removeAllItems();

    void menuCallback(cocos2d::CCObject* pSender);

	//from cocos2d::CCLayer
	virtual bool init();
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

private:
	void scrollTo(float moveY);
    float beginTouchPosOffset;
    cocos2d::CCMenu* menu;
	cocos2d::CCObject* target;
	ListControllerHandler selector;
	float width;
	float height;
};

#endif
