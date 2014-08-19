//
//  BattleScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-11.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//
#ifndef __ibubble_BattleScene_H__
#define __ibubble_BattleScene_H__
#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN
class GameConfig;
class GameModle;

class BattleBaseLayer : public cocos2d::CCLayer
{
public:
	virtual void load(const cocos2d::CCDictionary * dict)=0;
	virtual void updateLayer()=0;
};

/**游戏界面内的背景层*/
class BattleBgLayer : public BattleBaseLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener
,public cocos2d::extension::CCBAnimationManagerDelegate
{
public:
	BattleBgLayer();
    virtual ~BattleBgLayer();
public:
	virtual void load(const cocos2d::CCDictionary * dict);
	virtual void updateLayer();
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BattleBgLayer, create);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
#pragma mark-
#pragma mark CCBAnimationManagerDelegate
	virtual void completedAnimationSequenceNamed(const char *name);
protected:
	cocos2d::extension::CCBAnimationManager *_animationManager;
};

class BattleBgLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BattleBgLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BattleBgLayer);
	
};


/**游戏界面内的游戏对象层,所有游戏对象存在于该层*/
class BattleGameObjectLayer : public BattleBaseLayer {
public:
	BattleGameObjectLayer();
    virtual ~BattleGameObjectLayer();
public:
	virtual void load(const cocos2d::CCDictionary * dict);
	virtual void updateLayer();
	void draw();
private:
	GameConfig* _configInstance;
	GameModle * _modleInstance;
};

/**游戏界面内的控制层:按钮,数值信息等在此层显示*/
#pragma mark-
#pragma mark BattleControllerLayer
class BattleControllerLayer :public BattleBaseLayer 
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener
{
public:
	BattleControllerLayer();
    virtual ~BattleControllerLayer();
public:
	virtual void load(const cocos2d::CCDictionary * dict);
	virtual void updateLayer();
	
	void press_back();
	void press_pause();
	
	void press_restart();
	
	void press_play();
	
	void press_home();

	void press_condition();
	
	void press_setting();
	
	void press_help();

	void press_skill1();
	void press_skill2();
	void press_skill3();
	void press_skill4();
	void press_skill5();
	void press_skill6();
    void skillUpdated(CCObject * params);
private:
	void hideMenu(bool animation = false);
	void showMenu(bool animation = false);
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BattleControllerLayer, create);
#pragma mark-
#pragma mark BattleControllerLayer CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
#pragma mark-
#pragma mark BattleControllerLayer CCBMemberVariableAssigner
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
#pragma mark-
#pragma mark BattleControllerLayer CCBNodeLoaderListener
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
private:
    CCNode * _cur_kill_node;
    CCSprite *_cur_kill_sprite;
	CCLayer * _cur_menu_node;
	CCPoint		_startPos;
	CCMenuItemImage * _pause_button;
	CCMenuItemImage * _back_button;
    
	CCMenuItemImage * _skill1Button;
	CCMenuItemImage * _skill2Button;
	CCMenuItemImage * _skill3Button;
	CCMenuItemImage * _skill4Button;
	CCMenuItemImage * _skill5Button;
	CCMenuItemImage * _skill6Button;
	
    CCNode * _ChallengeRoot;
    CCMenu * _ChallengeMenu;
    
    CCNode * _skill1text;
	CCNode * _skill2text;
	CCNode * _skill3text;
	CCNode * _skill4text;
	CCNode * _skill5text;
	CCNode * _skill6text;
};


#pragma mark-
#pragma mark BattleControllerLayerLoader
class BattleControllerLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BattleControllerLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BattleControllerLayer);
};



/**战斗场景,MVC中的View*/
class BattleScene : public cocos2d::CCLayer {
public:
    ~BattleScene();
    BattleScene();
    static cocos2d::CCScene* scene();
	void load();
	void modleUpdated();
	virtual void onEnter();
    virtual void onExit();
	//触摸事件
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
private:
	cocos2d::CCArray * _layers;
	GameConfig* _configInstance;
	GameModle * _modleInstance;
};
NS_KAI_END
#endif // __ibubble_BattleScene_H__
