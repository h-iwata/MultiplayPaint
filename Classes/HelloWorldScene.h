#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "NetworkListener.h"


USING_NS_CC;

class HelloWorld : public Layer
{
public:
    CREATE_FUNC(HelloWorld);
    static Scene* createScene();
    virtual bool init();
private:
    
    void showParts();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    virtual void update(float delta);
    
    void drawLine(int playerId, Vec2 position);
    
    void onTapResetButton(Ref* pSender);
    void onTapCloseButton(Ref* pSender);
    
    RenderTexture* _renderTexture;
    Vec2 _previousTouchLocation;
    
    NetworkListener* networkListener;
    
};

#endif // __HELLOWORLD_SCENE_H__
