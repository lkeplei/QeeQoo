//
//  GameResoureManager.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameResoureManager_h
#define ibubble_GameResoureManager_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"

USING_NS_CC;
NS_KAI_BEGIN

class  GameResoureManager : public CCObject
{
protected:
	GameResoureManager();
	virtual ~GameResoureManager();
public:
    /** returns a shared instance of GameResoureManager */
    static GameResoureManager *sharedInstance(void);
    /** purge the shared instance of GameResoureManager */
    static void purgeInstance(void);
	/** init the new created instance of GameResoureManager*/
	bool init(void);
	
	CCObject * getValue(const std::string & key);
	
	void setValue(const std::string & key,CCObject * value);
#pragma mark-
#pragma mark loadSpriteSheet
	void loadSpriteSheetAsync(CCArray * spriteFiles,CCObject *target, SEL_CallFuncO selector);
	void loadSpriteSheetSync(CCArray * spriteFiles);
	void loadSpriteSheetCallBack(CCObject * obj);

private:
	CCDictionary * _properties;
	uint32_t _spriteSheetCount;
	uint32_t _spriteSheetLoadedCount;
	CCArray * _spriteFiles;
	CCObject * _loadSpriteSheetAsyncTarget;
	SEL_CallFuncO _loadSpriteSheetAsyncSelector;
};
NS_KAI_END
#endif
