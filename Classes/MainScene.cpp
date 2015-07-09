#include "MainScene.h"
#include "NetworkEvent.h"
#include "DataType.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    scene->addChild(MainScene::create());
    return scene;
}

bool MainScene::init()
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
    _listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    _listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    _listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

    networkListener = new NetworkListener();
    scheduleUpdate();

    return true;
}

void MainScene::showParts()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto title = Label::createWithTTF("touch screen", "fonts/Marker Felt.ttf", 24);
    title->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - title->getContentSize().height));
    addChild(title, 1);

    ui::Button* resetButton = ui::Button::create("reset-button.jpg");
    resetButton->setScale(0.3);
    resetButton->addTouchEventListener(CC_CALLBACK_1(MainScene::onTapResetButton, this));
    resetButton->setPosition(Vec2(origin.x + resetButton->getContentSize().width / 4,
                                  origin.y + resetButton->getContentSize().height / 2));
    addChild(resetButton);

    ui::Button* closeButton = ui::Button::create("CloseNormal.png",  "CloseSelected.png");
    closeButton->addTouchEventListener(CC_CALLBACK_1(MainScene::onTapCloseButton, this));
    closeButton->setPosition(Vec2(origin.x + visibleSize.width - closeButton->getContentSize().width / 2,
                                  origin.y + closeButton->getContentSize().height / 2));
    addChild(closeButton);
}

void MainScene::onTapResetButton(cocos2d::Ref* pSender)
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    _renderTexture->removeFromParent();
    _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
    _renderTexture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addChild(_renderTexture);

    networkListener->sendEvent(TAP_RESET, new ExitGames::Common::Hashtable());
}

bool MainScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    _previousTouchLocation = touch->getLocation();
    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
    eventContent->put<int, float>(DataType::LOCATION_X, touch->getLocation().x);
    eventContent->put<int, float>(DataType::LOCATION_Y, touch->getLocation().y);
    networkListener->sendEvent(TOUCH_BEGAN, eventContent);
    return true;
}

void MainScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    drawLine(networkListener->playerNr, touch->getLocation());

    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
    eventContent->put<int, float>(DataType::LOCATION_X, touch->getLocation().x);
    eventContent->put<int, float>(DataType::LOCATION_Y, touch->getLocation().y);
    networkListener->sendEvent(TOUCH_MOVING, eventContent);
}

void MainScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {}

void MainScene::drawLine(int playerId, Vec2 position)
{
    _renderTexture->begin();
    auto drawNode = DrawNode::create();
    drawNode->drawSegment(_previousTouchLocation, position, 10, Color4F(1, 1, 1, 1));
    drawNode->visit();
    _renderTexture->end();
    _previousTouchLocation = position;
}

void MainScene::update(float delta)
{
    networkListener->run();
    switch (networkListener->getState()) {
    case STATE_CONNECTED:
    case STATE_LEFT:
        // TODO: start touch event
        if (networkListener->isRoomExists()) {
            networkListener->setLastInput(NetworkListener::INPUT_JOIN_GAME);
        } else {
            networkListener->setLastInput(NetworkListener::INPUT_CREATE_GAME);
        }
        break;
    default:
        break;
    }

    while (!networkListener->beginEventQueue.empty()) {
        std::array<float, 3>arr = networkListener->beginEventQueue.front();
        networkListener->beginEventQueue.pop();
        _previousTouchLocation =  Vec2(arr[DataType::LOCATION_X], arr[DataType::LOCATION_Y]);
    }

    while (!networkListener->eventQueue.empty()) {
        std::array<float, 3>arr = networkListener->eventQueue.front();
        networkListener->eventQueue.pop();
        drawLine(static_cast<int>(arr[0]), Vec2(arr[DataType::LOCATION_X], arr[DataType::LOCATION_Y]));
    }

    while (!networkListener->tapResetEnventQueue.empty()) {
        networkListener->tapResetEnventQueue.pop();
        cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
        _renderTexture->removeFromParent();
        _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
        _renderTexture->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        addChild(_renderTexture);
    }
}

void MainScene::onTapCloseButton(Ref* pSender)
{
    Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}
