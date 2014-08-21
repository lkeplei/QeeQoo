//
//  ChallengeStart.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_ChallengeStart_h
#define ibubble_ChallengeStart_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark 挑战模式选择继续或重新开始对话框
class ChallengeStart : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ChallengeStart, create);
    virtual ~ChallengeStart();
    ChallengeStart();
    static cocos2d::CCScene* scene();
	static ChallengeStart* createWithCCB();
#pragma mark-
#pragma mark 继续上次打到的关卡
	void press_continue();
#pragma mark-
#pragma mark 重新开始关卡
    void press_new();
#pragma mark-
#pragma mark 返回
    void press_back();
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
	CCMenuItemImage *_continue_menu;
};

#pragma mark-
#pragma mark ChallengeStartLayerLoader
class ChallengeStartLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ChallengeStartLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ChallengeStart);
};

NS_KAI_END

#endif

