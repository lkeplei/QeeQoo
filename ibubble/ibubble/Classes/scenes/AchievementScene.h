//
//  AchievementScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_AchievementScene_h
#define ibubble_AchievementScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark AchievementScene

class AchievementScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(AchievementScene, create);
    virtual ~AchievementScene();
    AchievementScene();
    static cocos2d::CCScene* scene();
	static AchievementScene* createWithCCB();
	void press_back(CCObject *pSender, CCControlEvent pCCControlEvent);
	void press_next(CCObject *pSender, CCControlEvent pCCControlEvent);
	virtual void onEnter();
    virtual void onExit();
public:
#pragma mark-
#pragma mark CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
#pragma mark-
#pragma mark CCBMemberVariableAssigner
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
#pragma mark-
#pragma mark CCBNodeLoaderListener
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
private:
    const char *getSpriteNameFromTag(int tag);
};



#pragma mark-
#pragma mark AchievementSceneLayerLoader
class AchievementSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(AchievementSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(AchievementScene);
};

NS_KAI_END

#endif

