#include "cocos2d.h"

#include "Chat-cpp/inc/Listener.h"
#include "../inc/AppDelegate.h"
#include "../inc/DemoScene.h"
#include "../inc/DemoConstants.h"
#include "../inc/ViewConstants.h"

bool useTcp = false;

using namespace cocos2d;
using namespace ExitGames::Chat;
using namespace ExitGames::Common;
using namespace ExitGames::Photon;

#define SERVICE_TICK_INTERVAL 0.01f

class ServiceTick : public CCNode
{
public:
	ServiceTick(Client* client)
		: client(client)
	{
		schedule(schedule_selector(ServiceTick::tick), SERVICE_TICK_INTERVAL);
	}
	void tick(float)
	{
		client->service();
	}
private:
	Client* client;
};

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	Client* client = NULL;

	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	if(screenSize.width < 1) // 0 for desktop - set to default values
		CCEGLView::sharedOpenGLView()->setFrameSize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	else // set to device resolution
		CCEGLView::sharedOpenGLView()->setFrameSize(screenSize.width, screenSize.height);

	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	// sets landscape mode
#ifndef CC_PLATFORM_MARMALADE
	pDirector->setDeviceOrientation(kCCDeviceOrientationLandscapeLeft);
#endif

	// turn on display FPS
//	pDirector->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	pDirector->setAnimationInterval(1.0 / 60);

	// create a scene. it's an autorelease object
	CCScene* rootScene = CCScene::create();
	DemoScene *scene = DemoScene::create();
	rootScene->addChild(scene);

	client = new Client(*scene, APP_ID, APP_VERSION, DEFAULT_USER_ID);
	client->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
	scene->setClient(client);
	scene->setupUI();

//	scene->log( "[i]: Connecting...");
//	client->connect();
	ServiceTick* serviceTick = new ServiceTick(client);
	rootScene->addChild(serviceTick);
	// run
	pDirector->runWithScene(rootScene);

	// trigger start hint
	scene->onStateChange(client->getState());

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
	CCDirector::sharedDirector()->pause();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->resume();
}
