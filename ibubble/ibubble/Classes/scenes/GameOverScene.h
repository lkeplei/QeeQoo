//
//  GameOverScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameOverScene_h
#define ibubble_GameOverScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark GameOverScene

class GameOverScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameOverScene, create);
    virtual ~GameOverScene();
    GameOverScene();
    static cocos2d::CCScene* scene(bool gameOver=false);
	static GameOverScene* createWithCCB();
	void press_menu_page();
	void press_replay();
	void press_play_next();
	
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
    
    void updateScene();
    
private:
	CCNode * _result_scores;
	CCSprite * _result_icon;
	CCSprite * _achievement_icon;
	CCMenuItem * _play_next;
    CCMenuItem * _replay_menu;
    CCMenuItem * _home_menu;
	int32_t   _currentLevelId;
	int32_t   _nextLevelId;
public:
	bool _gameOver;
};



#pragma mark-
#pragma mark GameOverSceneLayerLoader
class GameOverSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameOverSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameOverScene);
};

NS_KAI_END

#endif

