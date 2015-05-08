#include "cocos2d.h"
#include "AppDelegate.h"
#include "View.h"
#include "ViewConstants.h"
#include "../../src/LoadBalancingListener.h"
#include "../../src/Game.h"
#include "../../src/DemoConstants.h"

using namespace cocos2d;
#define LOGIC_TICK_INTERVAL 0.01f

class LogicTick : public CCNode
{
public:
	LogicTick(ExitGames::LoadBalancing::Client* lbc, LoadBalancingListener* lbl)
		: lbc(lbc)
		, lbl(lbl)
	{
		schedule(schedule_selector(LogicTick::tick), LOGIC_TICK_INTERVAL);
	}
	void tick(float)
	{
		lbc->service();
		lbl->service();
	}
private:
	ExitGames::LoadBalancing::Client* lbc;
	LoadBalancingListener* lbl;
};

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
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

	CCScene* rootScene = CCScene::create();
	View *view = View::create();
	rootScene->addChild(view);

	LoadBalancingListener* lbl = new LoadBalancingListener();
	ExitGames::LoadBalancing::Client* lbc = new ExitGames::LoadBalancing::Client(*lbl, APP_ID, APP_VERSION, "");
	lbc->setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
	ExitGames::Common::Base::setListener(lbl);
	ExitGames::Common::Base::setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO);
	view->init(lbl);
	view->setup();
	lbl->init(lbc, view);


	LogicTick* logicTick = new LogicTick(lbc, lbl);
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