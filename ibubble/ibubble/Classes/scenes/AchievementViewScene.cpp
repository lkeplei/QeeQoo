//
//  AchievementViewScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "AchievementViewScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameObject.h"
#include "UiTool.h"
#include "GameEffectManager.h"
#include "KissView.h"
NS_KAI_BEGIN
AchievementViewScene::AchievementViewScene():CCLayer(),
_displaySpriteRoot(NULL)
,_bgSpriteRoot(NULL)
,_swf(NULL)
,swfFileArray(NULL)
{
    setTouchEnabled( true );	
}

AchievementViewScene::~AchievementViewScene()
{
	CC_SAFE_RELEASE_NULL(_displaySpriteRoot);
    CC_SAFE_RELEASE_NULL(_bgSpriteRoot);
}

AchievementViewScene* AchievementViewScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("AchievementViewScene",AchievementViewSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	AchievementViewScene * node =(AchievementViewScene*)ccBReader->readNodeGraphFromFile("achievement_view_layer.ccbi");
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

CCScene* AchievementViewScene::scene(CCObject * params)
{
	CCScene *scene = CCScene::create();
	CCLayer * node = (CCLayer*)createWithCCB();
	if(node!=NULL){
		((AchievementViewScene *)node)->initWithParams(params);
		scene->addChild(node);
	}
	return scene;
}

void AchievementViewScene::press_back()
{
    if (swfFileArray && swfIndex < swfFileArray->count()) {
        playSwf((CCString *)swfFileArray->objectAtIndex(swfIndex));
        swfIndex++;
    } else {
        GameModle::sharedInstance()->stopBackgroundMusic();
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_ACHIEVEMENT,CCInteger::create(0));
    }
}

void AchievementViewScene::playSwf(CCString *filename) {
    if (_swf) {
        _swf->stopSWFSequence();
    }
    
    _swf = CCSWFNodeSequence::create(filename->getCString());
    
    if (_swf) {
        const CCSize size = _displaySpriteRoot->getContentSize();
        _swf->setPosition(ccp(size.width * 0.5,size.height * 0.5));
        _swf->setScale(1.0f);
        _displaySpriteRoot->addChild(_swf,100);
        _swf->addCompletedListener(this,menu_selector(AchievementViewScene::handleSwfFinished));
        _swf->runSWFSequence();
    }
}

void AchievementViewScene::initWithParams(CCObject * params){
    canTouch = false;
	if(_displaySpriteRoot && params){
		CCDictionary * dict = (CCDictionary *)params;
        CCDictionary * display = (CCDictionary *)dict->objectForKey(KStrDisplay);
        CCString * type =(CCString *)display->objectForKey(KStrType);
        if (type->intValue() == GameObject::K_SWF_FILE) {
            canTouch = true;
            
            if (_bgSpriteRoot) {
                _bgSpriteRoot->setVisible(false);
            }

            swfIndex = 0;
            swfFileArray = (CCArray *)display->objectForKey(KStrFileArray);
            playSwf((CCString *)swfFileArray->objectAtIndex(swfIndex));
            swfIndex++;
        }
        else if (type->intValue() == GameObject::K_CCBI_FILE_WITH_CODE) {
            if (_bgSpriteRoot) {
                _bgSpriteRoot->setVisible(false);
            }
            
            CCString * filename = (CCString *)display->objectForKey(KStrFile);
            if (filename && (filename->m_sString.compare("KissView.ccbi") == 0)) {
                KissView * node = KissView::createWithCCB("KissView.ccbi");
                node->setPosition(CCPoint(0,0));
                _displaySpriteRoot->addChild(node);
            }
            
        }else{
            GameDisplayNode * node = GameDisplayNode::create(display);
            if (node) {
                CCDictionary * paramDict = (CCDictionary *)dict->objectForKey(KStrParam);
                if (paramDict) {
                    CCString  *  str = (CCString  * )paramDict->objectForKey(KStrDefaultAnim);
                    if (str) {
                        node->runAnimation(str->getCString());
                    }
                }
                node->setPosition(CCPoint(_displaySpriteRoot->getContentSize().width/2,
                                          _displaySpriteRoot->getContentSize().height/2));
                _displaySpriteRoot->addChild(node);
            }
        }
	}
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler AchievementViewScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",AchievementViewScene::press_back);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler AchievementViewScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", AchievementViewScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool AchievementViewScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "display_node", CCNode *,_displaySpriteRoot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "bg_node", CCSprite *,_bgSpriteRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void AchievementViewScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
}

void AchievementViewScene::handleSwfFinished(cocos2d::CCObject * obj){
    if (swfIndex == swfFileArray->count() || swfFileArray == NULL) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        GameModle::sharedInstance()->playBackground(K_BG_MUSIC_OUTSIDE_OF_BATTLE);
    }
    
    press_back();
}

void AchievementViewScene::onEnter(){
	CCLayer::onEnter();

    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

#pragma mark - touch
//触摸事件
bool AchievementViewScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return canTouch;
}

void AchievementViewScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void AchievementViewScene::ccTouchEnded(CCTouch *pTouch, CCEvent* event){
    press_back();
}

NS_KAI_END

