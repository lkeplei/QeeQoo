//
//  GameAIScript.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameAIScript_h
#define ibubble_GameAIScript_h

#include "cocos2d.h"
#include "GameMacros.h"
#include <string>
USING_NS_CC;
NS_KAI_BEGIN
class GameObject;
class  GameAIScript : public CCObject
{
public:
	virtual bool runScript(GameObject * aGameObject,CCDictionary * dic,float dt) = 0;
};


/*泡泡自己的AI*/
class  BubbleAIScript : public GameAIScript
{
public:
	virtual bool runScript(GameObject * aGameObject,CCDictionary * dic,float dt);
	
};

/*子弹的直线运动轨迹AI*/
class  BulletAIScript : public GameAIScript
{
public:
	virtual bool runScript(GameObject * aGameObject,CCDictionary * dic,float dt);
	
};

/*子弹的Bezier曲线运动轨迹AI*/

class  BulletBezierAIScript : public GameAIScript
{
public:
	virtual bool runScript(GameObject * aGameObject,CCDictionary * dic,float dt);
};




NS_KAI_END
#endif
