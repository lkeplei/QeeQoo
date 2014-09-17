//
//  LevelMVScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "LevelMVScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "CCSWFNodeExt.h"
#include "GameEffectManager.h"
#include "GameConfig.h"

#include "GameOverScene.h"

NS_KAI_BEGIN
LevelMVScene::LevelMVScene():CCLayer(),
_helpSpriteRoot(NULL),
_swf(NULL),
_zoneid(0)
{
    mvCanTouch = true;
    _movieKey = KStrMovieBegin;
    setTouchEnabled( true );
}

LevelMVScene::~LevelMVScene()
{
    CCLOG("LevelMVScene::~LevelMVScene()");
	CC_SAFE_RELEASE_NULL(_helpSpriteRoot);
}

LevelMVScene* LevelMVScene::createWithCCB(const std::string & movieKey)
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("LevelMVScene",LevelMVSceneLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	LevelMVScene * node = (LevelMVScene*) ccBReader->readNodeGraphFromFile("level_mv_layer.ccbi");
	if (node) {
        node->_movieKey = movieKey;
        //node->retain();
		CC_SAFE_RELEASE_NULL(ccBReader);
        node->autorelease();
	}
	else {
		CC_SAFE_RELEASE_NULL(ccBReader);
	}
	return node;
}

CCScene* LevelMVScene::scene(const int zoneid,const int levelid,const std::string & movieKey)
{
	CCScene *scene = CCScene::create();
	LevelMVScene * node = createWithCCB(movieKey);
	if(node!=NULL){
		node->_levelid = levelid;
		node->_zoneid = zoneid;
		scene->addChild(node);
	}
	return scene;
}

void LevelMVScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SUB_LEVELS_SELECTION,CCInteger::create(_zoneid));
}

void LevelMVScene::press_next()
{
    if (_swf) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        GameModle::sharedInstance()->playBackground(K_BG_MUSIC_OUTSIDE_OF_BATTLE);
    }
    
    if (GameModle::sharedInstance()->getHelpIndex(_levelid, _zoneid) == -1) {
        if (_zoneid == 2 && _levelid == 14) {
            CCScene *pScene = GameOverScene::scene(true);
            GameController::sharedInstance()->controllerPushSence(pScene);
        } else {
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_PRE,CCInteger::create(_levelid));   
        }
        
        mvCanTouch = false;
    } else {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_HELP_IN_LEVEL
                                                      ,CCInteger::create(_zoneid)
                                                      ,CCInteger::create(_levelid));
        
        mvCanTouch = false;
    }
}

void LevelMVScene::updateHelper()
{
    if (!_swf) {
        //放动画前停止之前的后台音乐
        GameModle::sharedInstance()->stopBackgroundMusic();
        
        stringstream currentBigStr;
        currentBigStr << _zoneid;
        CCDictionary * dict = (CCDictionary *)GameConfig::sharedInstance()->getLevelsValue(currentBigStr.str());
        CCString * file = (CCString *)dict->objectForKey(_movieKey);
        _swf = CCSWFNodeSequence::create(file->getCString());
        if (_swf) {
            const CCSize size = this->getContentSize();
            _swf->setPosition(ccp(size.width * 0.5,size.height * 0.5));
            _swf->setScale(1.0f);
            this->addChild(_swf, 100);
            _swf->addCompletedListener(this,menu_selector(LevelMVScene::handleSwfFinished));
            _swf->addFrameListener(this,menu_selector(LevelMVScene::handleSwfFrameChanged));
            _swf->runSWFSequence();
        }
    }
}

void LevelMVScene::handleSwfFrameChanged(cocos2d::CCObject * obj){
    if (_swf && _swf->getCurrentSequenceIndex() == _swf->getSequenceSize() - 1) {
        CCSWFNodeExt * swfNode = ( CCSWFNodeExt *)obj;
        if (swfNode && swfNode->getCurrentFrame() == (swfNode->getFrameCount() - 2)) {
//            _swf->stopSWFSequence();
//            CCCallFunc * call = CCCallFunc::create(this, callfunc_selector(LevelMVScene::continueSwf));
//            CCDelayTime * delay = CCDelayTime::create(3.0f);
//            CCSequence * seq = CCSequence::createWithTwoActions(delay, call);
//            this->runAction(seq);
        }
    }
}

void LevelMVScene::continueSwf(){
    press_next();
}

void LevelMVScene::handleSwfFinished(cocos2d::CCObject * obj){
    press_next();
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler LevelMVScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "help_back_last_view",LevelMVScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_next",LevelMVScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler LevelMVScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", LevelMVScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool LevelMVScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "help_sprite", CCNode *,_helpSpriteRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void LevelMVScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
}

void LevelMVScene::onEnter(){
	CCLayer::onEnter();
    updateHelper();
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

#pragma mark - touch
//触摸事件
bool LevelMVScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	return mvCanTouch;
}

void LevelMVScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
}

void LevelMVScene::ccTouchEnded(CCTouch *pTouch, CCEvent* event){
    press_next();
}

NS_KAI_END

