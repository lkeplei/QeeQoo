//
//  AboutScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "AboutScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
NS_KAI_BEGIN
AboutScene::AboutScene():CCLayer()
, showMenu(NULL)
, showDanDan(false)
{
	
}

AboutScene::~AboutScene()
{
    CC_SAFE_RELEASE_NULL(showMenu);
}

AboutScene* AboutScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("AboutScene",AboutSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	AboutScene * node =(AboutScene*)ccBReader->readNodeGraphFromFile("about_layer.ccbi");
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

CCScene* AboutScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void AboutScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SETTING,CCInteger::create(0));
}

void AboutScene::press_show() {
    showMenu->setVisible(true);
    
    if (showDanDan) {
        showMenu->runAction(CCFadeOut::create(1));
    } else {
        showMenu->runAction(CCFadeIn::create(1));
    }
    
    showDanDan = !showDanDan;
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler AboutScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",AboutScene::press_back);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_show",AboutScene::press_show);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler AboutScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", AboutScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool AboutScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "showMenu", CCSprite *, showMenu);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void AboutScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("AboutScene onNodeLoaded~") ;
}

void AboutScene::onEnter(){
	CCLayer::onEnter();
}

void AboutScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

