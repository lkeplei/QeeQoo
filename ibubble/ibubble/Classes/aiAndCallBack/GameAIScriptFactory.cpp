//
//  GameAIScriptFactory.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameAIScriptFactory.h"
#include "GameAIScript.h"
#include "BattleAIScript.h"

NS_KAI_BEGIN
static GameAIScriptFactory * _sharedInstance=NULL;

GameAIScriptFactory::GameAIScriptFactory():CCObject()
,_properties(NULL)
{
	
}

GameAIScriptFactory::~GameAIScriptFactory(){
	CC_SAFE_RELEASE_NULL(_properties);
}

GameAIScriptFactory * GameAIScriptFactory::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameAIScriptFactory();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameAIScriptFactory::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameAIScriptFactory::init(void){
	_properties=new CCDictionary();
	
	//创建AI
	GameAIScript * script=NULL;
	
	script=new BattleAIScript;
	_properties->setObject(script, KStrBattleAIScriptDefault);
	CC_SAFE_RELEASE_NULL(script);
	
	script=new BulletAIScript;
	_properties->setObject(script, KStrGameAIScriptBullet);
	CC_SAFE_RELEASE_NULL(script);
	
	script=new BubbleAIScript;
	_properties->setObject(script, KStrGameAIScriptBubble);
	CC_SAFE_RELEASE_NULL(script);
	
	script=new BulletBezierAIScript;
	_properties->setObject(script, KStrGameAIScriptBulletBezier);
	CC_SAFE_RELEASE_NULL(script);
	
	
	return true;
}

GameAIScript * GameAIScriptFactory::getGameAIScript(const std::string & key){
	return (GameAIScript*)_properties->objectForKey(key);
}

NS_KAI_END