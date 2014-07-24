//
//  GameController.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameController.h"
#include "GameModle.h"
#include "BattleScene.h"
#include "GameObject.h"
#include "GameConfig.h"
#include "GameConstant.h"
#include "LevelsSelectionScene.h"
#include "StartupScene.h"
#include "HelpScene.h"
#include "SettingScene.h"
#include "AboutScene.h"
#include "GameOverScene.h"
#include "SubLevelsSelectionScene.h"
#include "AchievementScene.h"
#include "AchievementViewScene.h"
#include "GameStartScene.h"
#include "ScoresViewScene.h"
#include "HelpInLevelScene.h"
#include "LevelMVScene.h"

#include "ChallengeStart.h"
#include "ChallengeGameStartScene.h"
#include "ChallengeHelpInLevelScene.h"
#include "ChallengeGameOverScene.h"

NS_KAI_BEGIN
static GameController * _sharedInstance=NULL;

GameController::GameController():CCObject()
,_sharedGameModleInstance(NULL)
,_director(NULL)
,_updateStoped(true)
,_pushSenceFromBattle(false)
,_pushSence(false)
{
	
}

GameController::~GameController(){

}

GameController * GameController::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameController();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameController::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameController::init(void){
	_director = CCDirector::sharedDirector();
	_sharedGameModleInstance = GameModle::sharedInstance();
	return true;
}

void GameController::update(float dt){
	
	//刷新modle[物理世界，业务逻辑]
	if(!_updateStoped) 
		_sharedGameModleInstance->update(dt);
}

void GameController::pushSence(const ESceneId sceneId,CCObject * param,CCObject * param2){
    CCLOG("GameController::pushSence %d",sceneId);
	switchSence(sceneId,param,param2,true);
}

bool GameController::popSence(){
    CCLOG("GameController::popSence");
	if (_pushSence) {
		if (_director->getRunningScene()) {
			_director->popScene();
		}
		_pushSence = false;
		return true;
	}
	return false;
}

void GameController::switchSence(const ESceneId sceneId,
								 CCObject * param,
								 CCObject * param2,
								 bool pushSence){
	
	_pushSence = pushSence;
    CCScene *pScene = NULL;	
	switch (sceneId) {
		case K_SCENE_HOME:
		{
			pScene = StartupScene::scene();
		}
			break;
		case K_SCENE_LEVELS_SELECTION:
		{
			pScene = LevelsSelectionScene::scene();
		}
			break;
		case K_SCENE_BATTLE_PRE:
		{
			CCInteger * levelid = dynamic_cast<CCInteger * >(param);
			CCAssert(levelid != NULL, "GameController::switchSence K_SCENE_BATTLE_PRE---> param is not CCInteger");
			pScene = GameStartScene::scene(levelid->getValue());
		}
			break;
        case K_SCENE_BATTLE_CHALLENGE:
		{
            CCInteger * levelid = dynamic_cast<CCInteger * >(param);
            _sharedGameModleInstance->resetBattleInfo();
            _sharedGameModleInstance->setBattleMode(K_HARD_PVE_BATTLE);
            _sharedGameModleInstance->setCurrentLevelId(levelid->getValue());
            _sharedGameModleInstance->setCurrentBigLevelId(4);
			_pushSenceFromBattle = _pushSence;
			pScene = BattleScene::scene();
		}
            break;
		case K_SCENE_BATTLE:
		{
            CCInteger * levelid = dynamic_cast<CCInteger * >(param);
            _sharedGameModleInstance->resetBattleInfo();
            _sharedGameModleInstance->setBattleMode(K_NORMAL_PVE_BATTLE);
            _sharedGameModleInstance->setCurrentLevelId(levelid->getValue());
            if (param2) {
                _sharedGameModleInstance->setCurrentBigLevelId(dynamic_cast<CCInteger * >(param2)->getValue());
            }
			_pushSenceFromBattle = _pushSence;
			pScene = BattleScene::scene();
		
		}
			break;
		case K_SCENE_SETTING:
		{
			pScene = SettingScene::scene();
		}
			break;
			
		case K_SCENE_SETTING_IN_LEVEL:
		{
			pScene = SettingInLevelScene::scene();
		}
			break;
		case K_SCENE_HELP:
		{
			pScene = HelpScene::scene();
		}
			break;
		case K_SCENE_ACHIEVEMENT:
		{
			pScene=AchievementScene::scene();
		}
			break;
		case K_SCENE_VIEW_ACHIEVEMENT:
		{
			pScene = AchievementViewScene::scene(param);
		}
			break;
		case K_SCENE_ABOUT:
		{
			pScene = AboutScene::scene();
		}
			break;
		case K_SCENE_SUCCESS:
		case K_SCENE_FAILED:
		{
            if (_sharedGameModleInstance->getBattleMode() ==  K_HARD_PVE_BATTLE) {
                pScene = ChallengeGameOverScene::scene();
            }else{
                pScene = GameOverScene::scene();
            }
		}
			break;
		case K_SCENE_HELP_IN_LEVEL:
		{
			CCInteger * levelid = dynamic_cast<CCInteger * >(param2);
			CCInteger * zoneid = dynamic_cast<CCInteger * >(param);
			pScene = HelpInLevelScene::scene(zoneid->getValue(),levelid->getValue());
		}
			break;
		case K_SCENE_LEVEL_MV:
		{
			CCInteger * levelid = dynamic_cast<CCInteger * >(param2);
			CCInteger * zoneid = dynamic_cast<CCInteger * >(param);
			pScene = LevelMVScene::scene(zoneid->getValue(),levelid->getValue(),KStrMovieBegin);
		}
			break;
        case K_SCENE_ChallengeStart:
		{
			pScene = ChallengeStart::scene();
		}
			break;
        case K_SCENE_ChallengeGameStart:
		{
            CCInteger * type = dynamic_cast<CCInteger * >(param);
			pScene = ChallengeGameStartScene::scene(type ? type->getValue() : 0);
		}
			break;
        case K_SCENE_ChallengeHelpInLevel:
		{
			pScene = ChallengeHelpInLevelScene::scene();
		}
			break;
		case K_SCENE_SUB_LEVELS_SELECTION:
		{
			CCInteger * levelid = dynamic_cast<CCInteger * >(param);
			CCAssert(levelid != NULL, "GameController::switchSence K_SCENE_SUB_LEVELS_SELECTION---> param is not CCInteger");
			_sharedGameModleInstance->setCurrentBigLevelId(levelid->getValue());
			pScene = SubLevelsSelectionScene::scene();
			
		}
			break;
		case K_SCENE_SCORES:
		{
			pScene = ScoresViewScene::scene();
			
		}break;
		default:
			break;
	}
	CCAssert(pScene != NULL, "GameController::switchSence sceneId is invalid..");
	if (pScene) {
		if (_director->getRunningScene()) {
			if (pushSence) {
				_director->pushScene(pScene);
			}
			else _director->replaceScene(pScene);
		}
		else{
			_director->runWithScene(pScene);
		}
	}
}

void GameController::onEnterScene(const ESceneId sceneId,CCLayer * layer){
	CCLOG("GameController::onEnterScene[%x]...",this);
	if (sceneId == K_SCENE_BATTLE ||sceneId == K_SCENE_BATTLE_CHALLENGE) {
		CCAssert(layer != NULL, "GameController::onEnterScene->battle layer is invalid");
		_sharedGameModleInstance->resetCache(layer);
		_sharedGameModleInstance->initAudioEffect();
		_sharedGameModleInstance->releasePhysics();
		_sharedGameModleInstance->initPhysics();
		_sharedGameModleInstance->generateGameObjects();
		_sharedGameModleInstance->playBackground(K_BG_MUSIC_BATTLE);
		_sharedGameModleInstance->gameStart();
		startBattle();
	}
	
}

void GameController::onExitScene(const ESceneId sceneId,CCLayer * layer){
	CCLOG("GameController::onExitScene[%x]...",this);
	if (sceneId == K_SCENE_BATTLE ||sceneId == K_SCENE_BATTLE_CHALLENGE) {
		CCAssert(layer != NULL, "GameController::onExitScene->battle layer is invalid");
		_sharedGameModleInstance->releaseAudioEffect();
		_sharedGameModleInstance->clearCache();
		_sharedGameModleInstance->releasePhysics();
		_sharedGameModleInstance->playBackground(K_BG_MUSIC_OUTSIDE_OF_BATTLE);
	}
}

void GameController::startBattle(void){
	resumeBattle();
}

void GameController::pauseBattle(void){
	_updateStoped=true;
	_director->getScheduler()->unscheduleSelector(schedule_selector(GameController::update)
												, this);
}

void GameController::resumeBattle(void){
	_director->getScheduler()->scheduleSelector(schedule_selector(GameController::update)
												, this, 1.0f/60.f, false);
	_updateStoped=false;
}

void GameController::onTouchesEnded(CCSet* touches, CCEvent* event){
    CCSetIterator it;
    CCTouch* touch;
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
		
        CCPoint location = touch->getLocationInView();
        location = _director->convertToGL(location);
		_sharedGameModleInstance->pointPressed(location);
    }
}

NS_KAI_END
