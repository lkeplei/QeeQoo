//
//  AchievementViewScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_AchievementViewScene_h
#define ibubble_AchievementViewScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
#include "CCSWFNodeExt.h"
USING_NS_CC;

NS_KAI_BEGIN
#pragma mark-
#pragma mark AchievementViewScene

class AchievementViewScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(AchievementViewScene, create);
    virtual ~AchievementViewScene();
    AchievementViewScene();
    static cocos2d::CCScene* scene(CCObject * params);
	static AchievementViewScene* createWithCCB();
	void press_back();
	void press_next();
	void initWithParams(CCObject * params);
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget,
														   CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
	
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget,
										   CCString * pMemberVariableName,
										   CCNode * pNode);
	
	virtual void onNodeLoaded(CCNode * pNode,
							  cocos2d::extension::CCNodeLoader * pNodeLoader);
    
    void handleSwfFinished(cocos2d::CCObject * obj);
    
    virtual void onEnter();
    //触摸事件
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *touch, CCEvent* event);
    
private:
    void playSwf(CCString *filename);
    
private:
    CCSWFNodeSequence *_swf;
	CCNode * _displaySpriteRoot;
    CCSprite * _bgSpriteRoot;
    CCMenuItemImage *_backMenuItem;
    CCArray *swfFileArray;
    int swfIndex;
    bool canTouch;
};

class AchievementViewSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AchievementViewSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AchievementViewScene);
};

NS_KAI_END

#endif

