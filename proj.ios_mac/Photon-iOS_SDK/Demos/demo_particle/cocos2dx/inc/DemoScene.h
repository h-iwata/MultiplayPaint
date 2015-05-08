#ifndef __DEMO_SCENE_H
#define __DEMO_SCENE_H

#include "cocos2d.h"
#include "../../src/LoadBalancingListener.h"
#include "../../src/BaseView.h"
#include "PlayerView.h"
#include "ListController.h"


class DemoScene : public cocos2d::CCLayer, public BaseView
{
public:
	DemoScene();
	~DemoScene();
	static DemoScene* create();
	
	void setLBC(ExitGames::LoadBalancing::Client* lbc, LoadBalancingListener* lbl);
	void setupUI(void);
	void tick(float delta);

	virtual void info(const char* format, ... );
	virtual void warn(const char* format, ... );
	virtual void error(const char* format, ... );
	virtual void updateState(int state, const char* stateStr, const char* joinedRoomName);
	virtual void initPlayers();
	virtual void addPlayer(int playerNr, const char* playerName, bool local);
	virtual void removePlayer(int playerNr);
	virtual void changePlayerColor(int playerNr, int color);
	virtual void changePlayerPos(int playerNr, int x, int y);
    virtual void setupScene(int gridSize);
	virtual void updateRoomList(const char* roomNames[], unsigned int size);

	//from cocos2d::CCLayer
	virtual bool init();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

private: 
	int canvasSize;
	int cellSize;

	cocos2d::CCDrawNode* canvas;
	cocos2d::CCDrawNode* checker;
	cocos2d::CCDrawNode* grid;
	cocos2d::CCNode* playersNode;

	ExitGames::LoadBalancing::Client* lbc;
	LoadBalancingListener* lbl;

	cocos2d::CCLabelBMFont* labelState;
	cocos2d::CCLabelBMFont* labelLog;
	ListController* roomList;

	void drawChecker(int gridSize);
	void drawGrid(int gridSize);
	void updatePlayerCellSizes(int gridSize);

	void menuNewGameCallback(CCObject* pSender);
	void menuLeaveCallback(CCObject* pSender);
	void menuGridSizeCallback(CCObject* pSender);
	void menuRandomColorCallback(CCObject* pSender);
	void menuAutomoveCallback(bool automove);
	void menuUseGroupsCallback(bool useGroups);

	void roomListCallback(const char* name);
	
	void onTouch(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	float setupDesktop(bool movable);
};

#endif
