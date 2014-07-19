//
//  CCHBBezier.cpp
//  ibubble
//
//  Created by Ryan Yuan on 13-1-2.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "CCHBBezier.h"
NS_KAI_BEGIN

//阶乘
static inline int factorial(int n){
    int result = 1;
    while (n) {
        result *=n;
        n--;
    }
    return result;
}

//杨辉三角
static inline int pascalTriangleRatio(int n,int i){
    int result = 0;
    result = factorial(n)/(factorial(i)*factorial(n-i));
    return result;
}

//贝塞尔
static inline CCPoint bezieerat(CCArray *points,float t){
    float px =0;
    float py = 0;
    int i=0;
	int n=points->count()-1;
    while (i<=n) {
		CCPoint * p = (CCPoint *)points->objectAtIndex(i);
        px += pascalTriangleRatio(n, i)*p->x*powf(t, i)*powf(1-t, n-i);
        py += pascalTriangleRatio(n, i)*p->y*powf(t, i)*powf(1-t, n-i);
        i++;
    }
    CCPoint m;
    m.x = px;
    m.y = py;
    return m;
}

static inline float dt_cell_bezieerat(int n,int i,float t){
    float px =0;
    int _i=0;
	int _n=(n-i);
    while (_i <= _n) {
        px += (((_i % 2 )!= 0)? -1:1 ) * pascalTriangleRatio(_n, _i)*(_i+i) * powf(t, (i+_i-1));
        _i++;
    }
    return px;
}

static inline CCPoint dt_bezieerat(CCArray *points,float t){
    float px =0;
    float py = 0;
    int i=0;
	int n=points->count()-1;
    while (i<=n) {
		CCPoint * p = (CCPoint *)points->objectAtIndex(i);
        px += pascalTriangleRatio(n, i)*p->x*dt_cell_bezieerat(n,i,t);
        py += pascalTriangleRatio(n, i)*p->y*dt_cell_bezieerat(n,i,t);
        i++;
    }
    CCPoint m;
    m.x = px;
    m.y = py;
    return m;
}

CCHBBezier::CCHBBezier():arrayOfControlPoints(NULL){
	arrayOfControlPoints=CCArray::createWithCapacity(3);
	arrayOfControlPoints->retain();
}

CCHBBezier::~CCHBBezier(){
	CC_SAFE_RELEASE_NULL(arrayOfControlPoints);
}

CCHBBezier * CCHBBezier::actionWithDuration(float duration ,CCArray * controlPos){
	CCHBBezier * shake=new CCHBBezier;
	if (shake->initWithDuration(duration,controlPos)) {
		shake->autorelease();
	}
	else {
		CC_SAFE_RELEASE_NULL(shake);
	}
	return shake;
}

bool  CCHBBezier::initWithDuration(float duration ,CCArray* controlPos){
	
	if(CCActionInterval::initWithDuration(duration)){
		
		for (int i=0; i<controlPos->count(); i++) {
			CCPoint * p= new CCPoint(*(CCPoint *)controlPos->objectAtIndex(i));
			arrayOfControlPoints->addObject(p);
			CC_SAFE_RELEASE_NULL(p);
		}
		
		return true;
	}
	else {
		return false;
	}
}

CCObject* CCHBBezier::copyWithZone(CCZone* pZone)
{
	return CCHBBezier::actionWithDuration(m_fDuration,arrayOfControlPoints);
}

void CCHBBezier::startWithTarget(CCNode * aTarget)
{
	CCActionInterval::startWithTarget(aTarget);
	
}

void CCHBBezier::stop()
{
	CCActionInterval::stop();
}

void CCHBBezier::update(float t)
{
	if (arrayOfControlPoints) {
		
		CCPoint p = bezieerat(arrayOfControlPoints, t);
		CCPoint p_dt = dt_bezieerat(arrayOfControlPoints, t);
		float angle = atan2(p_dt.y,p_dt.x) * 180 / M_PI;
		m_pTarget->setPosition(p);
		m_pTarget->setRotation(-angle);
		/*
		CCSprite* pSprite = CCSprite::createWithSpriteFrameName("m_5_s_3.png");
		pSprite->setAnchorPoint(ccp(0, 0.5));
		pSprite->setPosition(p);
		pSprite->setRotation(-angle);
		m_pTarget->getParent()->addChild(pSprite);
		*/
	}
}

CCActionInterval *CCHBBezier::reverse(){
	
	CCArray * _arrayOfControlPoints=CCArray::createWithCapacity(arrayOfControlPoints->count());
	for (int i = arrayOfControlPoints->count()-1; i >= 0; i--) {
		_arrayOfControlPoints->addObject(arrayOfControlPoints->objectAtIndex(i));
	}
	return CCHBBezier::actionWithDuration(m_fDuration,_arrayOfControlPoints);
}

NS_KAI_END
