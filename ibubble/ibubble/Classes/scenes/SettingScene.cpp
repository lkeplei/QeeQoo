//
//  SettingScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "SettingScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "CCControlSlider.h"
#include "GameEffectManager.h"
#include "UiTool.h"
NS_KAI_BEGIN
SettingScene::SettingScene():CCLayer()
{
	_audio_node = NULL;
	_bgmusic_node = NULL;
}

SettingScene::~SettingScene()
{
	CC_SAFE_RELEASE_NULL(_audio_node);
	CC_SAFE_RELEASE_NULL(_bgmusic_node);
}

SettingScene* SettingScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("SettingScene",SettingSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	SettingScene * node =(SettingScene*)ccBReader->readNodeGraphFromFile("setting_layer.ccbi");
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

CCScene* SettingScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void SettingScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void SettingScene::press_next()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_ABOUT,CCInteger::create(0));
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler SettingScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",SettingScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_about",SettingScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler SettingScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", SettingScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool SettingScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "audio_node", CCNode *, this->_audio_node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bgmusic_node", CCNode *, this->_bgmusic_node);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void SettingScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("SettingScene onNodeLoaded~") ;

	CCSize size = _audio_node->getContentSize();
	
	CCSprite * backgroundSprite = NULL;
	CCSprite * pogressSprite = NULL;
	CCSprite * thumbSprite  = NULL;
	
	backgroundSprite = CCSprite::createWithSpriteFrameName("audio_volume_1.png");
	pogressSprite = CCSprite::createWithSpriteFrameName("audio_volume_2.png");
	thumbSprite  = CCSprite::createWithSpriteFrameName("icon_audio_button.png");
	
	cocos2d::extension::CCControlSlider * audioSlider = cocos2d::extension::CCControlSlider::create(backgroundSprite, pogressSprite, thumbSprite);
	audioSlider->setMinimumValue(0.0f); // Sets the min value of range
    audioSlider->setMaximumValue(1.0f); // Sets the max value of range
    audioSlider->setMaximumAllowedValue(1.0f);
    audioSlider->setMinimumAllowedValue(0.0f);
    audioSlider->setValue(GameModle::sharedInstance()->effectManager()->getEffectsVolume());
    audioSlider->setTag(80001);
	audioSlider->setPosition(size.width * 0.5, size.height * 0.5);
	_audio_node->addChild(audioSlider);
    audioSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::slidervalueChanged), CCControlEventValueChanged);
	
	
	
	backgroundSprite = CCSprite::createWithSpriteFrameName("audio_volume_1.png");
	pogressSprite = CCSprite::createWithSpriteFrameName("audio_volume_2.png");
	thumbSprite  = CCSprite::createWithSpriteFrameName("icon_audio_button.png");
	
	cocos2d::extension::CCControlSlider * musicSlider = cocos2d::extension::CCControlSlider::create(backgroundSprite, pogressSprite, thumbSprite);
	musicSlider->setMinimumValue(0.0f); // Sets the min value of range
    musicSlider->setMaximumValue(1.0f); // Sets the max value of range
    musicSlider->setMaximumAllowedValue(1.0f);
    musicSlider->setMinimumAllowedValue(0.0f);
    musicSlider->setValue(GameModle::sharedInstance()->effectManager()->getBackgroundMusicVolume());
    musicSlider->setTag(80002);
	musicSlider->setPosition(size.width * 0.5, size.height * 0.5);
	_bgmusic_node->addChild(musicSlider);
    musicSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(SettingScene::slidervalueChanged), CCControlEventValueChanged);


}
void SettingScene::slidervalueChanged(CCObject * sender, CCControlEvent controlEvent){
	
	if (controlEvent == CCControlEventValueChanged ) {
		cocos2d::extension::CCControlSlider * slider = (cocos2d::extension::CCControlSlider *)sender;
		if ((slider->getTag() == 80001)) {
			GameModle::sharedInstance()->effectManager()->setEffectsVolume(slider->getValue());
		}
		else if ((slider->getTag() == 80002)) {
			GameModle::sharedInstance()->effectManager()->setBackgroundMusicVolume(slider->getValue());
		}
	}
}

void SettingScene::onEnter(){
	CCLayer::onEnter();
}

void SettingScene::onExit(){
	
	CCLayer::onExit();
}


SettingInLevelScene* SettingInLevelScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("SettingInLevelScene",SettingInLevelSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	SettingInLevelScene * node = (SettingInLevelScene*)ccBReader->readNodeGraphFromFile("setting_in_level_layer.ccbi");
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

CCScene* SettingInLevelScene::scene()
{
	CCScene *scene = CCScene::create();
	SettingInLevelScene * node = createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void SettingInLevelScene::press_back()
{
	GameController::sharedInstance()->popSence();
	GameController::sharedInstance()->resumeBattle();
}

NS_KAI_END

