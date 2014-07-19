//
//  BattleAIScript.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_BattleAIScript_h
#define ibubble_BattleAIScript_h
#include "GameAIScript.h"
NS_KAI_BEGIN
class GameObject;
class  BattleAIScript : public GameAIScript
{
public:
	virtual bool runScript(GameObject * aGameObject,CCDictionary * dic,float dt);
	
};
NS_KAI_END
#endif
