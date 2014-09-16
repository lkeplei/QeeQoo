//
//  KissView.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-12-04.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#include "KissView.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "CCSWFNodeExt.h"
NS_KAI_BEGIN
KissView::KissView()
: CCLayer()
, _defaultBgImg(NULL)
, _defaultSprite(NULL)
, _defaultBoyKissGirl(NULL)
, _defaultGirlKissBoy(NULL)
, _swf(NULL)
, defaultStatus(true)
{

}

KissView::~KissView()
{
    CC_SAFE_RELEASE_NULL(_defaultBgImg);
    CC_SAFE_RELEASE_NULL(_defaultSprite);
    CC_SAFE_RELEASE_NULL(_defaultBoyKissGirl);
    CC_SAFE_RELEASE_NULL(_defaultGirlKissBoy);
}

KissView* KissView::createWithCCB(const char *pCCBFileName)
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("KissView",KissViewLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	KissView * node =(KissView*)ccBReader->readNodeGraphFromFile(pCCBFileName);
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

CCScene* KissView::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB("KissView.ccbi");
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void KissView::press_default() {
    defaultStatus = true;
    
    _defaultBgImg->setVisible(true);
    _defaultSprite->setVisible(true);
    _defaultGirlKissBoy->setVisible(false);
    _defaultBoyKissGirl->setVisible(false);
}

void KissView::press_back() {
    GameController::sharedInstance()->switchSence(GameController::K_SCENE_ACHIEVEMENT,CCInteger::create(0));
}

void KissView::press_boy_kiss() {
    if (defaultStatus) {
        _defaultBgImg->setVisible(true);
        _defaultSprite->setVisible(false);
        _defaultGirlKissBoy->setVisible(false);
        _defaultBoyKissGirl->setVisible(true);
        
        playSWF("kiss_movie_1.plist");
    } else {
        press_default();
    }
}

void KissView::press_girl_kiss() {
    if (defaultStatus) {
        _defaultBgImg->setVisible(true);
        _defaultSprite->setVisible(false);
        _defaultGirlKissBoy->setVisible(true);
        _defaultBoyKissGirl->setVisible(false);
        
        playSWF("kiss_movie_2.plist");
    } else {
        press_default();
    }
}

void KissView::playSWF(const std::string & file)
{
    _swf = CCSWFNodeSequence::create(file.c_str());
    if (_swf) {
        const CCSize size = this->getContentSize();
        _swf->setPosition(ccp(size.width * 0.5,size.height * 0.5));
        _swf->setScale(1.0f);
        this->addChild(_swf,100);
        _swf->addCompletedListener(this,menu_selector(KissView::handleSwfFinished));
        _swf->addFrameListener(this,menu_selector(KissView::handleSwfFrameChanged));
        _swf->runSWFSequence();
    }
}

void KissView::handleSwfFrameChanged(cocos2d::CCObject * obj){
    if (_swf && _swf->getCurrentSequenceIndex() == _swf->getSequenceSize() - 1) {
        CCSWFNodeExt * swfNode = ( CCSWFNodeExt *)obj;
        if (swfNode && swfNode->getCurrentFrame() == (swfNode->getFrameCount() - 4)) {
//            _swf->stopSWFSequence();
//            swfNode->stopAction();
        }
    }
}

void KissView::handleSwfFinished(cocos2d::CCObject * obj){
    defaultStatus = false;
    
    if (_swf) {
        _swf->removeFromParentAndCleanup(true);
        _swf = NULL;
    }
}
#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler KissView::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",KissView::press_back);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_boy_kiss",KissView::press_boy_kiss);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_girl_kiss",KissView::press_girl_kiss);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_default",KissView::press_default);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler KissView::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", KissView::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool KissView::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "defaultBgImg", CCSprite *, _defaultBgImg);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "defaultSprite", CCSprite *, _defaultSprite);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "defaultGirlKissBoy", CCSprite *, _defaultGirlKissBoy);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "defaultBoyKissGirl", CCSprite *, _defaultBoyKissGirl);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void KissView::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("KissView onNodeLoaded~") ;
}

void KissView::onEnter(){
	CCLayer::onEnter();
}

void KissView::onExit(){
	CCLayer::onExit();
}

NS_KAI_END

