//
//  GameEffectManager.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameEffectManager.h"
#include "GameConfig.h"
#include "GameEffect.h"
#include "GameObject.h"

NS_KAI_BEGIN

static GameEffectManager * _sharedInstance=NULL;

GameEffectManager::GameEffectManager():CCObject(),_properties(NULL),_simpleAudioEngine(NULL)
{
	
}

GameEffectManager::~GameEffectManager(){
}

GameEffectManager * GameEffectManager::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameEffectManager();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameEffectManager::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameEffectManager::init(void){
	_properties=(CCDictionary *)GameConfig::sharedInstance()->getValue(KStrEffect);
	_simpleAudioEngine=CocosDenshion::SimpleAudioEngine::sharedEngine();
	
	return true;
}

void GameEffectManager::loadAudioEffect(CCDictionary * audios){
	
	CCArray * array=(CCArray * )audios->objectForKey(KStrEffect);
	if (array) {
		CCObject * obj=NULL;
		CCARRAY_FOREACH(array, obj){
			CCString * fileStr=(CCString * )obj;
			_simpleAudioEngine->preloadEffect(fileStr->getCString());
		}
	}
	
	CCString * fileStr=(CCString * )audios->objectForKey(KStrBackgound);
	if (fileStr) {
		_simpleAudioEngine->preloadBackgroundMusic(fileStr->getCString());
	}
	
}

void GameEffectManager::releaseAudioEffect(CCDictionary * audios){
	CCArray * array=(CCArray * )audios->objectForKey(KStrEffect);
	if (array) {
		CCObject * obj=NULL;
		CCARRAY_FOREACH(array, obj){
			CCString * fileStr=(CCString * )obj;
			_simpleAudioEngine->unloadEffect(fileStr->getCString());
		}
	}
	
	CCString * fileStr=(CCString * )audios->objectForKey(KStrBackgound);
	if (fileStr) {
		
		if (_backgroundMusicName.compare(fileStr->getCString()) == 0) {
			if(_simpleAudioEngine->isBackgroundMusicPlaying())
				_simpleAudioEngine->stopBackgroundMusic(true);
		}
		else {
			
		}
	}
}

void GameEffectManager::playBackground(const std::string & name,const bool stop2play){
	
	if (!(_backgroundMusicName.compare(name) == 0 && _simpleAudioEngine->isBackgroundMusicPlaying())) {
		_backgroundMusicName=name;
		_simpleAudioEngine->playBackgroundMusic(_backgroundMusicName.c_str());
	}
	else if(stop2play){
		_backgroundMusicName=name;
		_simpleAudioEngine->stopBackgroundMusic();
		_simpleAudioEngine->playBackgroundMusic(_backgroundMusicName.c_str());
	}
}


CCNode * GameEffectManager::playEffect(const std::string & effectId,CCNode *targetLayer,const CCPoint & position,CCAction * action){
	CCNode * node=NULL;
	CCDictionary * effectInfo=(CCDictionary *)_properties->objectForKey(effectId);
	if (effectInfo) {
		
		CCString * type=(CCString *)effectInfo->objectForKey(KStrType);
		if (type) {
			switch (type->uintValue()) {
				case K_EFFECT_PARTICLE:
				{
					CCString * plist = (CCString *)effectInfo->objectForKey(KStrPlist);
					GameEffectParticle * pNode = ExplosionParticle::create(plist->getCString());
					pNode->setPosition(position);
					pNode->setAutoRemoveOnFinish(true);
					targetLayer->addChild(pNode);
					node=pNode;
				}
					break;
				default:
					break;
			}
		}
		
		CCString * audio = (CCString *)effectInfo->objectForKey(KStrAudioName);
		if (audio) {
			_simpleAudioEngine->playEffect(audio->getCString());
		}
	}
	
	if (node) {
		if (action) {
			node->runAction(action);
		}
	}
	
	return node;
}

float GameEffectManager::getBackgroundMusicVolume(){
	return _simpleAudioEngine->getBackgroundMusicVolume();
}

void GameEffectManager::setBackgroundMusicVolume(float volume){
	_simpleAudioEngine->setBackgroundMusicVolume(volume);
}

float GameEffectManager::getEffectsVolume(){
	return _simpleAudioEngine->getEffectsVolume();
}

void GameEffectManager::setEffectsVolume(float volume){
	 _simpleAudioEngine->setEffectsVolume(volume);
}

NS_KAI_END
