#include "View.h"
#include "ViewConstants.h"
#include "../../src/DemoConstants.h"
#include "LoadBalancing-cpp/inc/Enums/PeerStates.h"
#include <math.h>

using namespace cocos2d;

#define Z_TOPMOST   100

const char* tileIconFiles[] = {
"tile-angler-fish.png",
"tile-crown.png",
"tile-drink-me.png",
"tile-fishbone.png",
"tile-glass-heart.png",
"tile-horse-head.png",
"tile-robe.png",
"tile-rocket.png",
};

const char* FONT_MEDIUM = "arial28.fnt";
const char* FONT_SMALL = "arial20.fnt";
const float TEXT_MEDIUM = 28;

// setContentSize does not work as expected, returning size by parameter
static CCMenuItem* decorateButton(CCMenuItemLabel* item, CCSize& size)
{
	const cocos2d::ccColor4F BORDER_COLOR = {1, 1, 1, 0.5};
	const cocos2d::ccColor4F NORMAL_COLOR = {1, 1, 1, 0.3f};

	CCDrawNode* bg = CCDrawNode::create();
	bg->setPosition(0, 0);
	float lw = item->getLabel()->getContentSize().width;
	float lh = item->getLabel()->getContentSize().height;
	float marginX = lh*0.2f;
	float marginY = lh*0.05f;
	float w = lw + 2*marginX;
	float h = lh + 2*marginY;
	CCPoint pp[] = {ccp(0, 0), ccp(w, 0), ccp(w, h), ccp(0, h)};
	bg->drawPolygon(pp, 4, NORMAL_COLOR, 1, BORDER_COLOR);
	bg->setPosition(-marginX, - marginY);
	item->addChild(bg);
	// adding another margin around rect
	size.width = w + marginX;
	size.height = h + marginY;
	return item;
}

static CCMenuItem* createButton(CCLabelBMFont* label, CCObject* target, SEL_MenuHandler menuHandler, CCSize& size)
{
	CCMenuItemLabel* item = CCMenuItemLabel::create(label, target, menuHandler);
	decorateButton(item, size);
	return item;
}

View::View()
	: lbl(NULL)
	, boardSize(0)
	, tileSize(0)
	, tileMargin(0)
	, tileRows(0)
	, tileCols(0)
	, sceneConnect(NULL)
	, sceneLobby(NULL)
	, sceneGame(NULL)
	, currentScene(NULL)
	, board(NULL)
	, tiles(NULL) 
	, labelState(NULL)
	, labelLog(NULL)
	, roomList(NULL)
	, savedRoomList(NULL)
	, lobbyStats1(NULL)
	, lobbyStats2(NULL)
	, labelMessage(NULL)
	, stateString(NULL)
	, inputUserName(NULL)
{
}

View::~View()
{
}

View* View::create()
{
	View *pRet = new View();
	if(pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
};

void View::init(LoadBalancingListener* lbl) 
{
	this->lbl = lbl;
#if CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE
	setKeypadEnabled(true);
#endif
}

void View::onExit()
{
}

// on "init" you need to initialize your instance
bool View::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	} while(0);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	return bRet;
}

bool View::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();
	CCPoint pos = board->convertToNodeSpace( touchLocation );
	return pos.x > 0 && pos.x < boardSize && pos.y > -boardSize && pos.y < 0;
}

void View::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
}

void View::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint touchLocation = touch->getLocation();    
	CCPoint pos = board->convertToNodeSpace( touchLocation );
	int x = (int)(pos.x / (tileSize + 2 * tileMargin));
	int y = (int)((boardSize + pos.y) / (tileSize + 2 * tileMargin));
	if(x >= 0 && x < tileCols && y >= 0 && y < tileRows)
		lbl->click(x + y * tileCols);
}

void View::setup()
{
    setupUI();
    info("setup");
    switchScene(sceneConnect);
}

void View::setupUI()
{
	CCMenuItem *b0, *b1, *b2, *b3;
	CCMenu* menu;
	float y;

	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	boardSize = sz.width < sz.height ? sz.width : sz.height;
	tileMargin = ceilf(sz.height / 200);

	// common controls
		
	labelState = CCLabelBMFont::create("Undef", FONT_SMALL, (float)boardSize, cocos2d::kCCTextAlignmentLeft);
	labelState->setLineBreakWithoutSpace(true);
	labelLog = CCLabelBMFont::create("Undef", FONT_SMALL, (float)boardSize, cocos2d::kCCTextAlignmentLeft);
	addChild(labelState);
	addChild(labelLog);
	labelState->setAnchorPoint(ccp(0, 1));
	labelLog->setAnchorPoint(ccp(0, 1));
	labelState->setPosition(0, sz.height);                                 // state docked to screen top
	labelLog->setPosition(0, labelLog->getContentSize().height * 4);	   // log docked to screen bottom

	// scenes
	float y0 = sz.height - labelState->getContentSize().height;
	sceneConnect = CCNode::create();
	sceneLobby = CCNode::create();
	sceneGame = CCNode::create();
	addChild(sceneConnect);
	addChild(sceneLobby);
	addChild(sceneGame);
	sceneConnect->setPositionY(y0);
	sceneLobby->setPositionY(y0);
	sceneGame->setPositionY(y0);

	// sceneConnect
	y = - sz.height / 3;
	CCLabelBMFont* l = CCLabelBMFont::create("User:", FONT_MEDIUM);
	float lw = l->getContentSize().width;
	float lh = l->getContentSize().height;
	float marginX = lh*0.3f;
	float marginY = lh*0.15f;
	l->setAnchorPoint(ccp(0, 0.5f));
	l->setPosition(marginX, y);
	sceneConnect->addChild(l);

#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
	inputUserName = CCLabelBMFont::create(DEFAULT_USER_NAME.ANSIRepresentation().cstr(), FONT_MEDIUM);
	sceneConnect->addChild(inputUserName);
	inputUserName->setAnchorPoint(ccp(0, 0.5f));
	inputUserName->setPosition(marginX + lw + marginX, y);    
#else
	inputUserName = CCTextFieldTTF::textFieldWithPlaceHolder("<set user name>", "", 28);
	inputUserName->setDelegate(this);
	inputUserName->setString(DEFAULT_USER_NAME.ANSIRepresentation().cstr());
	inputUserName->setColor(INPUT_COLOR);
	CCMenuItemLabel* inputUserNameButton = CCMenuItemLabel::create(inputUserName, this, menu_selector(View::menuInputUserNameTouchCallback));
	inputUserNameButton->setAnchorPoint(ccp(0, 0.5f));
	inputUserNameButton->setPosition(ccp(marginX + lw + marginX, y));    
	CCMenu* inputUserNameMenu = CCMenu::create(inputUserNameButton, NULL);
	inputUserNameMenu->setAnchorPoint(ccp(0, 0));
	inputUserNameMenu->setPosition(0,0);
	sceneConnect->addChild(inputUserNameMenu);

#endif
	y -= 2*lh;

	CCSize bs;
	b0 = createButton(CCLabelBMFont::create("Connect", FONT_MEDIUM), this, menu_selector(View::menuConnectCallback), bs);
	menu = CCMenu::create(b0, NULL);
	menu->setPosition(0, 0);
	b0->setAnchorPoint(ccp(0.5f, 0.5f));
	b0->setPosition(boardSize/2, y);  y -= bs.height;
	sceneConnect->addChild(menu);

	// sceneLobby
	y = 0;
	float roomListHeight = sz.height / 4;
	lobbyStats1 = CCLabelBMFont::create("Saved Games:", FONT_MEDIUM);	
	savedRoomList = ListController::create(sz.width, roomListHeight, this, (ListControllerHandler)&View::savedRoomListCallback);
	b0 = createButton(CCLabelBMFont::create("Refresh", FONT_MEDIUM), this, menu_selector(View::menuRefreshCallback), bs);
	b1 = createButton(CCLabelBMFont::create("Leave", FONT_MEDIUM), this, menu_selector(View::menuLeaveCallback), bs);
	lobbyStats2 = CCLabelBMFont::create("Rooms in lobby:", FONT_MEDIUM);
	roomList = ListController::create(sz.width, roomListHeight, this, (ListControllerHandler)&View::roomListCallback);
	b2 = createButton(CCLabelBMFont::create("Random", FONT_MEDIUM), this, menu_selector(View::menuRandomGameCallback), bs);
	b3 = createButton(CCLabelBMFont::create("New Game", FONT_MEDIUM), this, menu_selector(View::menuNewGameCallback), bs);
	lobbyStats1->setAnchorPoint(ccp(0, 1));
	lobbyStats2->setAnchorPoint(ccp(0, 1));
	b0->setAnchorPoint(ccp(0, 0.5f));
	b1->setAnchorPoint(ccp(1, 0.5f));
	b2->setAnchorPoint(ccp(0, 0.5f));
	b3->setAnchorPoint(ccp(1, 0.5f));
	float bh = bs.height;
	float bsh2 = bs.height/2;
	lobbyStats1->setPositionY(y);                                     y -= lobbyStats1->getContentSize().height;
	savedRoomList->setPosition(0, y);                                 y -= roomListHeight + marginY;
	b0->setPosition(marginX, y - bsh2);  b1->setPosition(boardSize - marginX, y - bsh2);  y -= bh;
	lobbyStats2->setPositionY(y);                                     y -= lobbyStats2->getContentSize().height;
	roomList->setPosition(0, y);                                      y -= roomListHeight + marginY;
	b2->setPosition(marginX, y - bsh2);  b3->setPosition(boardSize- marginX, y - bsh2);  y -= bh;
	
	menu = CCMenu::create(b0, b1, b2, b3, NULL);
	menu->setPosition(0, 0);
	
	sceneLobby->addChild(lobbyStats1);
	sceneLobby->addChild(savedRoomList);	
	sceneLobby->addChild(lobbyStats2);
	sceneLobby->addChild(roomList);
	sceneLobby->addChild(menu);
	
	// sceneGame
	y = 0;
	labelMessage = CCLabelBMFont::create("Welcome", FONT_MEDIUM);
	labelMessage->setAnchorPoint(ccp(0, 1));
	labelMessage->setPositionY(y);           y -= labelMessage->getContentSize().height;
	board = CCDrawNode::create();
	board->setPositionY(y);                  y -= boardSize;
	
	CCPoint pp[] = {ccp(0, 0), ccp(boardSize, 0), ccp(boardSize, -boardSize), ccp(0, -boardSize)};
	board->drawPolygon(pp, 4, BOARD_COLOR, 0, BOARD_COLOR);

	b0 = createButton(CCLabelBMFont::create("Suspend", FONT_MEDIUM), this, menu_selector(View::menuSuspendCallback), bs);
	b1 = createButton(CCLabelBMFont::create("Abandon", FONT_MEDIUM), this, menu_selector(View::menuAbandonCallback), bs);
	b0->setAnchorPoint(ccp(0, 0.5f));
	b1->setAnchorPoint(ccp(1, 0.5f));
	// bh = set above
	b0->setPosition(marginX, y - bsh2);
	b1->setPosition(boardSize - marginX, y - bsh2);

	menu = CCMenu::create(b0, b1, NULL);
	menu->setPosition(0, 0);

	sceneGame->addChild(board);
	sceneGame->addChild(labelMessage);
	sceneGame->addChild(menu, Z_TOPMOST);

	sceneConnect->setPositionX(0);
	sceneLobby->setPositionX(sz.width);
	sceneGame->setPositionX(sz.width);
	currentScene = sceneConnect;

}

void View::menuInputUserNameTouchCallback(CCObject* pSender)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE
#else
	inputUserName->attachWithIME();
#endif
}

bool View::onTextFieldInsertText(cocos2d::CCTextFieldTTF * pSender, const char* text, int nLen)
{
	return false;
}

void View::keyBackClicked()
{
	CCDirector::sharedDirector()->popScene();
}

void View::menuLeaveCallback(CCObject* pSender)
{
	if(!lbl->leave())
		CCDirector::sharedDirector()->popScene();
}

const char* View::getInputUserName()
{
	return inputUserName->getString();
}

void View::menuConnectCallback(CCObject* pSender)
{
	lbl->connect(getInputUserName());
}

void View::menuNewGameCallback(CCObject* pSender)
{
	lbl->createRoom(getInputUserName());
}

void  View::menuRandomGameCallback(CCObject* pSender)
{
	lbl->joinRandomRoom();
}

void  View::menuRefreshCallback(CCObject* pSender)
{
	lbl->querySavedRoomList();
}

void  View::menuSuspendCallback(CCObject* pSender)
{
	lbl->suspendRoom();
}

void  View::menuAbandonCallback(CCObject* pSender)
{
	lbl->abandonRoom();
}

void View::roomListCallback(const char* name)
{
	lbl->joinRoom(name);
}

void View::savedRoomListCallback(const char* name)
{
	lbl->joinSavedRoom(name);
}

void View::info(const char* format, ... )
{	
	char buf[256];
	va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, 256, format, argptr);
    va_end(argptr);

	labelLog->setString(buf);

	BaseView::info(buf);
}

void View::warn(const char* format, ... )
{
	char buf[256];
	va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, 256, format, argptr);
    va_end(argptr);

	labelLog->setString(buf);

	BaseView::warn(buf);
}

void View::error(const char* format, ... )
{
	char buf[256];
	va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, 256, format, argptr);
    va_end(argptr);

	labelLog->setString(buf);

	BaseView::error(buf);
}

void View::onStateChange(int state, const char* stateStr, bool inLobby, bool inRoom)
{
	int l = strlen(stateStr);
	delete[] stateString;
	stateString = new char[l + 1];
	strcpy(stateString, stateStr);

	refreshStateInfo(NULL);
	if(inLobby)
		switchScene(sceneLobby);
	else if(inRoom)
		switchScene(sceneGame);
	else
		switchScene(sceneConnect);
//	else if(state == ExitGames::LoadBalancing::PeerStates::Error)
//		switchScene(sceneConnect);
    
//    scrollToCurrentRoom(roomList)
}
void View::refreshStateInfo(const char* stateInfo)
{
	const int BUF_SIZE = 1024;
	char buf[BUF_SIZE];
	sprintf(buf, "%s %s", stateString, stateInfo ? stateInfo : "");
	labelState->setString(buf);
}

void View::refreshMessage(const char* msg)
{
	labelMessage->setString(msg);
}

void View::updateRoomList(const char* roomNames[], unsigned int size)
{
	roomList->removeAllItems();
	for(unsigned int i = 0; i < size;++i)
		roomList->addItem(roomNames[i]);
}

void View::updateSavedRoomList(const char* roomNames[], unsigned int size)
{
	savedRoomList->removeAllItems();
	for(unsigned int i = 0; i < size;++i)
		savedRoomList->addItem(roomNames[i]);
}

void View::setupBoard(int tileCols, int tileRows)
{
	this->tileCols = tileCols;
	this->tileRows = tileRows;
	info("setupBoard", tileCols, tileRows);
	if(tiles)
		tiles->removeFromParentAndCleanup(true);
	tiles = CCNode::create();
	board->addChild(tiles, Z_TOPMOST);
	
	tileSize = (boardSize - (tileCols * tileMargin * 2)) / tileCols;
	for(int y = 0; y < tileRows;++y)
		for(int x = 0; x < tileCols;++x)
		{
			int i = x + y*tileCols;
			CCNode* tile = CCNode::create();
			tile->setPosition(tileMargin + x * (tileSize + 2 * tileMargin), - (tileMargin + ((tileRows - 1 - y) * (tileSize + 2 * tileMargin))));
			tiles->addChild(tile);
		}
}

void View::updateBoard(unsigned char* tileStates, unsigned char flippedTiles[2])
{
	for(int  i = 0; i < tileCols*tileRows;++i)
	{
		CCNode* tile = (CCNode*)tiles->getChildren()->objectAtIndex(i);
		unsigned char tileState = tileStates[i];
		bool flipped = flippedTiles[0] == i || flippedTiles[1] == i;
		if(tile->getChildrenCount() == 0)
		{
			CCDrawNode* rect = CCDrawNode::create();
			CCPoint pp[] = {ccp(0, 0), ccp(tileSize, 0), ccp(tileSize, -tileSize), ccp(0, -tileSize)};
			rect->drawPolygon(pp, 4, HIDDEN_TILE_COLOR, 0, HIDDEN_TILE_COLOR);
			rect->setAnchorPoint(ccp(0, 1));
			CCNode* icon;
			if(tileState)
			{
				icon = CCSprite::create(tileIconFiles[tileState - 1]);
				icon->setScale(tileSize / icon->getContentSize().width);
				icon->setAnchorPoint(ccp(0, 1));
			}
			else
				icon = CCNode::create(); //dummy

			tile->addChild(rect);
			tile->addChild(icon);
		}
		CCNode* rect = (CCNode*)tile->getChildren()->objectAtIndex(0);
		CCNode* icon = (CCNode*)tile->getChildren()->objectAtIndex(1);
		rect->setVisible(tileState != 0 && !flipped);
		icon->setVisible(flipped);
	}
}

int View::sceneOrder(CCNode* scene)
{
	if(scene == sceneConnect) return 1;
	else if(scene == sceneLobby) return 2;
	else if(scene == sceneGame) return 3;
	else return 0;
}

void View::switchScene(CCNode* scene)
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	float w = sz.width;
	float h = sz.height;
	if(currentScene != scene)
	{
		bool back = false;
		if(currentScene)
		{
			if(sceneOrder(scene) < sceneOrder(currentScene))
				back = true;
			if(back)
				currentScene->runAction(CCMoveTo::create(0.4f, ccp(w, currentScene->getPositionY())));
			else
				currentScene->runAction(CCMoveTo::create(0.4f, ccp(-w, currentScene->getPositionY())));
		}		
		if(back)
			scene->setPositionX(-w);
		else
			scene->setPositionX(w);
		scene->runAction(CCMoveTo::create(0.4f, ccp(0, scene->getPositionY())));

		currentScene = scene;
	}
}