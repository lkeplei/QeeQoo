//
//  GameAIScriptFactory.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameAIScriptFactory_h
#define ibubble_GameAIScriptFactory_h

#include "cocos2d.h"
#include "GameMacros.h"
#include <string>
#include "GameAIScript.h"
USING_NS_CC;
NS_KAI_BEGIN
class  GameAIScriptFactory : public CCObject
{
protected:
	GameAIScriptFactory();
	virtual ~GameAIScriptFactory();
public:
    /** returns a shared instance of GameAIScriptFactory */
    static GameAIScriptFactory *sharedInstance(void);
    /** purge the shared instance of GameAIScriptFactory */
    static void purgeInstance(void);
	/** init the new created instance of GameAIScriptFactory*/
	bool init(void);
	GameAIScript * getGameAIScript(const std::string & key);
private:
	CCDictionary * _properties;
};
NS_KAI_END
#endif
