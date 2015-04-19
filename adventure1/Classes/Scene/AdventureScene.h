//
//  AdventureScene.h
//  adventure1
//
//  Created by Raditya Perdhevi on 8/11/13.
//
//

#ifndef adventure1_AdventureScene_h
#define adventure1_AdventureScene_h

#include "cocos2d.h"
#include "Actor.h"

enum  {
    DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT
};

#define SCENE_STARTED 0
#define SCENE_INITIALIZING 1
#define SCENE_RUNNING 2
#define SCENE_EXITED 3


#define MAX_WALK_FRAME 9
#define MOVEMENT_SPEED 2

using namespace cocos2d;

class AdventureScene : public CCLayer
{
protected:
    Actor *hero;
public:
    //Map Related things
    CCTMXTiledMap *map;
    CCTMXLayer *meta_layer;
    
    //Sprites
    CCSpriteBatchNode *batchNode;
    CCSpriteFrameCache *spriteCache;
    CCSprite *sprite;

    CCAnimate *animate;
    CCAnimation *walk_animation[4];
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    CCPoint starPoint;
    CCPoint starDirection;
    bool walking;
    int curDirection;
    int sceneState;
    const char *mapName;
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    static CCScene* scene(const char *name, const char *point);

    CREATE_FUNC(AdventureScene);
    
    virtual bool init();
    virtual void updateLogic(float dt);
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();

    virtual int unloadMap();
    virtual int loadMap(const char *name, const char*startPoint);
    virtual void checkSwitchMap();
    virtual void centerOnStar();
    
    void starMoveLeft();
    void starMoveRight();
    void starMoveUp();
    void starMoveDown();
    void starStop();
    
    void setAnimationWalk(int direction);
    
};

#endif
