//
//  CCShake.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-12-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CCShake.h"
#define CCSHAKE_EVERY_FRAME	0
NS_KAI_BEGIN
CCShake * CCShake::actionWithDuration(float t ,const CCPoint &pamplitude){
	return actionWithDuration(t,pamplitude,false,CCSHAKE_EVERY_FRAME);
}

CCShake * CCShake::actionWithDuration(float t ,const CCPoint &pamplitude,bool pdampening){
	return actionWithDuration(t,pamplitude,pdampening,CCSHAKE_EVERY_FRAME);
}
CCShake * CCShake::actionWithDuration(float t ,const CCPoint &pamplitude,int pshakeNum){
	return actionWithDuration(t,pamplitude,false,pshakeNum);
}
CCShake * CCShake::actionWithDuration(float t ,const CCPoint &pamplitude,bool pdampening,int pshakeNum){
	CCShake * shake = new CCShake;
	if (shake->initWithDuration(t,pamplitude,pdampening,pshakeNum)) {
		shake->autorelease();
	}
	else {
		CC_SAFE_RELEASE_NULL(shake);
	}
	return shake;
}

bool  CCShake::initWithDuration(float t ,const CCPoint &pamplitude,bool pdampening,int pshakeNum){
	
	if(CCActionInterval::initWithDuration(t)){
		
		startAmplitude	= pamplitude;
		dampening	= pdampening;
		
		// calculate shake intervals based on the number of shakes
		if(pshakeNum == CCSHAKE_EVERY_FRAME)
			shakeInterval = 0;
		else
			shakeInterval = 1.f/pshakeNum;
		
		return true;
	}
	else {
		return false;
	}
}

CCObject* CCShake::copyWithZone(CCZone* pZone)
{
	CCShake *copy =new CCShake;
	copy->initWithDuration(m_fDuration, amplitude, dampening, shakeInterval == 0 ? 0 : 1/shakeInterval);
	copy->autorelease();
	return copy;
}

void CCShake::startWithTarget(CCNode * aTarget)
{
	CCActionInterval::startWithTarget(aTarget);
	amplitude	= startAmplitude;
	last		= CCPoint();
	nextShake	= 0;
}

void CCShake::stop()
{
	// undo the last shake
	m_pTarget->setPosition(ccpSub(((CCNode*)m_pTarget)->getPosition(), last));
	CCActionInterval::stop();
}

void CCShake::update(float t)
{
	// waits until enough time has passed for the next shake
	if(shakeInterval == CCSHAKE_EVERY_FRAME)
	{} // shake every frame!
	else if(t < nextShake)
		return; // haven't reached the next shake point yet
	else
		nextShake += shakeInterval; // proceed with shake this time and increment for next shake goal
		
		// calculate the dampening effect, if being used
		if(dampening)
		{
			float dFactor = (1-t);
			amplitude.x = dFactor * startAmplitude.x;
			amplitude.y = dFactor * startAmplitude.y;
		}
	
	CCPoint newp = ccp((CCRANDOM_0_1()*amplitude.x*2) - amplitude.x,(CCRANDOM_0_1()*amplitude.y*2) - amplitude.y);
	
	// simultaneously un-move the last shake and move the next shake
	m_pTarget->setPosition(ccpAdd(ccpSub(((CCNode*)m_pTarget)->getPosition(), last),newp));
	
	// store the current shake value so it can be un-done
	last = newp;
}



NS_KAI_END