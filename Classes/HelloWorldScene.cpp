#include "HelloWorldScene.h"
#include "NetworkEvent.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    scene->addChild(HelloWorld::create());
    return scene;
}

bool HelloWorld::init()
{
    if (!Layer::init()) {
        return false;
    }

    showParts();

    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

    _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
    _renderTexture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addChild(_renderTexture);

    EventListenerTouchOneByOne* _listener = EventListenerTouchOneByOne::create();
    _listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

    networkLogic = new NetworkListener();
    scheduleUpdate();

    return true;
}

void HelloWorld::showParts()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithTTF("touch screen", "fonts/Marker Felt.ttf", 24);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height));
    addChild(title, 1);

    ui::Button* resetButton = ui::Button::create("reset-button.jpg");
    resetButton->setScale(0.3);
    resetButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::onTapResetButton, this));
    resetButton->setPosition(Vec2(origin.x + resetButton->getContentSize().width / 4,
                                  origin.y + resetButton->getContentSize().height / 2));
    addChild(resetButton);

    ui::Button* closeButton = ui::Button::create("CloseNormal.png",  "CloseSelected.png");
    closeButton->addTouchEventListener(CC_CALLBACK_1(HelloWorld::onTapCloseButton, this));
    closeButton->setPosition(Vec2(origin.x + visibleSize.width - closeButton->getContentSize().width / 2,
                                  origin.y + closeButton->getContentSize().height / 2));
    addChild(closeButton);
}

void HelloWorld::onTapResetButton(cocos2d::Ref* pSender)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    _renderTexture->removeFromParent();
    _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
    _renderTexture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addChild(_renderTexture);

    networkLogic->sendEvent(TAP_RESET, new ExitGames::Common::Hashtable());
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    _previousTouchLocation = touch->getLocation();
    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
    eventContent->put<int, float>(1, touch->getLocation().x);
    eventContent->put<int, float>(2, touch->getLocation().y);
    networkLogic->sendEvent(TOUCH_BEGAN, eventContent);
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    drawLine(networkLogic->playerNr, touch->getLocation());

    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
    eventContent->put<int, float>(1, touch->getLocation().x);
    eventContent->put<int, float>(2, touch->getLocation().y);
    networkLogic->sendEvent(TOUCH_MOVING, eventContent);
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {}

void HelloWorld::drawLine(int playerId, Vec2 position)
{
    _renderTexture->begin();
    auto drawNode = DrawNode::create();
    drawNode->drawSegment(_previousTouchLocation, position, 10, Color4F(1, 1, 1, 1));
    drawNode->visit();
    _renderTexture->end();
    _previousTouchLocation = position;
}

void HelloWorld::update(float delta)
{
    networkLogic->run();
    switch (networkLogic->getState()) {
    case STATE_CONNECTED:
    case STATE_LEFT:
        if (networkLogic->isRoomExists()) {
            networkLogic->setLastInput(NetworkListener::INPUT_JOIN_GAME);
        } else   {
            networkLogic->setLastInput(NetworkListener::INPUT_CREATE_GAME);
        }
        break;

    case STATE_DISCONNECTED:
        networkLogic->connect();
        break;

    case STATE_CONNECTING:
    case STATE_JOINING:
    case STATE_JOINED:
    case STATE_LEAVING:
    case STATE_DISCONNECTING:
    default:
        break;
    }

    while (!networkLogic->beginEventQueue.empty()) {
        std::array<float, 3>arr = networkLogic->beginEventQueue.front();
        networkLogic->beginEventQueue.pop();
        _previousTouchLocation =  Vec2(arr[1], arr[2]);
    }

    while (!networkLogic->eventQueue.empty()) {
        std::array<float, 3>arr = networkLogic->eventQueue.front();
        networkLogic->eventQueue.pop();
        drawLine(static_cast<int>(arr[0]), Vec2(arr[1], arr[2]));
    }

    while (!networkLogic->tapResetEnventQueue.empty()) {
        networkLogic->tapResetEnventQueue.pop();
        cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
        _renderTexture->removeFromParent();
        _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
        _renderTexture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        addChild(_renderTexture);
    }
}

void HelloWorld::onTapCloseButton(Ref* pSender)
{
    Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}
