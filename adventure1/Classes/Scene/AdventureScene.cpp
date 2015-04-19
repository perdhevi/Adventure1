//
//  AdventureScene.cpp
//  adventure1
//
//  Created by Raditya Perdhevi on 8/11/13.
//
//

#include "AdventureScene.h"
#include "ControlPad.h"
#include "SimpleAudioEngine.h"
#include "string.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* AdventureScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = AdventureScene::scene("Scene1_2.tmx", NULL);
    return scene;
}

CCScene* AdventureScene::scene(const char *name, const char *point)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    ControlPad *ctrl;
    ctrl = ControlPad::create();
    // 'layer' is an autorelease object
    AdventureScene *layer = AdventureScene::create();
    if(point == NULL)
        layer->loadMap(name,"");
    else
        layer->loadMap(name, point);

    // add layer as a child to scene
    ctrl->receiver = layer;

    
    scene->addChild(layer);
    scene->addChild(ctrl);
    // return the scene
    return scene;
}
int AdventureScene::unloadMap()
{
    map->release();
    return 0;
}
int AdventureScene::loadMap(const char *name, const char*startPoint)
{
    //if(map)
    //    map->release();
    
    map = CCTMXTiledMap::create(name);
    CCSize mapSize = map->getMapSize();
    CCSize tileSize = map->getTileSize();
    map->setZOrder(0);
    this->addChild(map);
    
    meta_layer = map->layerNamed("walkable");
    meta_layer->setVisible(false);
    
    CCTMXObjectGroup *obj = map->objectGroupNamed("StartPoints");
    if(obj == NULL){
    
        starPoint.x =  ((mapSize.width * tileSize.width)/2);
        starPoint.y =  ((mapSize.height * tileSize.height)/2);
    }else{
        CCDictionary *start;
        if(strcmp(startPoint,"")==0)
            start = obj->objectNamed("StartPoint1");
        else
            start = obj->objectNamed(startPoint);
        
        int x = ((CCString)*start->valueForKey("x")).intValue();
        int y = ((CCString)*start->valueForKey("y")).intValue();
        
        starPoint.x = x;
        starPoint.y = y;
    }
    centerOnStar();
    return 1;
}




// on "init" you need to initialize your instance
bool AdventureScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    starPoint.x = 0;
	starPoint.y = 0;
    curDirection = 0;

    sceneState = SCENE_INITIALIZING;
    return true;
}

void AdventureScene::onEnterTransitionDidFinish()
{
    batchNode = CCSpriteBatchNode::create("walk.png");
    this->addChild(batchNode);
    spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteCache->addSpriteFramesWithFile("walk.plist");
    
    sprite = CCSprite::createWithSpriteFrameName("walk_1_0.png");
    

    sprite->setPosition(starPoint);
    sprite->setZOrder(1);
    this->addChild(sprite);
    starDirection.x = 0;
    starDirection.y = 0;
    
    CCPoint pos;
    sprite->setPosition(starPoint);
    //Prepare for touch Receive
    //dir->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    this->schedule(schedule_selector(AdventureScene::updateLogic));
    
    walking = false;
    sceneState = SCENE_RUNNING;
    
    //Animation Creation
    for(int j=0;j<4;j++)
    {
        walk_animation[j] = CCAnimation::create();
        walk_animation[j]->setDelayPerUnit(0.05f);
        char szFileName[128] = {0};
        for(int i=0;i<MAX_WALK_FRAME;i++)
        {
            sprintf(szFileName,"walk_%d_%d.png",j, i);
            walk_animation[j]->addSpriteFrame(spriteCache->spriteFrameByName(szFileName));
            
        }
        walk_animation[j]->setRestoreOriginalFrame(true);
        walk_animation[j]->setLoops(INFINITY);

        walk_animation[j]->retain();
    }
}

void AdventureScene::onExit()
{
    starStop();
    //this->stopAllActions();
    sceneState = SCENE_EXITED;
    this->unscheduleAllSelectors();
    for(int i;i<4;i++) walk_animation[i]->release();
    spriteCache->purgeSharedSpriteFrameCache();// removeSpriteFramesFromFile("walk.plist");
    CCLayer::onExit();
}

void AdventureScene::checkSwitchMap()
{
    CCTMXObjectGroup *obj = map->objectGroupNamed("ExitPoints");
    
    char *szName = NULL;
    char *szPoint = NULL;
    if(obj != NULL){
        CCArray *exits = obj->getObjects();
        CCObject *pObj;
        //CCARRAY_FOREACH(exits, pObj)
        int i=0;
        for(i=0;i<exits->count()-1;i++);
        {
            pObj = exits->objectAtIndex(i);
            CCDictionary *dict = (CCDictionary *)pObj;
            int x, y, width, height;
            x = ((CCString *)dict->objectForKey("x"))->intValue();
            y = ((CCString *)dict->objectForKey("y"))->intValue();
            width = ((CCString *)dict->objectForKey("width"))->intValue();
            height = ((CCString *)dict->objectForKey("height"))->intValue();
            CCRect rect = CCRect(x, y, width, height);
            if(rect.containsPoint(starPoint))
            {
                CCString *name;
                name = ((CCString *)dict->objectForKey("type"));
                //const char *total = name->getCString();
                char *text = (char *)malloc(name->length());
                strcpy(text, name->getCString());
                int j=0;
                while((text[j] != ',')&&(j<name->length()))
                {
                    j++;
                }
                szName = (char*)malloc(j+1);
                memset(szName, 0, j+1);
                for(int k=0;k<j;k++)
                {
                    szName[k] = text[k];
                }
                //szname[k] = NULL;
                j++;
                if(j<name->length())
                {
                    szPoint = (char*)malloc(name->length()-j+1);
                    memset(szPoint,0,name->length()-j+1);
                    for(int k=0;k<name->length()-j;k++)
                    {
                        szPoint[k] = text[j+k];
                    }
                }
                
                free(text);

            }
        }
    }
    if((szName != NULL)&&(!(strcmp(szName,"")==0)))
    {
        CCScene *scene = AdventureScene::scene(szName, szPoint);
        CCDirector *dir = CCDirector::sharedDirector();
        
        dir->replaceScene(scene);
    }
    if(szName != NULL)
        free(szName);
    if(szPoint != NULL)
        free(szPoint);
}


void AdventureScene::updateLogic(float dt)
{
    //Moving Star
    CCPoint nextLoc  =  starPoint + starDirection;
    
    CCPoint mapLoc;
    CCPoint curMapLoc;
    CCSize tileSize = map->getTileSize();
    CCSize mapSize = map->getMapSize();
    mapLoc.x = (nextLoc.x / tileSize.width);
    mapLoc.y = round(((tileSize.height * mapSize.height)- nextLoc.y) / tileSize.height);
    curMapLoc.x = (starPoint.x / tileSize.width);
    curMapLoc.y = round(((tileSize.height * mapSize.height)- starPoint.y) / tileSize.height);
    if(!mapLoc.equals(curMapLoc))
    {
        if((mapLoc.x<0)||(mapLoc.x>mapSize.width-1)||(mapLoc.y<0)||(mapLoc.y>mapSize.height-1))
        {
            starDirection.x = 0 ;
            starDirection.y = 0 ;
            starStop();
        }else
        {
            int tileGID = meta_layer->tileGIDAt(mapLoc);
            if(tileGID){
                CCDictionary *dict = map->propertiesForGID(tileGID);
                if(dict){
                    CCString *collide = new CCString();
                    *collide = *dict->valueForKey("blocked");
                    if(collide && (collide->compare("True")==0))
                    {
                        starDirection.x = 0 ;
                        starDirection.y = 0 ;
                        starStop();
                    }
                
                }
            }
        }
    }
    starPoint =  starPoint + starDirection;
    CCPoint center;
    center = this->getPosition();
    center = center - starDirection;
    this->setPosition(center);
    starDirection.x = 0 ;
    starDirection.y = 0 ;
    
    sprite->setPosition(starPoint);

    
    checkSwitchMap();
}

void AdventureScene::centerOnStar()
{
    //Center on starPoint
    CCDirector *dir = CCDirector::sharedDirector();
    CCSize ScreenSize = dir->getWinSize();
    
    CCPoint center;
    center.x =  (ScreenSize.width/2) - starPoint.x ;
    center.y =  (ScreenSize.height/2) -starPoint.y;
    
    
    this->setPosition(center);

}

void AdventureScene::setAnimationWalk(int direction)
{

    if((direction != curDirection)||(!walking))
    {
        sprite->stopAllActions();
        animate = CCAnimate::create(walk_animation[direction]);
        sprite->runAction(animate);
        curDirection=direction;
        walking = true;
    }
}


void AdventureScene::starMoveLeft()
{
    setAnimationWalk(DIR_LEFT);
    starDirection.x = -MOVEMENT_SPEED;
}
void AdventureScene::starMoveRight()
{
    setAnimationWalk(DIR_RIGHT);
    starDirection.x = MOVEMENT_SPEED;
}
void AdventureScene::starMoveUp()
{
    setAnimationWalk(DIR_UP);
    starDirection.y = MOVEMENT_SPEED;
}
void AdventureScene::starMoveDown()
{
    setAnimationWalk(DIR_DOWN);
    starDirection.y = -MOVEMENT_SPEED;
}

void AdventureScene::starStop()
{
    if((sceneState == SCENE_RUNNING)&&(walking))
    {
        sprite->stopAllActions();
        char szFileName[128] = {0};
        sprintf(szFileName,"walk_%d_%d.png",curDirection, 0);
        sprite->setDisplayFrame(spriteCache->spriteFrameByName(szFileName));
        walking = false;
    }
}
