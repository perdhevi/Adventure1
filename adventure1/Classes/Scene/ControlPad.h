//
//  ControlPad.h
//  adventure1
//
//  Created by Raditya Perdhevi on 8/18/13.
//
//

#ifndef __adventure1__ControlPad__
#define __adventure1__ControlPad__

enum {MODE_DPAD, MODE_FULLSCREEN};

#include <iostream>
#include "cocos2d.h"
#include "AdventureScene.h"

using namespace cocos2d;

class ControlPad : public CCLayer
{
public:
    CREATE_FUNC(ControlPad);
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *event);

    AdventureScene *receiver;
    virtual void updateLogic(float dt);
    bool beingTouched;
    CCPoint relLocation;
    CCPoint dPadCenter;
    CCSprite *dpad;
    CCRect dPadRect;
    int touchMode;
    
    void setDPad(float x, float y);
};

#endif /* defined(__adventure1__ControlPad__) */