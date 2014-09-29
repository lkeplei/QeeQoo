//
//  StartupScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "StartupScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"


@interface KenConfirmAlert : NSObject<UIAlertViewDelegate>

- (void)showAlert:(NSString *)title;

@end

@implementation KenConfirmAlert

- (void)showAlert:(NSString *)title {
    [[[UIAlertView alloc] initWithTitle:nil
                                message:title
                               delegate:self
                      cancelButtonTitle:nil
                      otherButtonTitles:KenLocal(@"app_confirm"), nil] show];
}

@end





NS_KAI_BEGIN
StartupScene::StartupScene():CCLayer()
,_logo(NULL)
, _achieve_lock_sprite(NULL)
,_pressJuqinBTN(NULL)
,_pressJiangliBTN(NULL)
,_pressTiaozhanBTN(NULL)
,_pressSettingBTN(NULL)
,_pressHelpBTN(NULL)
,_pressChengjiBTN(NULL)
,_pressJuqin(NULL)
,_pressJiangli(NULL)
,_pressTiaozhan(NULL)
,_pressSetting(NULL)
,_pressHelp(NULL)
,_pressChengji(NULL)
{

}

StartupScene::~StartupScene()
{
	CC_SAFE_RELEASE_NULL(_logo);
    CC_SAFE_RELEASE_NULL(_achieve_lock_sprite);
	
	CC_SAFE_RELEASE_NULL(_pressJuqinBTN);
	CC_SAFE_RELEASE_NULL(_pressJiangliBTN);
	CC_SAFE_RELEASE_NULL(_pressTiaozhanBTN);
	CC_SAFE_RELEASE_NULL(_pressSettingBTN);
	CC_SAFE_RELEASE_NULL(_pressHelpBTN);
	CC_SAFE_RELEASE_NULL(_pressChengjiBTN);
	
	CC_SAFE_RELEASE_NULL(_pressJuqin);
	CC_SAFE_RELEASE_NULL(_pressJiangli);
	CC_SAFE_RELEASE_NULL(_pressTiaozhan);
	CC_SAFE_RELEASE_NULL(_pressSetting);
	CC_SAFE_RELEASE_NULL(_pressHelp);
	CC_SAFE_RELEASE_NULL(_pressChengji);
}

StartupScene* StartupScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("StartupScene",StartupSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	StartupScene * node =(StartupScene*)ccBReader->readNodeGraphFromFile("startup_layer.ccbi");
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

CCScene* StartupScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void StartupScene::press_juqin()
{
	CCLog("StartupScene::press_juqin()");
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_LEVELS_SELECTION,CCInteger::create(0));
}

void StartupScene::press_jiangli()
{
	CCLog("StartupScene::press_jiangli()");
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_ACHIEVEMENT);
}

void StartupScene::press_tiaozhan()
{
	CCLog("StartupScene::press_tiaozhan()");
    
    if (GameUtilities::getUnlockWithId(10006)) {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeStart);
    } else {
        KenConfirmAlert *alert = [[KenConfirmAlert alloc] init];
        [alert showAlert:KenLocal(@"lock_achieve")];
    }
}

void StartupScene::press_setting()
{
	CCLog("StartupScene::press_setting()");
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SETTING);
}

void StartupScene::press_help()
{
	CCLog("StartupScene::press_help()");
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HELP);
}



void StartupScene::press_chengji()
{
	CCLog("StartupScene::press_chengji()");
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SCORES);
}


void StartupScene::loadImagesCallback(cocos2d::CCObject * obj){
	CCLog("StartupScene::loadImagesCallback()");
}

void StartupScene::handleMenuItemSelected(cocos2d::CCObject * obj){
	if (obj == _pressJuqin) {
		_pressJuqinBTN->selectedShow();
	}
	else if (obj == _pressJiangli) {
		_pressJiangliBTN->selectedShow();
	}
	else if (obj == _pressTiaozhan) {
		_pressTiaozhanBTN->selectedShow();
	}
	else if (obj == _pressSetting) {
		_pressSettingBTN->selectedShow();
	}
	else if (obj == _pressHelp) {
		_pressHelpBTN->selectedShow();
	}
	else if (obj == _pressChengji) {
		_pressChengjiBTN->selectedShow();
	}
	
}

void StartupScene::handleMenuItemUnSelected(cocos2d::CCObject * obj){
	if (obj == _pressJuqin) {
		_pressJuqinBTN->unselectedShow();
	}
	else if (obj == _pressJiangli) {
		_pressJiangliBTN->unselectedShow();
	}
	else if (obj == _pressTiaozhan) {
		_pressTiaozhanBTN->unselectedShow();
	}
	else if (obj == _pressSetting) {
		_pressSettingBTN->unselectedShow();
	}
	else if (obj == _pressHelp) {
		_pressHelpBTN->unselectedShow();
	}
	else if (obj == _pressChengji) {
		_pressChengjiBTN->unselectedShow();
	}
}


#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler StartupScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_juqin",StartupScene::press_juqin);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_jiangli",StartupScene::press_jiangli);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_tiaozhan",StartupScene::press_tiaozhan);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_setting",StartupScene::press_setting);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_help",StartupScene::press_help);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_chengji",StartupScene::press_chengji);
	return NULL;

}

cocos2d::extension::SEL_CCControlHandler StartupScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", StartupScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool StartupScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "logo", CCSprite *, this->_logo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "achieve_lock_sprite", CCSprite *, this->_achieve_lock_sprite);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_juqin_1", CCMenuItemImage *, this->_pressJuqinBTN);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_jiangli_1", CCMenuItemImage *, this->_pressJiangliBTN);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_tiaozhan_1", CCMenuItemImage *, this->_pressTiaozhanBTN);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_setting_1", CCMenuItemImage *, this->_pressSettingBTN);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_help_1", CCMenuItemImage *, this->_pressHelpBTN);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "button_chengji_1", CCMenuItemImage *, this->_pressChengjiBTN);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_juqin", CCMenuItemImage *, this->_pressJuqin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_jiangli", CCMenuItemImage *, this->_pressJiangli);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_tiaozhan", CCMenuItemImage *, this->_pressTiaozhan);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_setting", CCMenuItemImage *, this->_pressSetting);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_help", CCMenuItemImage *, this->_pressHelp);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "real_button_chengji", CCMenuItemImage *, this->_pressChengji);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void StartupScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("StartupScene onNodeLoaded~") ;
	
	_pressJuqin->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressJuqin->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
	
	_pressJiangli->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressJiangli->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
	
	_pressTiaozhan->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressTiaozhan->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
	
	_pressSetting->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressSetting->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
	
	_pressHelp->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressHelp->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
	
	_pressChengji->setSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemSelected));
	_pressChengji->setUnSelectEventTarget(this, menu_selector(StartupScene::handleMenuItemUnSelected));
    
    //挑战模式开启判断
    if (GameUtilities::getUnlockWithId(10006)) {
        _achieve_lock_sprite->setVisible(false);
    } else {
        _achieve_lock_sprite->setVisible(true);
    }
}

void StartupScene::onEnter(){
	CCLayer::onEnter();
}
NS_KAI_END
