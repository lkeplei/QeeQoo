//
//  GameStartScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameStartScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "GameConfig.h"

NS_KAI_BEGIN
GameStartScene::GameStartScene():CCLayer()
,_title_node(NULL)
,_pass_node(NULL)
,_star_node(NULL)
,_achievement_icon(NULL)
,_currentLevelId(0)
,_nextLevelId(-1)
,_play_next(NULL)
{

}

GameStartScene::~GameStartScene()
{
	CC_SAFE_RELEASE_NULL(_pass_node);
	CC_SAFE_RELEASE_NULL(_star_node);
	CC_SAFE_RELEASE_NULL(_title_node);
	CC_SAFE_RELEASE_NULL(_achievement_icon);
	CC_SAFE_RELEASE_NULL(_play_next);
}

GameStartScene* GameStartScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("GameStartScene",GameStartSceneLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	GameStartScene * node = (GameStartScene*)ccBReader->readNodeGraphFromFile("gamestart_layer.ccbi");
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

CCScene* GameStartScene::scene(int32_t   nextLevelId)
{
	CCScene *scene = CCScene::create();
	GameStartScene * node = createWithCCB();
	if(node != NULL){
		node->_nextLevelId = nextLevelId;
		node->afterLoaded();
		scene->addChild(node);
	}
	return scene;
}

void GameStartScene::press_play_next()
{
	if (!GameController::sharedInstance()->popSence()) {
		GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE,
                                                      CCInteger::create(_nextLevelId),
                                                      CCInteger::create(GameModle::sharedInstance()->currentBigLevelId()));
	}
	else{
		GameController::sharedInstance()->resumeBattle();
	}
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler GameStartScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget,
                                                               CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "play_next",GameStartScene::press_play_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler GameStartScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", GameStartScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool GameStartScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "title_node", CCNode *, _title_node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "achievement_icon", CCSprite *,_achievement_icon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "play_next_menu_item", CCMenuItem *,_play_next);
	
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pass_count", CCNode *, _pass_node);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star_count", CCNode *, _star_node);

	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void GameStartScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("GameStartScene onNodeLoaded~") ;
	
}
void GameStartScene::afterLoaded(){
	if (_title_node) {
		
		std::stringstream  starStr;
		
		starStr << (GameModle::sharedInstance()->currentBigLevelId() + 1) ;
		int index = _nextLevelId + 1;
		if (index < 10) {
			starStr << "/0" << index;
		} else {
			starStr << "/" << index;
		}
		
		CCSize labelSize =  _title_node->getContentSize();
        CCLabelAtlas *label = UiTool::createLabelAtlasWithStarNumber(starStr.str());
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        _title_node->addChild(label);
	}

    stringstream currentBigStr;
    currentBigStr << GameModle::sharedInstance()->currentBigLevelId();
    CCDictionary * dict = (CCDictionary *)GameConfig::sharedInstance()->getLevelsValue(currentBigStr.str());
    CCArray * sublevels = (CCArray *)dict->objectForKey(KStrSubLevels);
    CCDictionary * currentLevelDict = (CCDictionary *)sublevels->objectAtIndex(_nextLevelId);
    CCArray * npcs = (CCArray *)currentLevelDict->objectForKey(KStrNPCs);//npcs
    CCObject * npcitem = NULL;
    int pass_count = ((CCString *)currentLevelDict->objectForKey(KStrPassCount))->intValue();
    int star_count = ((CCString *)currentLevelDict->objectForKey(KStrStarCount))->intValue();
    int count = 0;
    CCARRAY_FOREACH(npcs, npcitem){
        CCDictionary * dict = (CCDictionary *)npcitem;
        count += ((CCString *)dict->objectForKey(KStrCount))->intValue();
    }
    
    if (_pass_node) {
        std::stringstream  starStr;
        starStr << pass_count << "/" << count;
        
        CCSize labelSize =  _title_node->getContentSize();
        CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(starStr.str());
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        _pass_node->addChild(label);
        
    }
    
    if (_star_node) {
        std::stringstream  starStr;
        starStr << star_count << "/" << count;
        
        CCSize labelSize =  _title_node->getContentSize();
        CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(starStr.str());
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        _star_node->addChild(label);
    }
}

void GameStartScene::onEnter(){
	CCLayer::onEnter();
}

void GameStartScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

