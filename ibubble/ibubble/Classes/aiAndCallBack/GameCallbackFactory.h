//
//  GameCallbackFactory.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameCallbackFactory_h
#define ibubble_GameCallbackFactory_h

#include "cocos2d.h"
#include "GameMacros.h"
#include <string>
#include "GameCallback.h"
USING_NS_CC;
NS_KAI_BEGIN
class  GameCallbackFactory : public CCObject
{
protected:
	GameCallbackFactory();
	virtual ~GameCallbackFactory();
public:
    /** returns a shared instance of GameCallbackFactory */
    static GameCallbackFactory *sharedInstance(void);
    /** purge the shared instance of GameCallbackFactory */
    static void purgeInstance(void);
	/** init the new created instance of GameCallbackFactory*/
	bool init(void);
	/** 查找一个callback */
	GameCallback * getGameCallback(const std::string & key);
	/** 执行一个callback */
	bool doCallback(const std::string & key,GameObject * aGameObject = NULL,CCDictionary * dic = NULL,float dt = 0);
private:
	CCDictionary * _properties;
};
NS_KAI_END
#endif
