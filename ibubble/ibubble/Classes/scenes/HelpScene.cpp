//
//  HelpScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "HelpScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
NS_KAI_BEGIN
HelpScene::HelpScene():CCLayer(),_helpSpriteRoot(NULL),_index(0)
{
	_list.push_back("help_1.png");
	_list.push_back("help_2.png");
	_list.push_back("help_3.png");
	_list.push_back("help_4.png");
	_list.push_back("help_5.png");
	_list.push_back("help_6.png");
}

HelpScene::~HelpScene()
{
	_list.clear();
	CC_SAFE_RELEASE_NULL(_helpSpriteRoot);
}

HelpScene* HelpScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("HelpScene",HelpSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	HelpScene * node =(HelpScene*)ccBReader->readNodeGraphFromFile("help_layer.ccbi");
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

CCScene* HelpScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void HelpScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void HelpScene::press_next()
{
	_index++;
	_index = _index%_list.size();
	updateHelper();
}

void HelpScene::updateHelper()
{
	if (_helpSpriteRoot) {
		_helpSpriteRoot->removeAllChildrenWithCleanup(true);
		CCSprite * helper=CCSprite::create(_list[_index].c_str());
		_helpSpriteRoot->addChild(helper);
		helper->setPosition(CCPoint(_helpSpriteRoot->getContentSize().width/2, _helpSpriteRoot->getContentSize().height/2));
	}
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler HelpScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "help_back_last_view",HelpScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_next",HelpScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler HelpScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", HelpScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool HelpScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "help_sprite", CCNode *,_helpSpriteRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void HelpScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("HelpScene onNodeLoaded~") ;
	updateHelper();
}

void HelpScene::onEnter(){
	CCLayer::onEnter();
}

void HelpScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

