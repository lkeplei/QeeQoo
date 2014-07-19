//
//  GameEffect.h
//  ibubble
//
//  Created by Ryan Yuan on 12-12-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameEffect_h
#define ibubble_GameEffect_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
USING_NS_CC;

NS_KAI_BEGIN
class PlayEffectCallback{
public:
	virtual void playEffectCompleted(CCObject * effectObj)=0;
};

class GameEffect{
public:
	GameEffect();
	virtual ~GameEffect();
	void setPlayEffectCallback(PlayEffectCallback* playEffectCallback){ _playEffectCallback = playEffectCallback; }
protected:
	PlayEffectCallback * _playEffectCallback;
};


class GameEffectParticle:public CCParticleSystemQuad,public GameEffect{
public:
	static GameEffectParticle * create(const char *plistFile);
	virtual void handleParticleSystemDestory(CCParticleSystem * particleSystem);
};



class ExplosionParticle:public GameEffectParticle,public CCParticleSystemDelegate{
public:
	static ExplosionParticle * create(const char *plistFile);
	bool initWithFile(const char *plistFile);
	virtual void initParticle(tCCParticle* particle);
	virtual bool updateParticleExtra(tCCParticle *p,float elapsed,float dt);
private:
	float m_maxExplosionRadius;
};




NS_KAI_END



#endif
