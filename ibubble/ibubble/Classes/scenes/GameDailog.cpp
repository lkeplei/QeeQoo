//
//  GameDailog.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameDailog.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
NS_KAI_BEGIN
GameDailog::GameDailog():CCLayer(),_content(NULL)
{
	
}

GameDailog::~GameDailog()
{

}

GameDailog* GameDailog::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("GameDailog",GameDailogLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	GameDailog * node =(GameDailog*)ccBReader->readNodeGraphFromFile("game_dailog.ccbi");
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

CCScene* GameDailog::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler GameDailog::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler GameDailog::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", GameDailog::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool GameDailog::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "dialog_content_node", CCNode *, this->_content);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void GameDailog::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("GameDailog onNodeLoaded~") ;
}
NS_KAI_END

