//
//  GameCallbackFactory.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameCallbackFactory.h"
#include "GameCallback.h"

NS_KAI_BEGIN
static GameCallbackFactory * _sharedInstance=NULL;

GameCallbackFactory::GameCallbackFactory():CCObject()
,_properties(NULL)
{
	
}

GameCallbackFactory::~GameCallbackFactory(){
	CC_SAFE_RELEASE_NULL(_properties);
}

GameCallbackFactory * GameCallbackFactory::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameCallbackFactory();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameCallbackFactory::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameCallbackFactory::init(void){
	_properties=new CCDictionary();

	GameCallback * callback=NULL;
	
	
	callback=new CallbackBubbleExplosion;
	_properties->setObject(callback, KStrCallbackBubbleExplosion);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackBubbleExplosionEffect;
	_properties->setObject(callback, KStrCallbackBubbleExplosionEffect);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackBubbleShoot;
	_properties->setObject(callback, KStrCallbackBubbleShoot);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackBubbleShootLaser;
	_properties->setObject(callback, KStrCallbackBubbleShootLaser);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackOpenSkillShoot;
	_properties->setObject(callback, KStrCallbackBubbleOpenSkillShoot);
	CC_SAFE_RELEASE_NULL(callback);  
	
	callback=new CallbackOpenSkillShootLaser;
	_properties->setObject(callback, KStrCallbackBubbleOpenSkillShootLaser);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackOpenSkillSpurtFire;
	_properties->setObject(callback, KStrCallbackBubbleOpenSkillSpurtFire);
	CC_SAFE_RELEASE_NULL(callback);
	
	callback=new CallbackRocketExplosion;
	_properties->setObject(callback, KStrCallbackRocketExplosion);
	CC_SAFE_RELEASE_NULL(callback);

	callback=new CallbackRocketExplosionEffect;
	_properties->setObject(callback, KStrCallbackRocketExplosionEffect);
	CC_SAFE_RELEASE_NULL(callback);
	
	return true;
}

GameCallback * GameCallbackFactory::getGameCallback(const std::string & key){
	return (GameCallback*)_properties->objectForKey(key);
}

bool GameCallbackFactory::doCallback(const std::string & key,GameObject * aGameObject,CCDictionary * dic,float dt){
	GameCallback * callback=getGameCallback(key);
	if (callback) {
		return callback->doCallback(aGameObject,dic,dt);
	}
	return false;
}

NS_KAI_END