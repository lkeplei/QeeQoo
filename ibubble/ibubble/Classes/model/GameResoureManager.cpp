//
//  GameResoureManager.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameResoureManager.h"
NS_KAI_BEGIN
static GameResoureManager * _sharedInstance=NULL;

GameResoureManager::GameResoureManager():CCObject()
,_properties(NULL)
,_spriteFiles(NULL)
,_loadSpriteSheetAsyncTarget(NULL)
{

}

GameResoureManager::~GameResoureManager(){
	CC_SAFE_RELEASE_NULL(_loadSpriteSheetAsyncTarget);
	CC_SAFE_RELEASE_NULL(_spriteFiles);
	CC_SAFE_RELEASE_NULL(_properties);
}

GameResoureManager * GameResoureManager::sharedInstance(void){
	
	if (! _sharedInstance)
    {
        _sharedInstance = new GameResoureManager();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameResoureManager::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameResoureManager::init(void){
	_properties=CCDictionary::create();
	_properties->retain();
	return true;
}

CCObject * GameResoureManager::getValue(const std::string & key){
	return _properties->objectForKey(key);
}

void GameResoureManager::setValue(const std::string & key,CCObject * value){
	CCAssert(value != NULL, "GameResoureManager::setValue-->value is invalid!!!");
	_properties->setObject(value, key);
}


void GameResoureManager::loadSpriteSheetAsync(CCArray * spriteFiles,CCObject *target, SEL_CallFuncO selector){
	_spriteSheetCount=0;
	_spriteSheetLoadedCount=0;
	CCAssert(_loadSpriteSheetAsyncTarget == NULL, "_loadSpriteSheetAsyncTarget != NULL is invalid");
	if (spriteFiles) {
		CC_SAFE_RELEASE_NULL(_spriteFiles);
		_spriteFiles=spriteFiles;
		_spriteFiles->retain();
		
		CCTextureCache * cache=CCTextureCache::sharedTextureCache();
		int count=spriteFiles->count();
		CCArray * list=CCArray::createWithCapacity(count);
		for (int i=0; i<count; i++) {
			CCDictionary * imageItem=(CCDictionary*)spriteFiles->objectAtIndex(i);
			CCString * image=(CCString *)imageItem->objectForKey(KStrImage);
			if(image){
				_spriteSheetCount++;
				list->addObject(image);
			}
		}
		
		for (int i=0; i< _spriteSheetCount; i++) {
			cache->addImageAsync(((CCString *)list->objectAtIndex(i))->getCString(),this, callfuncO_selector(GameResoureManager::loadSpriteSheetCallBack));
		}
	}
	

	if (_spriteSheetCount > 0) {
		_loadSpriteSheetAsyncTarget=target;
		_loadSpriteSheetAsyncTarget->retain();
		_loadSpriteSheetAsyncSelector=selector;
	}
	else {
		
		if (target && selector)
        {
			CCDictionary * dict=CCDictionary::create();
			dict->setObject(CCInteger::create(_spriteSheetLoadedCount), "x");
			dict->setObject(CCInteger::create(_spriteSheetCount), "y");
            (target->*selector)(dict);
        }
		return;
	}
}

void GameResoureManager::loadSpriteSheetSync(CCArray * spriteFiles) {
	if (spriteFiles) {
		CCSpriteFrameCache * cache=CCSpriteFrameCache::sharedSpriteFrameCache();
		int count=spriteFiles->count();
		for (int i=0; i<count; i++) {
			CCDictionary * imageItem=(CCDictionary *)spriteFiles->objectAtIndex(i);
			CCString * plist=(CCString *)imageItem->objectForKey(KStrPlist);
			CCString * image=(CCString *)imageItem->objectForKey(KStrImage);
			cache->addSpriteFramesWithFile(plist->getCString(), image->getCString());
		}
	}
}

void GameResoureManager::loadSpriteSheetCallBack(CCObject * obj) {
 	//CCTexture2D *texture=(CCTexture2D *)obj;
	_spriteSheetLoadedCount++;
	
	CCLog("loadSpriteSheetCallBack:%d/%d",_spriteSheetLoadedCount,_spriteSheetCount);
	if(_spriteSheetLoadedCount >= _spriteSheetCount ){
		
		loadSpriteSheetSync(_spriteFiles);
		
		//通知外部
		if (_loadSpriteSheetAsyncTarget && _loadSpriteSheetAsyncSelector)
		{
			CCDictionary * dict=CCDictionary::create();
			dict->setObject(CCInteger::create(_spriteSheetLoadedCount), "x");
			dict->setObject(CCInteger::create(_spriteSheetCount), "y");
			(_loadSpriteSheetAsyncTarget->*_loadSpriteSheetAsyncSelector)(dict);
		}
		
		CC_SAFE_RELEASE_NULL(_spriteFiles);
		CC_SAFE_RELEASE_NULL(_loadSpriteSheetAsyncTarget);
	}
	else{
		
		//通知外部
		if (_loadSpriteSheetAsyncTarget && _loadSpriteSheetAsyncSelector)
		{
			CCDictionary * dict=CCDictionary::create();
			dict->setObject(CCInteger::create(_spriteSheetLoadedCount), "x");
			dict->setObject(CCInteger::create(_spriteSheetCount), "y");
			(_loadSpriteSheetAsyncTarget->*_loadSpriteSheetAsyncSelector)(dict);
		}
	}

}
NS_KAI_END