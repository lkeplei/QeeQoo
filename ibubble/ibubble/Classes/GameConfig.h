//
//  GameConfig.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameConfig_h
#define ibubble_GameConfig_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"

USING_NS_CC;

NS_KAI_BEGIN
class  GameConfig : public CCObject
{
protected:
	GameConfig();
	virtual ~GameConfig();
	/** init the new created instance of GameConfig*/
	bool init(void);
public:
    /** returns a shared instance of GameConfig */
    static GameConfig *sharedInstance(void);
    /** purge the shared instance of GameConfig */
    static void purgeInstance(void);
	
	CCObject * getNpcValue(const std::string & key);
	CCObject * getLevelsValue(const std::string & key);
	CCObject * getBox2dFilterValue(const std::string & key);
	CCObject * getBox2dValue(const std::string & key);
	
	CCObject * getValue(const std::string & key);
	
	void setValue(const std::string & key,CCObject * value);

	bool isDebuging() const;
	void setIsDebuging(const bool value);
	uint32_t box2dVelocityIterations()const{return _box2dVelocityIterations;}
    uint32_t box2dPositionIterations()const{return _box2dPositionIterations;}
	
	uint32_t topLevelId() const;
	void 	 setTopLevelId(const uint32_t value);
private:
	std::string 	_version;
	std::string 	_build;
	std::string 	_platform;
	CCDictionary *  _properties;
	CCDictionary *  _npc;
	CCDictionary *  _levels;
	CCDictionary *  _box2d;
	CCDictionary *  _box2dfilter;
	
	uint32_t		_simpleProperties;
	uint32_t 		_box2dVelocityIterations;
    uint32_t 		_box2dPositionIterations;
	uint32_t	    _topLevelId;
	int32_t         _maxReliveTime;

};
NS_KAI_END
#endif
