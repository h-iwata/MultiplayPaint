#ifndef __VIEW_H
#define __VIEW_H

#include "cocos2d.h"
#include "../../src/LoadBalancingListener.h"
#include "../../src/BaseView.h"
#include "ListController.h"

class View : public cocos2d::CCLayer, public BaseView, public cocos2d::CCTextFieldDelegate
{
public:
	View();
	~View();

	static View* create();
	
	// BaseView
	virtual void info(const char* format, ... );
	virtual void warn(const char* format, ... );
	virtual void error(const char* format, ... );

	virtual void onStateChange(int state, const char* stateStr, bool inLobby, bool inRoom);
	virtual void refreshStateInfo(const char* stateInfo);
	virtual void refreshMessage(const char* msg);

    virtual void setup();
    virtual void setupBoard(int tileCols, int tileRows);
	virtual void updateBoard(unsigned char* tiles, unsigned char flippedTiles[2]);
	virtual void updateRoomList(const char* roomNames[], unsigned int size);
	virtual void updateSavedRoomList(const char* savedRoomNames[], unsigned int size);

	//cocos2d::CCLayer
	virtual bool init();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void keyBackClicked();

	// cocos2d::CCTextFieldDelegate
	virtual bool onTextFieldInsertText(cocos2d::CCTextFieldTTF * pSender, const char * text, int nLen);


	// self
	void init(LoadBalancingListener* lbl);

private: 
	void setupUI(void);

	LoadBalancingListener* lbl;
	float boardSize;
	float tileSize;
	float tileMargin;
	int tileRows;
	int tileCols;

	void switchScene(cocos2d::CCNode*);
	int sceneOrder(cocos2d::CCNode*);
	const char* getInputUserName();

	// scenes as nodes let keep unmovable info overlay
	cocos2d::CCNode* sceneConnect;
	cocos2d::CCNode* sceneLobby;
	cocos2d::CCNode* sceneGame;

	cocos2d::CCNode* currentScene;

	// all scenes
	cocos2d::CCLabelBMFont* labelState;
	cocos2d::CCLabelBMFont* labelLog;
	void menuLeaveCallback(CCObject* pSender);
	char* stateString;

	// sceneConnect
#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
	cocos2d::CCLabelBMFont* inputUserName;
#else
	cocos2d::CCTextFieldTTF* inputUserName;
#endif
	void menuInputUserNameTouchCallback(CCObject* pSender);
	void menuConnectCallback(CCObject* pSender);

	// sceneLobby
	cocos2d::CCLabelBMFont* lobbyStats1;
	cocos2d::CCLabelBMFont* lobbyStats2;
	ListController* roomList;
	ListController* savedRoomList;
	void roomListCallback(const char* name);
	void savedRoomListCallback(const char* name);

	void menuNewGameCallback(CCObject* pSender);
	void menuRandomGameCallback(CCObject* pSender);
	void menuRefreshCallback(CCObject* pSender);	
	
	// sceneGame
	cocos2d::CCLabelBMFont* labelMessage;
	cocos2d::CCDrawNode* board;
	cocos2d::CCNode* tiles;

	void menuSuspendCallback(CCObject* pSender);
	void menuAbandonCallback(CCObject* pSender);
};

#endif
