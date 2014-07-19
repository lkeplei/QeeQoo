//
//  GameObjectFactory.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameObjectFactory_h
#define ibubble_GameObjectFactory_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"

USING_NS_CC;
NS_KAI_BEGIN

class  GameObjectFactory : public CCObject
{
protected:
	GameObjectFactory();
	virtual ~GameObjectFactory();
public:
    /** returns a shared instance of GameObjectFactory */
    static GameObjectFactory *sharedInstance(void);
    /** purge the shared instance of GameObjectFactory */
    static void purgeInstance(void);
	/** init the new created instance of GameObjectFactory*/
	bool init(void);
	
	CCObject * getValue(const std::string & key);
	
	void setValue(const std::string & key,CCObject * value);
private:
	CCDictionary * _properties;
};

NS_KAI_END
#endif
