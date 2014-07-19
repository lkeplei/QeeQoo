//
//  CCHBBezier.h
//  ibubble
//
//  Created by Ryan Yuan on 13-1-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_CCHBBezier_h
#define ibubble_CCHBBezier_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
NS_KAI_BEGIN

class CCHBBezier : public CCActionInterval
{
protected:
	CCArray * arrayOfControlPoints;
public:
	CCHBBezier();
	~CCHBBezier();
	static CCHBBezier * actionWithDuration(float duration ,CCArray * controlPos);
	bool initWithDuration(float duration ,CCArray * controlPos);
	virtual CCObject* copyWithZone(CCZone* pZone);
	void startWithTarget(CCNode * aTarget);
	void stop();
	void update(float t);
	CCActionInterval *reverse();
};
NS_KAI_END

#endif
