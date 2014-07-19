//
//  GameCallback.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameCallback_h
#define ibubble_GameCallback_h

#include "cocos2d.h"
#include "GameMacros.h"
#include <string>
USING_NS_CC;
NS_KAI_BEGIN
class GameObject;
class  GameCallback : public CCObject
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt) = 0;
};
/** 特效播放 */
class  CallbackPlayEffect : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
};

/** 爆炸*/
class  CallbackExplode : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
	virtual void handleExplode(GameObject * aGameObject,CCDictionary * dic,float atk,GameObject * atkObject) = 0;
	virtual void handleExplodeEnd(GameObject * aGameObject,CCDictionary * dic,float atk) = 0;
};

/** 泡泡爆炸*/
class  CallbackBubbleExplosion : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
};

/** 泡泡爆炸之后引爆其他泡泡 */
class  CallbackBubbleExplosionEffect : public CallbackExplode
{
public:
	virtual void handleExplode(GameObject * aGameObject,CCDictionary * dic,float atk,GameObject * atkObject);
	virtual void handleExplodeEnd(GameObject * aGameObject,CCDictionary * dic,float atk);
};

/** 爆炸之后引爆其他泡泡 */
class  CallbackRocketExplosion : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
};

/** 导弹爆炸之后引爆其他泡泡 */
class  CallbackRocketExplosionEffect : public CallbackExplode
{
public:
	virtual void handleExplode(GameObject * aGameObject,CCDictionary * dic,float atk,GameObject * atkObject);
	virtual void handleExplodeEnd(GameObject * aGameObject,CCDictionary * dic,float atk);
};


/** 泡泡爆炸之后射击其他泡泡 */
class  CallbackBubbleShoot : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
	void bezierCompleted(CCObject * effectObj);
	
};

/** 泡泡爆炸之后使用激光射击其他泡泡 */
class  CallbackBubbleShootLaser : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
};

/** 释放攻击 */
class  CallbackOpenSkill : public GameCallback
{
public:
	virtual bool doCallback(GameObject * aGameObject,CCDictionary * dic,float dt);
	virtual bool doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt)=0;
};

/** 释放自动导弹攻击 */
class  CallbackOpenSkillShoot : public CallbackOpenSkill
{
public:
	virtual bool doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt);
};


/** 释放镭射攻击 */
class  CallbackOpenSkillShootLaser : public CallbackOpenSkill
{
public:
	virtual bool doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt);
};

/** 释放喷射火焰攻击 */
class  CallbackOpenSkillSpurtFire : public CallbackOpenSkill
{
public:
	virtual bool doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt);
};

NS_KAI_END

#endif
