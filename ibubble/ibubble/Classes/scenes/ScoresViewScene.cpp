//
//  ScoresViewScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ScoresViewScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameObject.h"
#include "UiTool.h"
#include "GameCenter.h"
NS_KAI_BEGIN
ScoresViewScene::ScoresViewScene():CCLayer(),_displaySpriteRoot(NULL)
{
	
}

ScoresViewScene::~ScoresViewScene()
{
	CC_SAFE_RELEASE_NULL(_displaySpriteRoot);
}

ScoresViewScene* ScoresViewScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ScoresViewScene",ScoresViewSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	ScoresViewScene * node =(ScoresViewScene*)ccBReader->readNodeGraphFromFile("scores_layer.ccbi");
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

CCScene* ScoresViewScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void ScoresViewScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME);
}

void ScoresViewScene::press_rank_list()
{
    GameCenter::showScores();
}

bool ScoresViewScene::init(){
	return  true;
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ScoresViewScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",ScoresViewScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_rank_list",ScoresViewScene::press_rank_list);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler ScoresViewScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", ScoresViewScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ScoresViewScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "display_node", CCNode *,_displaySpriteRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ScoresViewScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("ScoresViewScene onNodeLoaded~") ;
}

void ScoresViewScene::onEnter(){
	CCLayer::onEnter();
}

void ScoresViewScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

