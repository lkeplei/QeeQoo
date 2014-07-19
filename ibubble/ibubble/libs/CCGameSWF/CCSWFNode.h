//
//  CCSWFNode.h
//  GSGGameSWF
//
//  Created by dario on 13-02-25.
//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//  Modify by Ryan_Y(yuankaii.cn@gmail.com) on 13-11-04
//  fixed some memory leak issues and reduce memory useage.

#ifndef CCSWF_NODE
#define CCSWF_NODE

#import "cocos2d.h"
#import "CCTouchDelegateProtocol.h"
#include "cocos2d.h"
#include "gameswf.h"
#include "CCSWFNodeGroup.h"

using namespace cocos2d;
class CCSWFNode_imp;
class CCSWFNodeGroup;

class CCSWFNode : public cocos2d::CCSprite,public cocos2d::CCTouchDelegate
{
public:
    typedef cocos2d::CCSprite super;
public:
    CCSWFNode();
    ~CCSWFNode();

    void setGrouped(bool flag){ isGrouped = flag;};
    bool getGrouped(){ return isGrouped;};
    
    CCString* movieName();
    void setMovieName(char *movieName);
    static CCSWFNode* create(const char* file);
    bool initWithSWFFile(const char* file);

    bool Runing(){return isRuning;};
    void addCompletedListener(CCObject* target,SEL_MenuHandler selector);
    void addFrameListener(CCObject* target,SEL_MenuHandler selector);
    void setNextAction(CCSWFNode* target);

    void setRepeat(bool flag);
    bool getRepeat();
    const int getCurrentFrame();
    const int getFrameCount();
public:
    void setFlipX(bool flipX);
    void setFlipY(bool flipY);
    
    float scale();
    void setScale(float scale);
    float scaleX();
    void setScaleX(float scaleX);
    float scaleY();
    void setScaleY(float scaleY);
    
    void update(float dt);
    void draw();
    
    virtual void stopAction();
    virtual void runAction();
    virtual void resumeAction();
    void onEnter();
    void onExit();
public:
    const CCPoint getTouchInMovieCoordinates(CCTouch *pTouch);
    //CCTouchDelegate
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) ;
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
private:
    bool isGrouped;
    bool isRuning;
    CCSWFNode_imp *m_imp;
    GLfloat m_movieWidth;
    GLfloat m_movieHeight;
    GLfloat m_localScaleX;
    GLfloat m_localScaleY;
    GLfloat m_scaleX;
    GLfloat m_scaleY;
    CCArray* m_touchEvents;
    CCObject* m_listener;
    SEL_MenuHandler    m_pfnSelector;
    CCObject* m_endListener;
    SEL_MenuHandler m_pfnEndSelector;
    bool isRepeat;
    int  iFrameCount;
    int  iCurrentFrame;

};

#endif
