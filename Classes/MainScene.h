#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "NetworkListener.h"


USING_NS_CC;

class MainScene : public Layer
{
public:
    CREATE_FUNC(MainScene);
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
    
    std::map<int, Vec2> _previousTouchLocations;
    
    NetworkListener* networkListener;
    
};

#endif // __MainScene_SCENE_H__
