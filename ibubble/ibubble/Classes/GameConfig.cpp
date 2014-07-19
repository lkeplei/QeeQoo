//
//  GameConfig.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameConfig.h"
NS_KAI_BEGIN
static GameConfig * _configSharedInstance=NULL;

GameConfig::GameConfig():CCObject()
,_properties(NULL)
,_simpleProperties(0)
,_box2dVelocityIterations(8)
,_box2dPositionIterations(1)
,_topLevelId(0)
,_npc(NULL)
,_levels(NULL)
,_box2d(NULL)
,_box2dfilter(NULL)
,_maxReliveTime(3)
{
	
}

GameConfig::~GameConfig(){
	CC_SAFE_RELEASE_NULL(_properties);
}

GameConfig * GameConfig::sharedInstance(void){
	
	if (! _configSharedInstance)
    {
        _configSharedInstance = new GameConfig();
        _configSharedInstance->init();
    }
	
	return _configSharedInstance;
}

void GameConfig::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_configSharedInstance);
}

bool GameConfig::init(void){
	
	std::string filename = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("config.plist");
	_properties=CCDictionary::createWithContentsOfFile(filename.c_str());
	_properties->retain();
	_maxReliveTime = ((CCString *)_properties->objectForKey(KStrMaxReliveTime))->intValue();
    
	//setIsDebuging(true);
	
	
	return true;
}

CCObject * GameConfig::getValue(const std::string & key){
	return _properties->objectForKey(key);
}

void GameConfig::setValue(const std::string & key,CCObject * value){
	CCAssert(value != NULL, "GameConfig::setValue-->value is invalid!!!");
	_properties->setObject(value, key);
}


CCObject * GameConfig::getNpcValue(const std::string & key){
	if (!_npc) {
		_npc=(CCDictionary *)_properties->objectForKey(KStrNpc);
	}
	return _npc->objectForKey(key);
}

CCObject * GameConfig::getLevelsValue(const std::string & key){
	if (!_levels) {
		_levels=(CCDictionary *)_properties->objectForKey(KStrLevels);
	}
	return _levels->objectForKey(key);
}

CCObject * GameConfig::getBox2dFilterValue(const std::string & key){
	if (!_box2dfilter) {
		_box2dfilter=(CCDictionary *)_properties->objectForKey(KStrBox2dFilter);
	}
	return _box2dfilter->objectForKey(key);
}

CCObject * GameConfig::getBox2dValue(const std::string & key){
	if (!_box2d) {
		_box2d=(CCDictionary *)_properties->objectForKey(KStrBox2d);
	}
	return _box2d->objectForKey(key);
}



bool GameConfig::isDebuging() const {
	return (_simpleProperties  & 0x00000001) > 0;
}

void GameConfig::setIsDebuging(const bool value){
	if (value) {
		_simpleProperties=(_simpleProperties & 0xfffffffe)|(0x00000001);
	}
	else {
		_simpleProperties=(_simpleProperties & 0xfffffffe);
	}
}

uint32_t GameConfig::topLevelId() const{
	return _topLevelId;
}

void GameConfig::setTopLevelId(const uint32_t value){
	_topLevelId=value;
}
NS_KAI_END