//
//  GameObjectFactory.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameObjectFactory.h"
NS_KAI_BEGIN
static GameObjectFactory * _sharedInstance=NULL;

GameObjectFactory::GameObjectFactory():CCObject()
,_properties(NULL)
{
	
}

GameObjectFactory::~GameObjectFactory(){
	CC_SAFE_RELEASE_NULL(_properties);
}

GameObjectFactory * GameObjectFactory::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameObjectFactory();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameObjectFactory::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameObjectFactory::init(void){
	_properties=CCDictionary::create();
	_properties->retain();
	return true;
}

CCObject * GameObjectFactory::getValue(const std::string & key){
	return _properties->objectForKey(key);
}

void GameObjectFactory::setValue(const std::string & key,CCObject * value){
	CCAssert(value != NULL, "GameObjectFactory::setValue-->value is invalid!!!");
	_properties->setObject(value, key);
}
NS_KAI_END