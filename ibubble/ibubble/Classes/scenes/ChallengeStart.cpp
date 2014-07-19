//
//  ChallengeStart.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ChallengeStart.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "GameConfig.h"

NS_KAI_BEGIN
ChallengeStart::ChallengeStart():CCLayer()
{

}

ChallengeStart::~ChallengeStart()
{

}

ChallengeStart* ChallengeStart::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ChallengeStart",ChallengeStartLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	ChallengeStart * node = (ChallengeStart*)ccBReader->readNodeGraphFromFile("ChallengeStart.ccbi");
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

CCScene* ChallengeStart::scene(){
	CCScene *scene = CCScene::create();
	ChallengeStart * node = createWithCCB();
	if(node != NULL){
		scene->addChild(node);
	}
	return scene;
}

void ChallengeStart::press_continue(){
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeGameStart,CCInteger::create(0));
}

void ChallengeStart::press_new(){
    GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeHelpInLevel);
}

void ChallengeStart::press_back(){
    GameController::sharedInstance()->popSence();
    GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,NULL);
}


#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeStart::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_continue",ChallengeStart::press_continue);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_new",ChallengeStart::press_new);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",ChallengeStart::press_back);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler ChallengeStart::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ChallengeStart::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ChallengeStart::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoade){
}
NS_KAI_END

