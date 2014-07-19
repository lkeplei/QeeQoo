//
//  GameEffect.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-12-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameEffect.h"
NS_KAI_BEGIN

GameEffect::GameEffect():_playEffectCallback(NULL){

}

GameEffect::~GameEffect(){

}

GameEffectParticle * GameEffectParticle::create(const char *plistFile)
{
    GameEffectParticle *pRet = new GameEffectParticle();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

void GameEffectParticle::handleParticleSystemDestory(CCParticleSystem * particleSystem){
	if (_playEffectCallback) {
		_playEffectCallback->playEffectCompleted(this);
	}
}


ExplosionParticle * ExplosionParticle::create(const char *plistFile)
{
    ExplosionParticle *pRet = new ExplosionParticle();
    if (pRet && pRet->initWithFile(plistFile))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return pRet;
}

bool ExplosionParticle::initWithFile(const char *plistFile){
	m_maxExplosionRadius = 0.0f;
	
	bool bRet = false;
    m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plistFile);
    CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(m_sPlistFile.c_str());
	
    CCAssert( dict != NULL, "Particles: file not found");
    bRet = this->initWithDictionary(dict);
	
	CCString * maxExplosionRadius = (CCString *)dict->objectForKey("maxExplosionRadius");
	if (maxExplosionRadius) {
		m_maxExplosionRadius = maxExplosionRadius->floatValue() * 0.5;
	}
	
    dict->release();
	
	this->setDelegate(this);
	
	return bRet;
}

void ExplosionParticle::initParticle(tCCParticle* particle)
{
    // timeToLive
    // no negative life. prevent division by 0
    particle->timeToLive = m_fLife + m_fLifeVar * CCRANDOM_MINUS1_1();
    particle->timeToLive = MAX(0, particle->timeToLive);
	
    // position
    particle->pos.x = m_tSourcePosition.x + m_tPosVar.x * CCRANDOM_MINUS1_1();
	
    particle->pos.y = m_tSourcePosition.y + m_tPosVar.y * CCRANDOM_MINUS1_1();
	
	
    // Color
    ccColor4F start;
    start.r = clampf(m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    start.g = clampf(m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    start.b = clampf(m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    start.a = clampf(m_tStartColor.a + m_tStartColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);
	
    ccColor4F end;
    end.r = clampf(m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
    end.g = clampf(m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
    end.b = clampf(m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
    end.a = clampf(m_tEndColor.a + m_tEndColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);
	
    particle->color = start;
    particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
    particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
    particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
    particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;
	
    // size
    float startS = m_fStartSize + m_fStartSizeVar * CCRANDOM_MINUS1_1();
    startS = MAX(0, startS); // No negative value
	
    particle->size = startS;
	
    if( m_fEndSize == kCCParticleStartSizeEqualToEndSize )
    {
        particle->deltaSize = 0;
    }
    else
    {
        float endS = m_fEndSize + m_fEndSizeVar * CCRANDOM_MINUS1_1();
        endS = MAX(0, endS); // No negative values
        particle->deltaSize = (endS - startS) / particle->timeToLive;
    }
	
    // rotation
    //float startA = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1();
    //float endA = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1();
    //particle->rotation = startA;
    //particle->deltaRotation = (endA - startA) / particle->timeToLive;
	
	
	float directionAngle = m_fAngle + m_fAngleVar * CCRANDOM_MINUS1_1();
	
	particle->rotation = -directionAngle;
	particle->deltaRotation = 0;
	
    // position
    if( m_ePositionType == kCCPositionTypeFree )
    {
        particle->startPos = this->convertToWorldSpace(CCPointZero);
    }
    else if ( m_ePositionType == kCCPositionTypeRelative )
    {
        particle->startPos = m_tPosition;
    }
	
    // direction
    float a = CC_DEGREES_TO_RADIANS(directionAngle);
	
    // Mode Gravity: A
    if (m_nEmitterMode == kCCParticleModeGravity)
    {
        CCPoint v(cosf( a ), sinf( a ));
        float s = modeA.speed + modeA.speedVar * CCRANDOM_MINUS1_1();
		
        // direction
        particle->modeA.dir = ccpMult( v, s );
		
        // radial accel
        particle->modeA.radialAccel = modeA.radialAccel + modeA.radialAccelVar * CCRANDOM_MINUS1_1();
		
		
        // tangential accel
        particle->modeA.tangentialAccel = modeA.tangentialAccel + modeA.tangentialAccelVar * CCRANDOM_MINUS1_1();
		
    }
	
    // Mode Radius: B
    else
    {
        // Set the default diameter of the particle from the source position
        float startRadius = modeB.startRadius + modeB.startRadiusVar * CCRANDOM_MINUS1_1();
        float endRadius = modeB.endRadius + modeB.endRadiusVar * CCRANDOM_MINUS1_1();
		
        particle->modeB.radius = startRadius;
		
        if(modeB.endRadius == kCCParticleStartRadiusEqualToEndRadius)
        {
            particle->modeB.deltaRadius = 0;
        }
        else
        {
            particle->modeB.deltaRadius = (endRadius - startRadius) / particle->timeToLive;
        }
		
        particle->modeB.angle = a;
        particle->modeB.degreesPerSecond = CC_DEGREES_TO_RADIANS(modeB.rotatePerSecond + modeB.rotatePerSecondVar * CCRANDOM_MINUS1_1());
    }    
}

bool ExplosionParticle::updateParticleExtra(tCCParticle *p,float elapsed,float dt){

	if (m_maxExplosionRadius > 0 && ccpDistance(CCPoint(0,0),p->pos) > m_maxExplosionRadius) {
		p->timeToLive = 0.02 * CCRANDOM_0_1();
	}
	
	return true;
}

NS_KAI_END