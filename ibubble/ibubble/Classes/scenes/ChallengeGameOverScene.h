//
//  ChallengeGameOverScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_ChallengeGameOverScene_h
#define ibubble_ChallengeGameOverScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark ChallengeGameOverScene

class ChallengeGameOverScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ChallengeGameOverScene, create);
    virtual ~ChallengeGameOverScene();
    ChallengeGameOverScene();
    static cocos2d::CCScene* scene(bool first=true);
	static ChallengeGameOverScene* createWithCCB();
	void press_menu_page();
	void press_replay();
	void press_play_next();
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
    
    void setStep(bool first);
    
private:
    void getSkill(const int type);
    void show(int type);
    
private:
    CCSprite * _passTitle;
    CCSprite * _failTitle;
    CCSprite * _starTitle;
    CCSprite * _viciTitle;
    CCSprite * _boobeeTitle;
    CCSprite * _roodaTitle;
    CCSprite * _woowooTitle;
        
    CCSprite * _normal_flowers;
    CCSprite * _normal_gou;
    CCSprite * _normal_good;
    CCSprite * _normal_stars;
    CCSprite * _normal_total;
    
    CCNode * _reward_node;
    CCNode * _normal_records_node;
    CCNode * _normal_stars_node;
    CCNode * _normal_total_node;
        
    CCMenuItemImage * _normalMenuBtn;
    CCMenuItemImage * _replayBtn;
    CCMenuItemImage * _normalNextBtn;
    CCMenuItemImage * _hardMenuBtn;
    
    bool _firstStep;
};



#pragma mark-
#pragma mark ChallengeGameOverSceneLayerLoader
class ChallengeGameOverSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ChallengeGameOverSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ChallengeGameOverScene);
};

NS_KAI_END

#endif

