//
//  GameAIScript.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GameAIScript.h"
#include "GameObject.h"
#include "GameConfig.h"
#include "GameModle.h"
#include "GameConstant.h"
NS_KAI_BEGIN

bool BubbleAIScript::runScript(GameObject * aGameObject,CCDictionary * dic,float dt) {
	if (aGameObject) {
		GameModle * _modlesharedInstance=GameModle::sharedInstance();
		
		b2Body* physicsBody = aGameObject->getPhysicsBody();
		uint32 state = aGameObject->getState();
		
		if (state & GameObject::K_STATE_FALL) {
			
			//修改刚体
			aGameObject->switchFixture(KStrFreeFall);
			//角度旋转
			float currentAngle = -aGameObject->getRotation();
			int tmp = (int)(currentAngle/360);
			currentAngle = abs(tmp) != 0 ? (currentAngle-tmp*360):currentAngle;
			if(abs(currentAngle) > 0){
				
				//旋转正来的速度
				float angleStep = 140.f;//120.f
				if (abs(currentAngle) < 90) {
					angleStep = 80.f;//60.f
				}
				angleStep = angleStep * dt;
				
				if (currentAngle >0) {
					
					if (currentAngle > 180) {
						currentAngle = currentAngle + angleStep;
						currentAngle = currentAngle >= 360?0:currentAngle;	
					}
					else {
						currentAngle = currentAngle - angleStep;
						currentAngle = currentAngle < 0?0:currentAngle;
					}
					
				}
				else{
					
					if (currentAngle < -180) {
						currentAngle = currentAngle - angleStep;
						currentAngle = currentAngle <= -360?0:currentAngle;
					}
					else {
						currentAngle = currentAngle + angleStep;
						currentAngle = currentAngle > 0?0:currentAngle;	
					}
				}

				aGameObject->setRotationWithPhysics(currentAngle);
			}

			
			//增加重力
			
			b2Vec2 point = physicsBody->GetWorldCenter();
			b2Vec2 force;
			force.x = 0.0f;
			force.y = - 7.0f * physicsBody->GetMass(); //下降加速度修改成 7.0f
			physicsBody->ApplyForce(force,point);
	
		}
		else {
			
			if (state & GameObject::K_STATE_MOVING) {
				const float minSpeed = 0.5f;
				const float maxSpeed = 2.0f;
				const float staticSpeed = 0.0000001f;
				
				float _forcer = 0.02f;
				float _speedscale = 1.4f;
				CCString * speedscaleStr = (CCString *)_modlesharedInstance->valueFromCurrentLevel(KStrSpeedScale);
				CCString * forcerStr = (CCString *)_modlesharedInstance->valueFromCurrentLevel(KStrDefalutForcer);
				_forcer	= forcerStr?forcerStr->floatValue():_forcer;
				_speedscale	= speedscaleStr?speedscaleStr->floatValue():_speedscale;
				
				b2Vec2 velocity = physicsBody->GetLinearVelocity();
				float32 speed = velocity.Length();
				
				/**速度太小或者某个方向的速度小于staticSpeed的时候给一个随机的冲量*/
				if (speed < minSpeed 
					||(abs(velocity.x) <= staticSpeed  && abs(velocity.y) > staticSpeed) 
					||(abs(velocity.y) <= staticSpeed  && abs(velocity.x) > staticSpeed)) {
					
					const float forceMax = 2.0;
					const float forceMin = 1.1;
					float randx = forceMax*CCRANDOM_MINUS1_1();
					float randy = forceMax*CCRANDOM_MINUS1_1();
					
					if (randx < 0 && randx > -forceMin) {
						randx -= forceMin;
					}
					
					if (randx >= 0 && randx < forceMin) {
						randx += forceMin;
					}
					
					if (randy < 0 && randy > -forceMin) {
						randy -= forceMin;
					}
					
					if (randy >= 0 && randy < forceMin) {
						randy += forceMin;
					}
					
					b2Vec2 force = b2Vec2(randx,randy);
					
					const CCSize contentSizeInPixels=aGameObject->getContentSize();
					
					b2Vec2 vec;
					vec.x = physicsBody->GetPosition().x - (contentSizeInPixels.width /2.0f) * _forcer * CCRANDOM_MINUS1_1();
					vec.y = physicsBody->GetPosition().y - (contentSizeInPixels.width /2.0f) * _forcer * CCRANDOM_MINUS1_1();
					
					physicsBody->ApplyLinearImpulse(force,vec);
					
				}else if (speed < maxSpeed) {
					physicsBody->SetLinearDamping(0);
				}else if (speed > maxSpeed) {
					physicsBody->SetLinearDamping(0.5);
				}
				
				
				float angularVelocity = abs(physicsBody->GetAngularVelocity());
				const float minAngularSpeed = 0.00001f;
				const float maxAngularSpeed = 1.0f;
				if (angularVelocity < minAngularSpeed) {
					physicsBody->SetAngularDamping(0);
				}else if (angularVelocity > maxAngularSpeed) {
					physicsBody->SetAngularDamping(0.2);
				}
			}
		}
		
		//检查到碰撞(只是处理泡泡和泡泡直接的碰撞,当前为攻击方)
		if (state & GameObject::K_STATE_COLLIDE){
			GameModle * _modlesharedInstance = GameModle::sharedInstance();
			CCString * atkStr = (CCString *)aGameObject->getValue(KStrSpurtFireAtk);
			float atkValue = atkStr ? atkStr->floatValue():0;
			if (atkValue > 0) {
				CCSet * set = _modlesharedInstance->collideObjects(aGameObject);
				if (set) {
					CCSet * result_set = new CCSet;
					for (CCSetIterator iter = set->begin(); iter != set->end(); ++iter)
					{
						GameObject *object = (GameObject *)(*iter);
						if (object->isActive() &&
							object->getTypeId() == KNPCTypeId &&
							!(object->getState() & GameObject::K_STATE_FALL)) {
							
							const int32_t checkValue = (int32_t)aGameObject;
							if (!object->checkExistInExtraMap(KStrSKillATK_SOURCE,checkValue)) {
								object->addValueInExtraMap(KStrSKillATK_SOURCE, checkValue);
								GameObject::reduceHP(object,atkValue,aGameObject);
								if (object->getHP() <= 0) {
									result_set->addObject(object);	
								}
							}
						}
					}
					
					for (CCSetIterator iter = result_set->begin(); iter != result_set->end(); ++iter){
						GameObject * tmp = (GameObject *)(*iter);
						_modlesharedInstance->bubbleExplosion(tmp,KBubbleToExplosionDelay);
					}
					
					CC_SAFE_RELEASE_NULL(result_set);	
				}
			}
			//完成本次碰撞的处理
			aGameObject->setState((aGameObject->getState() & ~GameObject::K_STATE_COLLIDE));
		}
	}
	return true;
}

bool BulletAIScript::runScript(GameObject * aGameObject,CCDictionary * dic,float dt) {
	CCLog("GameObject=0x%x BulletAIScript=0x%x runScript...",aGameObject,this);
	if (aGameObject) {
		b2Body* physicsBody = aGameObject->getPhysicsBody();
		uint32 state = aGameObject->getState();
		if (state & GameObject::K_STATE_MOVING){
			CCDictionary * directionDict = (CCDictionary *)aGameObject->getValue(KStrDirection);
			CCString 	 * speed = (CCString *)aGameObject->getValue(KStrSpeed);
			CCPoint 	direction = GameObject::dict2Point(directionDict);
			b2Vec2 linearVelocity;
			linearVelocity.x = direction.x * speed->floatValue();
			linearVelocity.y = direction.y * speed->floatValue();
			physicsBody->SetLinearVelocity(linearVelocity);
		}
		
		
		//检查到碰撞
		if (state & GameObject::K_STATE_COLLIDE) {
			GameModle * _modlesharedInstance = GameModle::sharedInstance();
			CCDictionary * propertyDict = (CCDictionary *)aGameObject->getValue(KStrProperty);
			CCString * atkStr = (CCString *)propertyDict->objectForKey(KStrATK);
			float atkValue = atkStr ? atkStr->floatValue():0;
			if (atkValue > 0) {
				CCSet * set = _modlesharedInstance->collideObjects(aGameObject);
				if (set) {
					CCSet * result_set = new CCSet;
					for (CCSetIterator iter = set->begin(); iter != set->end(); ++iter){
						GameObject *object = (GameObject *)(*iter);
						if (object != aGameObject->getOwner()
							&& object->isActive() 
							&& object->getTypeId() == KNPCTypeId 
							&& !(object->getState() & GameObject::K_STATE_FALL)) {
							
							int32_t checkValue = (int32_t)aGameObject;
							if (!object->checkExistInExtraMap(KStrATK_SOURCE,checkValue)) {
								object->addValueInExtraMap(KStrATK_SOURCE, checkValue);
								GameObject::reduceHP(object,atkValue,aGameObject);
								if (object->getHP() <= 0) {
									result_set->addObject(object);
								}
							}
						}
					}
					
					
					//激光将所有相交的对象引爆
					for (CCSetIterator iter = result_set->begin(); iter != result_set->end(); ++iter){
						GameObject * tmp=(GameObject *)(*iter);
						_modlesharedInstance->bubbleExplosion(tmp,0);//KBubbleToExplosionDelay
					}
					
					CC_SAFE_RELEASE_NULL(result_set);
				}
			}

			//完成本次碰撞的处理
			aGameObject->setState((aGameObject->getState() & ~GameObject::K_STATE_COLLIDE));
		}
	}
	return true;
}



bool BulletBezierAIScript::runScript(GameObject * aGameObject,CCDictionary * dic,float dt) {
	return true;
}

NS_KAI_END