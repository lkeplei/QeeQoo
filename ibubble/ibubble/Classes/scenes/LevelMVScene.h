//
//  LevelMVScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_LevelMVScene_h
#define ibubble_LevelMVScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class CCSWFNodeSequence;
NS_KAI_BEGIN

#pragma mark-
#pragma mark LevelMVScene

class LevelMVScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LevelMVScene, create);
    virtual ~LevelMVScene();
    LevelMVScene();
    static cocos2d::CCScene* scene(const int zoneid,
                                   const int levelid,
                                   const std::string & movieKey);
	static LevelMVScene* createWithCCB(const std::string & movieKey);
	void press_back();
	void press_next();
	void updateHelper();
    void handleSwfFinished(cocos2d::CCObject * obj);
    void handleSwfFrameChanged(cocos2d::CCObject * obj);
    void continueSwf();
	virtual void onEnter();
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
	CCNode * _helpSpriteRoot;
	int  _levelid;
	int  _zoneid;
    CCSWFNodeSequence * _swf;
	std::string _movieKey;
};



#pragma mark-
#pragma mark LevelMVSceneLayerLoader
class LevelMVSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LevelMVSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LevelMVScene);
};

NS_KAI_END

#endif

