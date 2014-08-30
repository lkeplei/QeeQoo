//
//  ChallengeHelpInLevelScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ChallengeHelpInLevelScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
ChallengeHelpInLevelScene::ChallengeHelpInLevelScene()
: CCLayer()
{

}

ChallengeHelpInLevelScene::~ChallengeHelpInLevelScene()
{
    
}

ChallengeHelpInLevelScene* ChallengeHelpInLevelScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ChallengeHelpInLevelScene",ChallengeHelpInLevelSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	ChallengeHelpInLevelScene * node = (ChallengeHelpInLevelScene*)ccBReader->readNodeGraphFromFile("ChallengeHelpInLevelScene.ccbi");
	if (node) {
		//node->retain();
		CC_SAFE_RELEASE_NULL(ccBReader);
		node->autorelease();
	}
	else {
		CC_SAFE_RELEASE_NULL(ccBReader);
	}
	return node;
}

CCScene* ChallengeHelpInLevelScene::scene(bool onlyHelp)
{
	CCScene *scene = CCScene::create();
	ChallengeHelpInLevelScene * node = createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
        node->isOnlyHelp = onlyHelp;
	}
	return scene;
}

void ChallengeHelpInLevelScene::press_next(){
    //冲第一关卡开始
    if (isOnlyHelp) {
        GameController::sharedInstance()->popSence();
        GameController::sharedInstance()->resumeBattle();
//        GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,
//                                                      CCInteger::create(GameUtilities::getAchieveLevelId()));
    } else {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeGameStart,CCInteger::create(0));
    }
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeHelpInLevelScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_next",ChallengeHelpInLevelScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler ChallengeHelpInLevelScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ChallengeHelpInLevelScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "help_sprite", CCNode *,_helpSpriteRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ChallengeHelpInLevelScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
}

NS_KAI_END

