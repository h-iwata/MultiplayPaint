#include "cocos2d.h"
#include "AppDelegate.h"
#include "DemoScene.h"
#include "ViewConstants.h"
#include "../../src/DemoConstants.h"
#include "../../src/LoadBalancingListener.h"

bool useTcp = false;

using namespace cocos2d;
using namespace ExitGames::LoadBalancing;
using namespace ExitGames::Common;
using namespace ExitGames::Photon;

#define LOGIC_TICK_INTERVAL 0.01f

class LogicTick : public CCNode
{
public:
	LogicTick(Client* lbc, LoadBalancingListener* lbl, DemoScene* scene)
		: lbc(lbc)
		, lbl(lbl)
		, scene(scene)
	{
		schedule(schedule_selector(LogicTick::tick), LOGIC_TICK_INTERVAL);
	}
	void tick(float)
	{
		lbc->service();
		lbl->service();
		scene->tick(LOGIC_TICK_INTERVAL);
	}
private:
	Client* lbc;
	LoadBalancingListener* lbl;
	DemoScene* scene;
};

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	LoadBalancingListener* lbl = NULL;
	Client* lbc = NULL;

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

	lbl = new LoadBalancingListener(scene);
	lbc = new Client(*lbl, appId, appVersion, JString(L"NV")+GETTIMEMS(), useTcp, AuthenticationValues(), false);
	lbc->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
//	lbc->setCrcEnabled(true);
	lbl->setLBC(lbc);
	lbc->setTrafficStatsEnabled(true);
//	lbc->setLimitOfUnreliableCommands(15);
	scene->setLBC(lbc, lbl);
	scene->setupUI();

	scene->info( "LBC: connecting...");
	lbl->connect();
	LogicTick* logicTick = new LogicTick(lbc, lbl, scene);
	rootScene->addChild(logicTick);
	// run
	pDirector->runWithScene(rootScene);

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