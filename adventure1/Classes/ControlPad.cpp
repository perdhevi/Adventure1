//
//  ControlPad.cpp
//  adventure1
//
//  Created by Raditya Perdhevi on 8/18/13.
//
//

#include "ControlPad.h"

bool ControlPad::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCDirector *dir = CCDirector::sharedDirector();
//    CCSize winsize = dir->getWinSiz	e(
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("controlPad.plist");
    
    dpad = CCSprite::createWithSpriteFrameName("control.png");
    this->addChild(dpad);
    //Prepare for touch Receive
    dir->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    this->schedule(schedule_selector(ControlPad::updateLogic));
    setDPad(68, 68);
    return true;
};

void ControlPad::setDPad(float x, float y)
{
    dPadCenter.x = x;
    dPadCenter.y = y;
    dpad->cocos2d::CCNode::setPosition(dPadCenter.x, dPadCenter.y );
    dPadRect.setRect(x-48, y-48, 96, 96);
}

void ControlPad::updateLogic(float dt)
{
    if(beingTouched)
    {
        if(relLocation.x > 0.5)
        {
            //right side of the screen
            if(relLocation.y > 0.5)
            {
                //bottom right
                if((relLocation.x -0.5) > relLocation.y-0.5)
                    receiver->starMoveRight();
                else
                    receiver->starMoveUp();
            }else{
                //top right
                if((relLocation.x - 0.5) > 0.5 -(relLocation.y))
                    receiver->starMoveRight();
                else
                    receiver->starMoveDown();
            }
        }else{
            //left side of the screen
            if(relLocation.y > 0.5)
            {
                //bottom left
                if(relLocation.x > 0.5 - (relLocation.y - 0.5))
                    receiver->starMoveUp();
                else
                    receiver->starMoveLeft();
            }else{
                //top left
                if(relLocation.x > relLocation.y)
                    receiver->starMoveDown();
                else
                    receiver->starMoveLeft();
            }
        }
    }
}

bool ControlPad::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCDirector *dir = CCDirector::sharedDirector();
    CCSize win = dir->getWinSize();

    beingTouched = false;
    CCPoint pos = pTouch->getLocationInView();
    pos.y = win.height - pos.y;
    if(dPadRect.containsPoint(pos))
    {
        beingTouched = true;
        relLocation.x = (pos.x-dPadRect.origin.x) / (dPadRect.size.width);
        relLocation.y = (pos.y-dPadRect.origin.y) / (dPadRect.size.height);
    }
    return true;
}

void ControlPad::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCDirector *dir = CCDirector::sharedDirector();
    CCSize win = dir->getWinSize();
    
    CCPoint pos = pTouch->getLocationInView();
    pos.y = win.height - pos.y;
    
    if(dPadRect.containsPoint(pos))
    {
        if(!beingTouched){
            beingTouched = true;
            updateLogic(0);
        }
        relLocation.x = (pos.x-dPadRect.origin.x) / (dPadRect.size.width);
        relLocation.y = (pos.y-dPadRect.origin.y) / (dPadRect.size.height);
        //
    }else{
        beingTouched=false;
        receiver->starStop();
    }
    
}

void ControlPad::ccTouchEnded(CCTouch *pTouch, CCEvent *event)
{
    //if(beingTouched)
    {
        beingTouched = false;
        receiver->starStop();
    }
}