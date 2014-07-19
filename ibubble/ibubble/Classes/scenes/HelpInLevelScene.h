//
//  HelpInLevelScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_HelpInLevelScene_h
#define ibubble_HelpInLevelScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark HelpInLevelScene

class HelpInLevelScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(HelpInLevelScene, create);
    virtual ~HelpInLevelScene();
    HelpInLevelScene();
    static cocos2d::CCScene* scene(const int zoneid,const int levelid);
	static HelpInLevelScene* createWithCCB();
	void press_back();
	void press_next();
	void updateHelper();
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
	CCMenuItemImage * _nextButton;
	CCNode * _helpSpriteRoot;
	int32_t    _index;
	std::vector<std::string> _list;
	int  _levelid;
	int  _zoneid;
	
};



#pragma mark-
#pragma mark HelpInLevelSceneLayerLoader
class HelpInLevelSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(HelpInLevelSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(HelpInLevelScene);
};

NS_KAI_END

#endif

