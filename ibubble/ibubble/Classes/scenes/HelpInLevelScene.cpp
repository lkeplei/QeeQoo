//
//  HelpInLevelScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "HelpInLevelScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
NS_KAI_BEGIN
HelpInLevelScene::HelpInLevelScene():CCLayer(),
_helpSpriteRoot(NULL),
_index(0),
_nextButton(NULL)
{
	_list.push_back("help_1.png");
	_list.push_back("help_2.png");
	_list.push_back("help_3.png");
	_list.push_back("help_4.png");
	_list.push_back("help_5.png");
	_list.push_back("help_6.png");
}

HelpInLevelScene::~HelpInLevelScene()
{
	_list.clear();
	CC_SAFE_RELEASE_NULL(_nextButton);
	CC_SAFE_RELEASE_NULL(_helpSpriteRoot);
}

HelpInLevelScene* HelpInLevelScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("HelpInLevelScene",HelpInLevelSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	HelpInLevelScene * node = (HelpInLevelScene*)ccBReader->readNodeGraphFromFile("help_in_level_layer.ccbi");
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

CCScene* HelpInLevelScene::scene(const int zoneid,const int levelid)
{
	CCScene *scene = CCScene::create();
	HelpInLevelScene * node = createWithCCB();
	if(node!=NULL){
		node->_levelid = levelid;
		node->_zoneid = zoneid;
		scene->addChild(node);
	}
	return scene;
}

void HelpInLevelScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SUB_LEVELS_SELECTION,CCInteger::create(_zoneid));
}

void HelpInLevelScene::press_next()
{
	if (_index < (_list.size() - 1)) {
		_index++;
		updateHelper();
	}
	else{
		
		if (!GameController::sharedInstance()->popSence()) {
			GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_PRE,CCInteger::create(_levelid));
		}
		else{
			GameController::sharedInstance()->resumeBattle();
		}
	}

}

void HelpInLevelScene::updateHelper()
{
	if (_helpSpriteRoot) {
		_helpSpriteRoot->removeAllChildrenWithCleanup(true);
		CCSprite * helper = CCSprite::create(_list[_index].c_str());
		_helpSpriteRoot->addChild(helper);
		helper->setPosition(CCPoint(_helpSpriteRoot->getContentSize().width/2, _helpSpriteRoot->getContentSize().height/2));
	}
	
	if (_nextButton) {
		CCSpriteFrameCache * gShareCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		CCSpriteFrame *  unselectedframe = gShareCache->spriteFrameByName("next_button.png");
		CCSpriteFrame *  selectedframe = gShareCache->spriteFrameByName("next_button_sel.png");
		if (_index == _list.size() - 1) {
			 unselectedframe = gShareCache->spriteFrameByName("start_button.png");
			 selectedframe = gShareCache->spriteFrameByName("start_button_sel.png");
		}
		
		_nextButton->setSelectedSpriteFrame(selectedframe);
		_nextButton->setNormalSpriteFrame(unselectedframe);
	}
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler HelpInLevelScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "help_back_last_view",HelpInLevelScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_next",HelpInLevelScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler HelpInLevelScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", HelpInLevelScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool HelpInLevelScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "help_sprite", CCNode *,_helpSpriteRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "next_button", CCMenuItemImage *,_nextButton);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void HelpInLevelScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("HelpInLevelScene onNodeLoaded~") ;
	updateHelper();
}

void HelpInLevelScene::onEnter(){
	CCLayer::onEnter();
}

void HelpInLevelScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

