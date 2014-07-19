//
//  GameCallback.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameCallback.h"
#include "GameObject.h"
#include "GameModle.h"
#include "GameCallbackFactory.h"
#include "GameConstant.h"
#include "CCHBBezier.h"

NS_KAI_BEGIN

bool CallbackPlayEffect::doCallback(GameObject * obj,CCDictionary * dic,float dt) {
	
	/*
	GameModle * _sharedInstance = GameModle::sharedInstance();

	CCDictionary * pdict = CCDictionary::create();
	pdict->setObject(GameObject::point2Dict(obj->getPosition()), KStrPosition);
	_sharedInstance->playEffect(KStrBubbleExplosion,_sharedInstance->battleGameObjectlayer(),obj,obj->getPosition(),pdict);
	
	//假如存在参数
	if (dic) {
		CCString * param=(CCString*)dic->objectForKey(KStrParam);
		if (param) {
			obj->doCallback(param->getCString());
		}
	}
	*/
	
	return true;
}


bool CallbackBubbleExplosion::doCallback(GameObject * obj,CCDictionary * dic,float dt) {
	CCLOG(" CallbackBubbleExplosion::doAction");
	GameModle * _sharedInstance=GameModle::sharedInstance();
	
	obj->resetLinearVelocity();
	obj->resetAngularVelocity();
	uint32 state=obj->getState();
	obj->setState((state & ~GameObject::K_STATE_MOVING)|GameObject::K_STATE_FALL);
	
	CCDictionary * pdict = CCDictionary::create();
	pdict->setObject(GameObject::point2Dict(obj->getPosition()), KStrPosition);
	_sharedInstance->playEffect(KStrBubbleExplosion,_sharedInstance->battleGameObjectlayer(),obj,obj->getPosition(),pdict);
	
	//假如存在参数
	if (dic) {
		CCString * param=(CCString*)dic->objectForKey(KStrParam);
		if (param) {
			obj->doCallback(param->getCString());
		}
	}
	
	//精灵执行自己的动画
	obj->runAnimation(KStrFreeFall);
	
	return true;
}

bool CallbackRocketExplosion::doCallback(GameObject * obj,CCDictionary * dic,float dt) {
	CCLOG("CallbackRocketExplosion::doAction");
	GameModle * _sharedInstance = GameModle::sharedInstance();
	
	obj->resetLinearVelocity();
	obj->resetAngularVelocity();
	
	uint32 state = obj->getState();
	obj->setState((state & ~GameObject::K_STATE_MOVING)|GameObject::K_STATE_FALL);
	CCDictionary * pdict = CCDictionary::create();
	pdict->setObject(GameObject::point2Dict(obj->getPosition()), KStrPosition);
	
	_sharedInstance->playEffect(KStrExplosionEffect,
								_sharedInstance->battleGameObjectlayer(),
								obj,
								obj->getPosition(),
								pdict);
	
	//假如存在参数
	if (dic) {
		CCString * param =(CCString*)dic->objectForKey(KStrParam);
		if (param) {
			obj->doCallback(param->getCString());
		}
	}
	
	//精灵执行自己的动画
	obj->runAnimation(KStrFreeFall);
	
	return true;
}

bool CallbackExplode::doCallback(GameObject * aGameObject,CCDictionary * dic,float dt) {
	if (dic && aGameObject) {
		
		//获得效果本身影响的范围
		CCDictionary * property = (CCDictionary *)aGameObject->getValue(KStrProperty);
		CCString * atkStr = (CCString *)property->objectForKey(KStrATK);
		CCString * atkRangeStr = (CCString *)property->objectForKey(KStrATK_RANGE);
		
		float atk_range_org = (atkRangeStr?atkRangeStr->floatValue():0) * 0.5;
		float atk = atkStr? atkStr->floatValue() : 0;
		
		//获得对应的效果播放的中心点
		CCDictionary * pDict=(CCDictionary *)dic->objectForKey(KStrPosition);
		CCString * paramStr=(CCString *)dic->objectForKey(KStrParam);
		float scale = !paramStr ? 1.0 : paramStr->floatValue();
		float atk_range = scale * atk_range_org;
		
		CCPoint p = GameObject::dict2Point(pDict);
		GameModle * _sharedInstance = GameModle::sharedInstance();
		CCDictionary * objects = _sharedInstance->gameObjectsDict();
		CCDictElement * pDictElement = NULL;
		CCDICT_FOREACH(objects, pDictElement){
			int32_t classid=pDictElement->getIntKey();
			if(KNPCTypeId == GameObject::typeIdOfClassId(classid)){
				CCSet * set=(CCSet*)pDictElement->getObject();
				CCSetIterator iter;
				for (iter = set->begin(); iter != set->end(); ++iter)
				{
					GameObject * tmp=(GameObject *)(*iter);
					if (tmp->isActive()) {
						
						float r = tmp->getContentSize().width * 0.5;
						//检测可以攻击到泡泡
						if(GameObject::isCircleContact(p,atk_range,tmp->getPosition(),r)){
							//处理进入到攻击范围的泡泡
							handleExplode(tmp,NULL,atk,aGameObject);
						}
					}
				}
			}
		}
		
		handleExplodeEnd(aGameObject,NULL,atk);
	}
	return true;
}

void CallbackBubbleExplosionEffect::handleExplode(GameObject * tmp,
												  CCDictionary * dic,
												  float atk,
												  GameObject * atkObject){
	
	if (!(tmp->getState() & GameObject::K_STATE_FALL) &&
		tmp->getTypeId() == KNPCTypeId ) {
		
		GameModle * _sharedInstance = GameModle::sharedInstance();
		int32_t checkValue = (int32_t)atkObject;
		if (!tmp->checkExistInExtraMap(KStrATK_SOURCE,checkValue)) {
			tmp->addValueInExtraMap(KStrATK_SOURCE, checkValue);
			GameObject::reduceHP(tmp,atk,atkObject);
			if (tmp->getHP() <= 0) {
				_sharedInstance->bubbleExplosion(tmp,KBubbleToExplosionDelay);
			}
		}

	}
}

void CallbackBubbleExplosionEffect::handleExplodeEnd(GameObject * tmp,CCDictionary * dic,float dt){
	
}


void CallbackRocketExplosionEffect::handleExplode(GameObject * tmp,
												  CCDictionary * dic,
												  float atk,
												  GameObject * atkObject){
	
	if (!(tmp->getState() & GameObject::K_STATE_FALL) && tmp->getTypeId() == KNPCTypeId) {
		GameModle * _sharedInstance = GameModle::sharedInstance();
		_sharedInstance->bubbleExplosion(tmp,KBubbleToExplosionDelay);
	}
}

void CallbackRocketExplosionEffect::handleExplodeEnd(GameObject * tmp,CCDictionary * dic,float dt){
	tmp->setIsActive(false);
}


bool CallbackBubbleShoot::doCallback(GameObject * aGameObject,CCDictionary * dic,float dt) {
	
	GameModle * _sharedInstance=GameModle::sharedInstance();
	
	//自动选择目标
	const CCPoint target_pos = _sharedInstance->queryTargetPoint(aGameObject->getPosition());
	
	CCDictionary * param = (CCDictionary *)dic->objectForKey(KStrParam);
	CCString * bullet=(CCString *)param->objectForKey(KStrBullet);
	CCString * speed=(CCString *)param->objectForKey(KStrSpeed);
	CCString * effectId=(CCString *)param->objectForKey(KStrEffect);
	_sharedInstance->playEffectById(effectId ? effectId->getCString() : NULL, NULL,CCPointZero);
	
	GameObject * obj=_sharedInstance->generateGameObjectByClassId(bullet->uintValue());
	if (obj) {
		obj->setOwner(aGameObject);
		const CCPoint  original_pos = aGameObject->getPosition();
		obj->setPositionWithPhysics(original_pos);

		obj->setValue(KStrSpeed,speed);
		obj->resetLinearVelocity();
		obj->resetAngularVelocity();
		uint32 state=obj->getState();
		obj->setState(state | GameObject::K_STATE_MOVING);
		
		
		//产生一个高阶Bezier
		CCPoint subPos = ccpSub(original_pos, target_pos);
		float distance = ccpDistance(original_pos, target_pos);
		float time = distance/(speed->floatValue() * PTM_RATIO);//注意配置的速度都是米为单位
		CCSize winSize=CCDirector::sharedDirector()->getWinSize();

		CCArray *controlPos= CCArray::createWithCapacity(5);
		
		CCPoint * p1 = new CCPoint(original_pos);
		controlPos->addObject(p1);
		
		
		CCPoint  offset;
		
		CCPoint * p2 = new CCPoint();
		controlPos->addObject(p2);
		CCPoint * p3 = new CCPoint();
		controlPos->addObject(p3);
		if (abs(subPos.x) < abs(subPos.y)) {
			
			offset = ccpSub(target_pos,*p1);
			
			float k = offset.y / offset.x;
			float m = target_pos.y - target_pos.x * k;
			
			p2->y = p1->y + offset.y * (0.1 + 0.4 * CCRANDOM_0_1());
			p2->x = (p2->y * k + m) + winSize.width * (0.20 + 0.2 * CCRANDOM_0_1());
			p2->x = p2->x > winSize.width  ? winSize.width : p2->x;  
			p2->x = p2->x < 0 ? 0 : p2->x;
			
			
			offset = ccpSub(target_pos,*p2);
			p3->y = p2->y + offset.y * (0.3 + 0.2 * CCRANDOM_0_1());
			p3->x = (p3->y * k + m) - winSize.width * (0.20 + 0.2 * CCRANDOM_0_1());
			p3->x = p3->x > winSize.width  ? winSize.width : p3->x;  
			p3->x = p3->x < 0 ? 0 : p3->x;
			
		}
		else
		{
			offset=ccpSub(target_pos,*p1);
			
			float k = offset.y / offset.x;
			float m = target_pos.y- target_pos.x * k;
			
			p2->x = p1->x + offset.x * (0.1 + 0.4 * CCRANDOM_0_1());
			p2->y = (p2->x * k + m) + winSize.height * (0.20 + 0.2 * CCRANDOM_0_1());
			p2->y = p2->y > winSize.height  ? winSize.height : p2->y; //避免出界面
			
			offset=ccpSub(target_pos,*p2);
			p3->x = p2->x + offset.x * (0.3 + 0.2 * CCRANDOM_0_1());
			p3->y = (p3->x * k + m) - winSize.height * (0.20 + 0.2 * CCRANDOM_0_1());
			p3->y = p3->y < 0 ? 0 : p3->y;
		}

		p1->release();
		p2->release();
		p3->release();
		
		CCPoint * pN = new CCPoint(target_pos);
		controlPos->addObject(pN);
		pN->release();
		
		CCHBBezier * bez = CCHBBezier::actionWithDuration(time,controlPos);
		
		//完成动画
		CCDictionary *effectDict=CCDictionary::create();
		effectDict->setObject(obj, KStrOwner);
		
		CCAction * bezierAction = CCSequence::createWithTwoActions(bez,CCCallFuncO::create(this,callfuncO_selector(CallbackBubbleShoot::bezierCompleted),effectDict));
		
		obj->runAction(bezierAction);
	}
	
	return true;
}

void CallbackBubbleShoot::bezierCompleted(CCObject * obj){
	CCDictionary *effectDict = (CCDictionary *)obj;
	if (effectDict) {
		GameObject * owner = (GameObject *)effectDict->objectForKey(KStrOwner);
		if (owner->isActive()) {
			owner->doCallback(KStrExplosion,effectDict);
		}
	}
}

bool CallbackBubbleShootLaser::doCallback(GameObject * aGameObject,CCDictionary * dic,float dt) {
	
	GameModle * _sharedInstance=GameModle::sharedInstance();
	
	//目标中点
	CCDictionary * pDict=(CCDictionary *)dic->objectForKey(KStrPosition);
	CCPoint p = GameObject::dict2Point(pDict);
	CCDictionary * param = (CCDictionary *)dic->objectForKey(KStrParam);
	CCString * bullet=(CCString *)param->objectForKey(KStrBullet);
	CCString * speed=(CCString *)param->objectForKey(KStrSpeed);
	CCString * effectId=(CCString *)param->objectForKey(KStrEffect);
	_sharedInstance->playEffectById(effectId ? effectId->getCString() : NULL, NULL,CCPointZero);
	GameObject * obj=_sharedInstance->generateGameObjectByClassId(bullet->uintValue());
	if (obj) {
		
		obj->setOwner(aGameObject);
		
		const CCPoint  original_pos= aGameObject->getPosition();
		//obj->setPositionWithPhysics(original_pos);
		CCPoint direction=GameObject::unitVector(original_pos,p);
		obj->setPositionWithPhysics(ccpAdd(original_pos,ccpMult(direction, obj->getContentSize().width * 0.5)));
		
		float angle=GameObject::angleFromVector(direction);
		obj->setRotationWithPhysics(angle);
		
		obj->setValue(KStrSpeed,speed);
		obj->setValue(KStrDirection,GameObject::point2Dict(direction));
		obj->resetLinearVelocity();
		obj->resetAngularVelocity();
		
		uint32 state=obj->getState();
		obj->setState(state | GameObject::K_STATE_MOVING);
	}
	return true;
}

bool CallbackOpenSkill::doCallback(GameObject * aGameObject,CCDictionary * dic,float dt){
	uint32 state=aGameObject->getState();
	if (!(state & GameObject::K_STATE_SKILL_OPENED)) {
		state=state|GameObject::K_STATE_SKILL_OPENED;
		aGameObject->setState(state);
		return this->doOpenSkill(aGameObject,dic,dt);
	}
	return false;
}

bool CallbackOpenSkillShoot::doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt){
	return aGameObject->doCallback(KStrShoot,dic,dt);
}

bool CallbackOpenSkillShootLaser::doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt){
	return aGameObject->doCallback(KStrShoot,dic,dt);
}

bool CallbackOpenSkillSpurtFire::doOpenSkill(GameObject * aGameObject,CCDictionary * dic,float dt){
	
	
	//假如存在参数
	CCDictionary * dict = (CCDictionary *)aGameObject->getValue(KStrProperty);
	if (dict) {
		dict = CCDictionary::create();
	}
	
	CCObject * param = dic->objectForKey(KStrParam);
	CCAssert(param != NULL, "CallbackOpenSkillSpurtFire::doOpenSkill param should not be NULL");
	//dict->setObject(param, KStrSpurtFireAtk);
	aGameObject->setValue(KStrSpurtFireAtk, param);
	//修改刚体
	aGameObject->destoryFixture(KStrSpurtFire);
	aGameObject->createFixtureToBody(KStrSpurtFire);
	
	return true;
}

NS_KAI_END
