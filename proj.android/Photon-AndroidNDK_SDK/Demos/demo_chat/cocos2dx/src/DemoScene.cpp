#include "DemoScene.h"
#include "../inc/ViewConstants.h"
#include "../inc/DemoConstants.h"

#ifdef _EG_ANDROID_PLATFORM
#include <android/log.h>
#endif

using namespace cocos2d;

// set according to screen size in DemoScene::init()
float FONT_SIZE           = 32;

// index = const in ExitGames::Chat::UserStatus
static const char* UserStatusStr[] = 
{
	"Offline",
	"Invisible",
	"Online",
	"Away",
	"Dnd",
	"Lfg",
	"Playing"
};

// index = const in ExitGames::Chat::ClientState::ClientState
static const char* ClientStateStr[] = 
{
	"Uninitialized",
	"ConnectingToNameServer",
	"ConnectedToNameServer",
	"Authenticating",
	"Authenticated",
	"DisconnectingFromNameServer",
	"ConnectingToFrontEnd",
	"ConnectedToFrontEnd",
	"Disconnecting",
	"Disconnected"
};

DemoScene::DemoScene()
	: labelState(NULL)
	, logList(NULL)
	, userIdInput(NULL)
	, messageInput(NULL)
	, messageInputMaxWidth(0)
	, subscribeButtonMode(0)
	, friendButtonMode(0)
{
	subscribeButtons[0] = NULL;
	subscribeButtons[1] = NULL;
	friendButtons[0] = NULL;
	friendButtons[1] = NULL;
}

DemoScene::~DemoScene()
{
}

DemoScene* DemoScene::create()
{
	DemoScene *pRet = new DemoScene();
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

// on "init" you need to initialize your instance
bool DemoScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	}
	while(0);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

	float h = CCDirector::sharedDirector()->getWinSize().height;
	FONT_SIZE = h * 5 / 100;

	return bRet;
}

bool DemoScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return false;
}

void DemoScene::resetInputFocus()
{
	userIdInput->detachWithIME();
	messageInput->detachWithIME();
}

static void drawBorder(CCNode* node)
{
	CCDrawNode* d = CCDrawNode::create();
	float width = node->getContentSize().width;
	float height = node->getContentSize().height;
	float marginX = CCDirector::sharedDirector()->getWinSize().width / 100 + 2;
	float marginY = - height / 10 + 2; // negative for text labels
	CCPoint pp[] = {ccp(-marginX, -marginY), ccp(width + marginX, -marginY), ccp(width + marginX, height + marginY), ccp(-marginX, height + marginY)};
	d->drawPolygon(pp, 4, BUTTON_COLOR, 1, BUTTON_BORDER_COLOR);
	d->setAnchorPoint(node->getAnchorPoint());
	node->addChild(d);
}

void DemoScene::setupUI()
{
	CCSize sz = CCDirector::sharedDirector()->getWinSize();
	float y = sz.height;
	float spacing = sz.width / 10;
	ccColor3B grey = {180,180,180};

	CCLabelTTF* header = CCLabelTTF::create("Photon Chat Demo", FONT_NAME, FONT_SIZE * 0.7f);
	header->setColor(grey);
	header->setAnchorPoint(ccp(0, 1));
	header->setPosition(ccp(0, y));
	addChild(header);

	y -= header->getContentSize().height;

	labelState = CCLabelTTF::create("Undef", FONT_NAME, FONT_SIZE * 0.7f);
	labelState->setColor(grey);
	labelState->setAnchorPoint(ccp(0, 1));
	labelState->setPosition(ccp(0, y));
	addChild(labelState);

	y -= labelState->getContentSize().height;

	CCLabelTTF* userIdLabel = CCLabelTTF::create("User id: ", FONT_NAME, FONT_SIZE);
	userIdLabel->setAnchorPoint(ccp(0, 1));
	userIdLabel->setPosition(ccp(0, y));

	userIdInput = CCTextFieldTTF::textFieldWithPlaceHolder("<set user id>", FONT_NAME, FONT_SIZE);
	userIdInput->setDelegate(this);
	userIdInput->setString(DEFAULT_USER_ID.ANSIRepresentation().cstr());
	userIdInput->setColor(INPUT_COLOR);
	userIdInput->setContentSize(CCSize(sz.width - userIdLabel->getContentSize().width, userIdInput->getContentSize().height));
	CCMenuItemLabel* userIdInputButton = CCMenuItemLabel::create(userIdInput, this, menu_selector(DemoScene::menuUserIdTouchCallback));
	userIdInputButton->setAnchorPoint(ccp(1, 1));
	CCMenu* userIdInputMenu = CCMenu::create(userIdInputButton, NULL);
	userIdInputMenu->setPosition(sz.width, y);

	addChild(userIdLabel);
	addChild(userIdInputMenu);

	y -= userIdInput->getContentSize().height;

	CCMenuItem* b0 = CCMenuItemLabel::create(CCLabelTTF::create("Connect", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuConnectCallback));	
	CCMenuItem* b1 = CCMenuItemLabel::create(CCLabelTTF::create("Disconnect", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuDisconnectCallback));
	CCMenuItem* b20 = CCMenuItemLabel::create(CCLabelTTF::create("Subscribe", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuSubscribeCallback));
	CCMenuItem* b21 = CCMenuItemLabel::create(CCLabelTTF::create("Unsubscr.", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuUnsubscribeCallback));
	CCMenuItem* b30 = CCMenuItemLabel::create(CCLabelTTF::create("Set friends", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuSetFriendsCallback));
	CCMenuItem* b31 = CCMenuItemLabel::create(CCLabelTTF::create("Clr. friends", FONT_NAME, FONT_SIZE), this, menu_selector(DemoScene::menuClearFriendsCallback));
	drawBorder(b0);
	drawBorder(b1);
	drawBorder(b20);
	drawBorder(b21);
	drawBorder(b30);
	drawBorder(b31);

	subscribeButtons[0] = b20;
	subscribeButtons[1] = b21;
	friendButtons[0] = b30;
	friendButtons[1] = b31;

	b0->setAnchorPoint(ccp(0, 1));
	b1->setAnchorPoint(ccp(1, 1));
	b20->setAnchorPoint(ccp(0, 1));
	b21->setAnchorPoint(ccp(0, 1));
	b30->setAnchorPoint(ccp(1, 1));
	b31->setAnchorPoint(ccp(1, 1));
	int bh = (int)b0->getContentSize().height;
	b0->setPosition(ccp(0, 0));
	b1->setPosition(ccp(sz.width, 0));
	b20->setPosition(ccp(0, -bh));
	b21->setPosition(ccp(0, -bh));
	b30->setPosition(ccp(sz.width, -bh));
	b31->setPosition(ccp(sz.width, -bh));

	CCMenu* pMenu = CCMenu::create(b0, b1, b20, b21, b30, b31, NULL);
	pMenu->setPosition(0, y);
	addChild(pMenu);

	y -= bh*2;
	
	const float logHeight = sz.height/3;
	logList = ListController::create(sz.width, logHeight, this, (ListControllerHandler)&DemoScene::logTouchCallback);
	logList->setAnchorPoint(ccp(0, 1));
	logList->setPosition(ccp(0, y));
	addChild(logList);

	y -= logHeight;

	y -= FONT_SIZE/2;

	CCMenuItem* sendButton = CCMenuItemLabel::create(CCLabelTTF::create("Send", FONT_NAME, FONT_SIZE * 1.2f), this, menu_selector(DemoScene::menuSendCallback));
	drawBorder(sendButton);
	sendButton->setAnchorPoint(ccp(1, 1));
	sendButton->setPosition(ccp(sz.width, 0));
	CCMenu* sendMenu = CCMenu::create(sendButton, NULL);
	sendMenu->setPosition(0, y);
	addChild(sendMenu);

	messageInputMaxWidth = sz.width - sendButton->getContentSize().width - spacing;
	messageInput = CCTextFieldTTF::textFieldWithPlaceHolder("<message>", CCSize(0, 0), kCCTextAlignmentLeft, FONT_NAME, FONT_SIZE * 1.2f);
	messageInput->setDelegate(this);
	messageInput->setColor(INPUT_COLOR);
	CCMenuItemLabel* messageInputButton = CCMenuItemLabel::create(messageInput, this, menu_selector(DemoScene::menuMessageTouchCallback));
	messageInputButton->setAnchorPoint(ccp(0, 1));
	CCMenu* messageInputMenu = CCMenu::create(messageInputButton, NULL);
	messageInputMenu->setPosition(0, y);
	addChild(messageInputMenu);

	y -= messageInput->getContentSize().height;

	y -= FONT_SIZE/2;

	CCMenu* userStatusMenu = CCMenu::create();
	userStatusMenu->setPosition(0, 0);
	float x = 0;
	for(int i=0; i<sizeof(UserStatusStr)/sizeof(const char*); ++i)
	{
		CCLabelTTF* l = CCLabelTTF::create(UserStatusStr[i], FONT_NAME, FONT_SIZE);
		drawBorder(l);
		CCMenuItem* b = CCMenuItemLabel::create(l, this, menu_selector(DemoScene::menuUserStatusCallback));
		b->setTag(i);
		b->setAnchorPoint(ccp(0, 1));
		CCSize size = b->getContentSize();
		if(x + size.width > sz.width)
		{
			x = 0;
			y -= size.height;
		}
		b->setPosition(ccp(x, y));
		x += size.width + spacing;
		userStatusMenu->addChild(b);
	}
	addChild(userStatusMenu);

	subscribeButtonModeSet(0);
	friendButtonModeSet(0);
}

void DemoScene::subscribeButtonModeSet(int m)
{
	subscribeButtonMode = m;
	if(m >= 0 && m < 2)
	{
		subscribeButtons[m]->setVisible(true);
		subscribeButtons[1 - m]->setVisible(false);
	}
}

void DemoScene::friendButtonModeSet(int m)
{
	friendButtonMode = m;
	if(m >= 0 && m < 2)
	{
		friendButtons[m]->setVisible(true);
		friendButtons[1 - m]->setVisible(false);
	}
}

void DemoScene::messageInputScroll()
{
	float pos = messageInputMaxWidth - messageInput->getContentSize().width;
	if(pos < 0) 
		messageInput->setPositionX(pos);
	else
		messageInput->setPositionX(0);
}

bool DemoScene::onTextFieldInsertText(cocos2d::CCTextFieldTTF * sender, const char* text, int nLen)
{
	if(sender == messageInput)
		messageInputScroll();
	if(sender == messageInput && '\n' == *text) 
		sendMessage();
	return false;
}

bool DemoScene::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
	if(sender == messageInput)
		messageInputScroll();
	return false;
}

bool DemoScene::onTextFieldAttachWithIME(CCTextFieldTTF * sender)
{
#ifndef _EG_WINDOWS_PLATFORM
#ifndef _EG_MARMALADE_PLATFORM
	float y = sender->convertToWorldSpace(ccp(0,0)).y;
	float y0 = CCDirector::sharedDirector()->getWinSize().height / 2;
	if(y < y0)
		setPositionY(y0 - y);
#endif
#endif
	return false;
}

bool DemoScene::onTextFieldDetachWithIME(CCTextFieldTTF * sender)
{
#ifndef _EG_WINDOWS_PLATFORM
#ifndef _EG_MARMALADE_PLATFORM
	setPositionY(0);
#endif
#endif
	return false;
}

void DemoScene::setClient(ExitGames::Chat::Client* c) 
{
	this->client = c;
}

void DemoScene::menuUserIdTouchCallback(CCObject* pSender)
{
	userIdInput->attachWithIME();
}

void DemoScene::menuMessageTouchCallback(CCObject* pSender)
{
	messageInput->attachWithIME();
}

void DemoScene::menuConnectCallback(CCObject* pSender)
{
	resetInputFocus();

	log( "[i]: Connecting...");
	client->setUserId(userIdInput->getString());
	client->connect();
}

void DemoScene::menuDisconnectCallback(CCObject* pSender)
{
	resetInputFocus();

	if(client->getState() == ExitGames::Chat::ClientState::Uninitialized || client->getState() == ExitGames::Chat::ClientState::Disconnected) 
		CCDirector::sharedDirector()->popScene();
	else
		client->disconnect();
}

void DemoScene::menuSubscribeCallback(CCObject* pSender)
{
	resetInputFocus();

	int size = sizeof(SUBSCRIBE_CHANNELS)/sizeof(const char*);
	ExitGames::Common::JVector<ExitGames::Common::JString> chs(size);
	for(int i=0; i<size; ++i)
		chs.addElement(SUBSCRIBE_CHANNELS[i]);
	if(client->opSubscribe(chs))
		log("[i]: Subscribing...\n");
	else
		log( "[i]: Connect first");
}

void DemoScene::menuSetFriendsCallback(CCObject* pSender)
{
	resetInputFocus();

	ExitGames::Common::JVector<ExitGames::Common::JString> friends(USER_COUNT);
	for(int i=0; i<USER_COUNT; ++i)
		friends.addElement(USER_PREFIX+i);
	if(client->opAddFriends(friends))
	{
		log("[i]: Setting friends: %s ...\n", friends.toString().UTF8Representation().cstr());
		friendButtonModeSet(1);
	}
	else
		log("[i]: Connect first");
}

void DemoScene::menuClearFriendsCallback(CCObject* pSender)
{
	resetInputFocus();

	ExitGames::Common::JVector<ExitGames::Common::JString> friends(USER_COUNT);
	for(int i = 0;i < USER_COUNT;++i)
		friends.addElement(USER_PREFIX+i);
	if(client->opRemoveFriends(friends))
		log("[i]: Clearing friends...\n", friends.toString().UTF8Representation().cstr());
	else
		log( "[i]: Connect first");
	friendButtonModeSet(0);
}

void DemoScene::menuUnsubscribeCallback(CCObject* pSender)
{
	resetInputFocus();

	int size = sizeof(SUBSCRIBE_CHANNELS)/sizeof(const char*);
	ExitGames::Common::JVector<ExitGames::Common::JString> chs(size);
	for(int i = 0;i < size;++i)
		chs.addElement(SUBSCRIBE_CHANNELS[i]);
	if(client->opUnsubscribe(chs))
		log("[i]: Unsubscribing...\n");
	else
		log( "[i]: Connect first");
}

void DemoScene::menuSendCallback(CCObject* pSender) 
{
	sendMessage();
}

void DemoScene::menuUserStatusCallback(CCObject* pSender)
{
	resetInputFocus();

	CCMenuItemLabel* item = (CCMenuItemLabel*)pSender;
	if(client->opSetOnlineStatus(item->getTag(), ExitGames::Common::ValueObject<ExitGames::Common::JString>("My status changed"))) // update message
		log("[i] my status sent: %d", item->getTag());
}

void DemoScene::sendMessage() 
{
	const char* s = messageInput->getString();
	const char* sep = strrchr(s, ':');
	if(sep) // publish
	{
		char* name = (char*)alloca(sep - s + 1);
		strncpy(name, s, sep - s);
		name[sep - s] = '\0';
		client->opPublishMessage(name, ExitGames::Common::ValueObject<ExitGames::Common::JString>(sep + 1));
	}
	else 
	{
		sep = strrchr(s, '@');
		if(sep) // private
		{
			char* name = (char*)alloca(sep - s + 1);
			strncpy(name, s, sep - s);
			name[sep - s] = '\0';
			client->opSendPrivateMessage(name, ExitGames::Common::ValueObject<ExitGames::Common::JString>(sep + 1));
		}
		else
		{ // publish to random channel
			static int cnt = 0;
			const ExitGames::Common::JVector<ExitGames::Chat::Channel>& chs = client->getPublicChannels();
			if(chs.getSize())
				client->opPublishMessage(chs[cnt++ % chs.getSize()].getName(), ExitGames::Common::ValueObject<ExitGames::Common::JString>(s));
			else
				log( "[i]: Not subscribed to any channels");
		}
	}
	messageInput->setString(NULL);
	messageInputScroll();
}

void DemoScene::logTouchCallback(const char* name)
{
}

void DemoScene::log(const char* format, ... )
{	
	const int BUF_SIZE = 16384;
	char buf[BUF_SIZE];
	va_list argptr;
    va_start(argptr, format);
    vsnprintf(buf, BUF_SIZE, format, argptr);	
    va_end(argptr);

	CCLOG(buf);
#ifdef _EG_ANDROID_PLATFORM
	__android_log_print(ANDROID_LOG_INFO, "DemoChat", buf, NULL);
#endif

	logList->addItem(buf);
	logList->scrollToBottom();
}

void DemoScene::debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string)
{
	const ExitGames::Common::UTF8String& s = string.UTF8Representation();
	switch(debugLevel) 
	{
	case ExitGames::Common::DebugLevel::ERRORS: 
		log("[e]: %s", s.cstr()); break;
	case ExitGames::Common::DebugLevel::WARNINGS: 
		log("[w]: %s", s.cstr()); break;
	default: 
		log("[i]: %s", s.cstr()); break;
	}

	
}

void DemoScene::onStateChange(int state)
{
	char buf[256];
	snprintf(buf, 256, "%s", state >=0 && state < sizeof(ClientStateStr)/sizeof(const char*) ? ClientStateStr[state] : "?");
	labelState->setString(buf);
	if(state == ExitGames::Chat::ClientState::ConnectedToFrontEnd)
	{
		log("[i]: [Subscribe] for public channels");
		log("[i]:    or type in 'userid@message'");
		log("[i]:    and press 'OK' for private");
		subscribeButtonModeSet(0);
	}
	else if(state == ExitGames::Chat::ClientState::Uninitialized || state == ExitGames::Chat::ClientState::Disconnected)
	{
		if(client->getDisconnectedCause() == ExitGames::Chat::DisconnectCause::INVALID_AUTHENTICATION)
		{
			log("[i]: Disconnected due to invalid authentication");
			log("[i]: Is app id correct?");
		}
		else
			log("[i]: Disconnected");
		log("-------------------------------------------------");
		log("[i]: touch log lines to see details");
		log("[i]: type in user id and press [Connect]");
		subscribeButtonModeSet(0);
		friendButtonModeSet(0);
	}
}

void DemoScene::connectionErrorReturn(int errorCode)
{
	CCLOGERROR("Connection Error %d", errorCode);
	log("[i]: Connection Error %d", errorCode);
}

void DemoScene::clientErrorReturn(int errorCode)
{
	CCLOGERROR("Error %d", errorCode);
	log("[i]: Error %d", errorCode);
}

void DemoScene::warningReturn(int warningCode)
{
	log("[i]: Warning %d", warningCode);
}

void DemoScene::serverErrorReturn(int errorCode)
{
	log("[i]: Server Error %d", errorCode);
}

void DemoScene::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	log("[i]: Connected to Front End");
}

void DemoScene::disconnectReturn(void)
{	
}

void DemoScene::subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels, const ExitGames::Common::JVector<bool>& results)
{
	ExitGames::Common::JVector<ExitGames::Common::JString> ok(100);
	for(unsigned int i = 0; i < channels.getSize();++i) 
	{
		if(results[i])
			ok.addElement(channels[i]);
	}
	log("[i]: Subscribed to %s", ok.toString().UTF8Representation().cstr());
	log("[i]: type in 'channel:message'");
	log("[i]:   and press [Send] to publish");
	subscribeButtonModeSet(1);
}

void DemoScene::unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString>& channels)
{
	log("[i]: Unsubscribed from %s", channels.toString().UTF8Representation().cstr());
	subscribeButtonModeSet(0);
}

void DemoScene::onGetMessages(const ExitGames::Common::JString& channelName, const ExitGames::Common::JVector<ExitGames::Common::JString>& senders, const ExitGames::Common::JVector<ExitGames::Common::Object>& messages)
{
	for(unsigned int i = 0;i < senders.getSize();++i) 
		log("[%s:%s]: %s", channelName.UTF8Representation().cstr(), senders[i].UTF8Representation().cstr(), messages[i].toString().UTF8Representation().cstr());
}

void DemoScene::onPrivateMessage(const ExitGames::Common::JString& sender, const ExitGames::Common::Object& message, const ExitGames::Common::JString& channelName)
{
	log("[%s@%s]: %s", channelName.UTF8Representation().cstr(), sender.UTF8Representation().cstr(), message.toString().UTF8Representation().cstr());
}

void DemoScene::onStatusUpdate(const ExitGames::Common::JString& user, int status, bool gotMessage, const ExitGames::Common::Object& message)
{
	const char* statusStr = "";
	if(status < sizeof(UserStatusStr)/sizeof(const char*)) 
		statusStr = UserStatusStr[status];
	log("[i]: %s: %s(%d) / %s", user.UTF8Representation().cstr(), statusStr, status, gotMessage ? message.toString().UTF8Representation().cstr(): "[message skipped]");
}
