//
//  CCShake.h
//  ibubble
//
//  Created by Ryan Yuan on 12-12-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_CCShake_h
#define ibubble_CCShake_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
NS_KAI_BEGIN

class CCShake : public CCActionInterval
{
protected:
	float shakeInterval;
	float nextShake;
	bool dampening;
	CCPoint startAmplitude;
	CCPoint amplitude;
	CCPoint last;
public:
	static CCShake * actionWithDuration(float t ,const CCPoint &pamplitude);
	static CCShake * actionWithDuration(float t ,const CCPoint &pamplitude,bool pdampening);
	static CCShake * actionWithDuration(float t ,const CCPoint &pamplitude,int pshakeNum);
	static CCShake * actionWithDuration(float t ,const CCPoint &pamplitude,bool pdampening,int pshakeNum);
	bool initWithDuration(float t ,const CCPoint &pamplitude,bool pdampening,int pshakeNum);
	virtual CCObject* copyWithZone(CCZone* pZone);
	void startWithTarget(CCNode * aTarget);
	void stop();
	void update(float t);
};
NS_KAI_END

#endif
