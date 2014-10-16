//
//  GameOverScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameOverScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
NS_KAI_BEGIN
GameOverScene::GameOverScene():CCLayer()
,_result_icon(NULL)
,_achievement_icon(NULL)
,_currentLevelId(0)
,_nextLevelId(-1)
,_play_next(NULL)
,_replay_menu(NULL)
,_home_menu(NULL)
,_result_scores(NULL)
,_gameOver(false)
{

}

GameOverScene::~GameOverScene()
{
	
	CC_SAFE_RELEASE_NULL(_result_scores);
	CC_SAFE_RELEASE_NULL(_result_icon);
	CC_SAFE_RELEASE_NULL(_achievement_icon);
	CC_SAFE_RELEASE_NULL(_play_next);
    CC_SAFE_RELEASE_NULL(_replay_menu);
    CC_SAFE_RELEASE_NULL(_home_menu);
}

GameOverScene* GameOverScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("GameOverScene",GameOverSceneLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	GameOverScene * node = (GameOverScene*)ccBReader->readNodeGraphFromFile("gameover_layer.ccbi");
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

CCScene* GameOverScene::scene(bool gameOver)
{
	CCScene *scene=CCScene::create();
	GameOverScene * node =(GameOverScene*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
        
        node->_gameOver = gameOver;
        node->updateScene();
	}
	return scene;
}

void GameOverScene::press_menu_page()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void GameOverScene::press_replay()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE,
                                                  CCInteger::create(_currentLevelId),
                                                  CCInteger::create(GameModle::sharedInstance()->currentBigLevelId()));
}

void GameOverScene::press_play_next() {
	if (_nextLevelId >= 0) {
        const int32_t bigLevelId = ((_nextLevelId % 900000) / 100) - 1;
        GameModle::sharedInstance()->setCurrentBigLevelId(bigLevelId);
        const int32_t sublevelId = (((_nextLevelId % 900000) - (bigLevelId + 1) * 100) - 1);
        
        //下一关，有动画先走动画
        if (sublevelId == 0) {
            std::stringstream strid;
            strid << "have_play_before_" << bigLevelId << std::endl;
            if (!CCUserDefault::sharedUserDefault()->getBoolForKey(strid.str().c_str())) {
                CCUserDefault::sharedUserDefault()->setBoolForKey(strid.str().c_str(), true);
                GameController::sharedInstance()->switchSence(GameController::K_SCENE_LEVEL_MV
                                                              ,CCInteger::create(bigLevelId)
                                                              ,CCInteger::create(sublevelId));
                
                return;
            }
        }
        
        //没有直接有帮助先走帮助
        if (GameModle::sharedInstance()->getHelpIndex(sublevelId, bigLevelId) == -1) {
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_PRE,
                                                          CCInteger::create(sublevelId));
        } else {
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_HELP_IN_LEVEL
                                                          ,CCInteger::create(bigLevelId)
                                                          ,CCInteger::create(sublevelId));
        }
        
//		const int bigLevelId = (int)((_nextLevelId % 900000) / 100);
//		GameModle * _sharedInstance=GameModle::sharedInstance();
//		_sharedInstance->setCurrentBigLevelId((bigLevelId - 1));
//		const int32_t sublevelId = (((_nextLevelId % 900000) - bigLevelId * 100) - 1);
//		GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_PRE,CCInteger::create(sublevelId));
	}
}

void GameOverScene::updateScene() {
    if (_gameOver) {
        _play_next->setVisible(false);
        _play_next->setEnabled(false);
        
        _replay_menu->setPosition(_replay_menu->getPosition().x + 50, _replay_menu->getPosition().y);
        _home_menu->setPosition(_home_menu->getPosition().x + 80, _home_menu->getPosition().y);
    }
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler GameOverScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "menu_page",GameOverScene::press_menu_page);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "replay",GameOverScene::press_replay);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "play_next",GameOverScene::press_play_next);

	return NULL;
}

cocos2d::extension::SEL_CCControlHandler GameOverScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", GameOverScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool GameOverScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "result_icon", CCSprite *, _result_icon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "achievement_icon", CCSprite *,_achievement_icon);
    
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "play_next_menu_item", CCMenuItem *,_play_next);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "replay_menu_item", CCMenuItem *, _replay_menu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "home_menu_item", CCMenuItem *, _home_menu);

	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "result_score_node",CCNode *, _result_scores);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void GameOverScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	
	GameModle * _sharedInstance=GameModle::sharedInstance();
	_currentLevelId=_sharedInstance->currentLevelId();
	
	CCString * nextlevel=(CCString *)_sharedInstance->valueFromCurrentLevel(KStrNextLevel);
	if(nextlevel){
		_nextLevelId = nextlevel->intValue();
	}

	const PlayerAchievement & playerAchievement = _sharedInstance->playerAchievement();
	
	if (_result_icon) {
		std::string  filename="scene_biggou.png";
		if (playerAchievement._failed) {
			filename = "scene_bigcha.png";
		}
		else if (playerAchievement._star_count > 0) {
			filename = "scene_bigstar.png";
		}
		
		CCSpriteFrame * pNewFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename.c_str());
		if (pNewFrame) {
			_result_icon->setDisplayFrame(pNewFrame);
		}

	}
	
	if (_nextLevelId == -1 || playerAchievement._failed) {
		if (_play_next) {
			_play_next->setEnabled(false);
            _play_next->selected();
		}
	}
	
	std::stringstream  resultTextStr;
	resultTextStr << playerAchievement._killNpcCount << "/" << playerAchievement._totalNpcCount;
	CCSize labelSize =  _result_scores->getContentSize();
    CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(resultTextStr.str());
    CCSize cellSize =  label->getContentSize();
    label->setAnchorPoint(CCPoint(0.5, 0.5));
    label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
    _result_scores->addChild(label);
	
	CCLOG("GameOverScene onNodeLoaded~") ;
}

void GameOverScene::onEnter(){
	CCLayer::onEnter();
}

void GameOverScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

