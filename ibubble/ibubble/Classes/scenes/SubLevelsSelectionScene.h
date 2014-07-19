//
//  SubLevelsSelectionScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_SubLevelsSelectionScene_h
#define ibubble_SubLevelsSelectionScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark SubLevelsSelectionScene

class SubLevelsSelectionScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SubLevelsSelectionScene, create);
    virtual ~SubLevelsSelectionScene();
    SubLevelsSelectionScene();
    static cocos2d::CCScene* scene();
	static SubLevelsSelectionScene* createWithCCB();
	void press_back(CCObject *pSender, CCControlEvent pCCControlEvent);
	void press_play(CCObject *pSender, CCControlEvent pCCControlEvent);
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
	CCMenu * _levelsMenu;
	CCArray *_levelsList;
	CCNode   * _gouNodeRoot;
	CCNode   * _starNodeRoot;
};



#pragma mark-
#pragma mark SubLevelsSelectionSceneLayerLoader
class SubLevelsSelectionSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SubLevelsSelectionSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SubLevelsSelectionScene);
};

NS_KAI_END

#endif

