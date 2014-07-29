//
//  GameObject.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameObject.h"
#include "GameModle.h"
#include "GameAIScript.h"
#include "GameCallback.h"
#include "GameConfig.h"
#include "GameAIScriptFactory.h"
#include "GameCallbackFactory.h"

NS_KAI_BEGIN
#pragma mark-
#pragma mark GameDisplayNode


GameDisplayNode::GameDisplayNode():CCNode()
,_animationManager(NULL)
{

}

GameDisplayNode::~GameDisplayNode(){
	CC_SAFE_RELEASE_NULL(_animationManager);
}

GameDisplayNode * GameDisplayNode::create(CCDictionary * dict){
	GameDisplayNode * node = new GameDisplayNode;
	if (node && node->initWithDictionary(dict)) {
		return node;
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

bool GameDisplayNode::initWithDictionary(CCDictionary * tmpDict){
	
	CCNode * _displayNode = NULL;
	if (tmpDict) {
		CCString * strValue = (CCString *)tmpDict->objectForKey(KStrType);
		uint32_t typeValue = strValue->uintValue();
		strValue = (CCString *)tmpDict->objectForKey(KStrFile);
		switch (typeValue) {
			case GameObject::K_SPRITE_FRAME:
			{
				_displayNode = CCSprite::createWithSpriteFrameName(strValue->getCString());
				this->addChild(_displayNode);
				_defaultFrame = strValue->getCString();
			}
				break;
			case GameObject::K_SPRITE_FILE:
			{
				_displayNode = CCSprite::create(strValue->getCString());
				this->addChild(_displayNode);
				_defaultFrame = strValue->getCString();
			}
				break;
			case GameObject::K_CCBI_FILE:
			{
				//动画加载
				CC_SAFE_RELEASE_NULL(_animationManager);
				string ccbifile = strValue->getCString();
				CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
				_displayNode = ccBReader->readNodeGraphFromFile(ccbifile.c_str(),this,&_animationManager);
				_animationManager->retain();
				_animationManager->setDelegate(this);
				this->addChild(_displayNode);
				CC_SAFE_RELEASE_NULL(ccBReader);
				
				CCString * default_frame = (CCString *)tmpDict->objectForKey(KStrDefaultFrame);
				if (default_frame) {
					_defaultFrame = default_frame->getCString();
				}
				
				CCString * default_anim=(CCString *)tmpDict->objectForKey(KStrDefaultAnim);
				if (default_anim) {
					_defaultAnimation = default_anim->getCString();
				}
			}
				break;
			default:
				break;
		}
	}
	return true;
}
	
void GameDisplayNode::runAnimation(const std::string & name,const float delay){
	if (_animationManager) {
		_animationManager->runAnimations(name.c_str(), delay);
	}
}
	
#pragma mark-
#pragma mark GameDisplayNode CCBSelectorResolver
SEL_MenuHandler GameDisplayNode::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_juqin",CCBAnimateNode::press_juqin);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler GameDisplayNode::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", CCBAnimateNode::press);
	return NULL;
}

#pragma mark-
#pragma mark GameDisplayNode CCBMemberVariableAssigner
bool GameDisplayNode::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "logo", CCSprite *, this->_logo);
	return false;
}

#pragma mark-
#pragma mark GameDisplayNode CCBNodeLoaderListener
void GameDisplayNode::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLog("GameObject node onNodeLoaded~") ;
}

#pragma mark-
#pragma mark GameDisplayNode CCBAnimationManagerDelegate
void GameDisplayNode::completedAnimationSequenceNamed(const char *name){
	
}

void GameDisplayNode::runningAnimationSequenceNamed(const char *name,const float time,const float dt){
	
}



#pragma mark-
#pragma mark GameObject

const int KGameObjectTagSprite=99999;

GameObject::GameObject():CCNode()
,_state(0)
,_classId(0)
,_simpleProperties(0)
,_properties(NULL)
,_displayNode(NULL)
,_physicsBody(NULL)
,_isFlipX(false)
,_isFlipY(false)
,_animationManager(NULL)
,_currentAnimInfo(NULL)
,_owner(NULL)
,_hp(0)
,_record(0)
{
	setIsActive(true);
	setIsTouchable(true);
}

GameObject::~GameObject(){
	_extraMap.clear();
	CC_SAFE_RELEASE_NULL(_animationManager);
	CC_SAFE_RELEASE_NULL(_actionDictionary);
	CC_SAFE_RELEASE_NULL(_aiScriptDictionary);
	CC_SAFE_RELEASE_NULL(_gameObjectComponent);
	CC_SAFE_RELEASE_NULL(_properties);
}

void GameObject::destoryPhysicsBody(){
	
	_fixtures.clear();
	
	if (_physicsBody) {
		b2World * _box2World=_physicsBody->GetWorld();
		if (_box2World) {
			_box2World->DestroyBody(_physicsBody);
		}
		_physicsBody=NULL;
	}
}

void GameObject::resetLinearVelocity(){
	if (_physicsBody) {
		b2Vec2 linearVelocity=b2Vec2(0.0f,0.0f);
		_physicsBody->SetLinearVelocity(linearVelocity);
	}
}

void GameObject::resetAngularVelocity(){
	if (_physicsBody) {
		float angularVelocity=0.0f;
		_physicsBody->SetAngularVelocity(angularVelocity);
	}
}

bool GameObject::init(){
	
	_properties = new CCDictionary();
	_gameObjectComponent = new CCDictionary();
	_aiScriptDictionary = new CCDictionary();
	_actionDictionary = new CCDictionary();
	
	return true;
}


bool GameObject::initWithDictionary(CCDictionary * dict){
	GameCallbackFactory *callbackFactory = GameCallbackFactory::sharedInstance();
	GameConfig * sharedGameConfigInstance = GameConfig::sharedInstance();
	GameModle  * sharedGameModleInstance = GameModle::sharedInstance();
	GameAIScriptFactory * sharedGameAIScriptFactoryInstance = GameAIScriptFactory::sharedInstance();
	GameObject::init();
	//解析字典，进行对象生成
	CCDictionary * tmpBox2dDict=NULL;
	CCDictElement * pDictElement=NULL;
	CCDICT_FOREACH(dict, pDictElement){
		const char * key = pDictElement->getStrKey();
		CCObject* keyValue=pDictElement->getObject();
		if (strcmp(key, KStrDisplay) == 0) {
			CCDictionary * tmpDict=dynamic_cast<CCDictionary *>(keyValue);
			if (tmpDict) {
				CCString * strValue=(CCString *)tmpDict->objectForKey(KStrType);
				uint32_t typeValue=strValue->uintValue();
				strValue=(CCString *)tmpDict->objectForKey(KStrFile);
				switch (typeValue) {
					case K_SPRITE_FRAME:
					    {
							_displayNode=CCSprite::createWithSpriteFrameName(strValue->getCString());
							this->addChild(_displayNode);
							this->setValue(KStrDefaultFrame, strValue);
						}
						break;
					case K_SPRITE_FILE:
						{
							_displayNode=CCSprite::create(strValue->getCString());
							this->addChild(_displayNode);
							this->setValue(KStrDefaultFrame, strValue);
						}
						break;
					case K_CCBI_FILE:
						{
							//动画加载
							CC_SAFE_RELEASE_NULL(_animationManager);
							string ccbifile = strValue->getCString();
							CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
							CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
							_displayNode = ccBReader->readNodeGraphFromFile(ccbifile.c_str(),this,&_animationManager);
							_animationManager->retain();
							_animationManager->setDelegate(this);
							this->addChild(_displayNode);
							CC_SAFE_RELEASE_NULL(ccBReader);
							
							CCString * default_frame = (CCString *)tmpDict->objectForKey(KStrDefaultFrame);
							if (default_frame) {
								this->setValue(KStrDefaultFrame, default_frame);
							}
							
							CCString * default_anim = (CCString *)tmpDict->objectForKey(KStrDefaultAnim);
							if (default_anim) {
								this->setValue(KStrDefaultAnim, default_anim);
							}
						}
						break;
					default:
						break;
				}
				
				if (_displayNode) {
					this->setContentSize(_displayNode->getContentSize());
				}

			}
		}
		else if (strcmp(key, KStrBox2d) == 0) {
			tmpBox2dDict=dynamic_cast<CCDictionary *>(keyValue);
			if (tmpBox2dDict) {
				this->setValue(key, keyValue);
			}
		}
		else if (strcmp(key, KStrPosition) == 0) {
			CCDictionary * tmpDict=dynamic_cast<CCDictionary *>(keyValue);
			this->setPosition(GameObject::dict2Point(tmpDict));
		}
		else if (strcmp(key, KStrAction) == 0) {
			CCDictionary * tmpDict=dynamic_cast<CCDictionary *>(keyValue);
			CCDictElement * pDictElement=NULL;
			CCDICT_FOREACH(tmpDict, pDictElement){
				const char * actionkey = pDictElement->getStrKey();
				CCDictionary * callbackDict = dynamic_cast<CCDictionary *>(pDictElement->getObject());
				if (callbackDict) {
					CCString * actionRealKey = (CCString *)callbackDict->objectForKey(KStrAction);
					GameCallback * callback=callbackFactory->getGameCallback(actionRealKey->getCString());
					this->addAction(actionkey,callback,callbackDict->objectForKey(KStrParam));
				}
			}
		}
		else if (strcmp(key, KStrAIScript) == 0) {
			CCDictionary * tmpDict=dynamic_cast<CCDictionary *>(keyValue);
			CCDictElement * pDictElement=NULL;
			CCDICT_FOREACH(tmpDict, pDictElement){
				const char * scriptkey = pDictElement->getStrKey();
				CCString * scriptRealKey=(CCString *)pDictElement->getObject();
				GameAIScript * script=sharedGameAIScriptFactoryInstance->getGameAIScript(scriptRealKey->getCString());
				this->addAIScript(scriptkey, script);
			}
		}
		else if (strcmp(key, KStrTouchable) == 0) {
			CCString * isTouchableStr = (CCString *)keyValue;
			this->setIsTouchable(isTouchableStr->boolValue());
		}
        else if (strcmp(key, KStrRecord) == 0) {
			CCString * val = (CCString *)keyValue;
			_record = val->intValue();
		}
		else {
			this->setValue(key, keyValue);
		}
	}
	
	//创建对应的物理属性
	if (tmpBox2dDict) {
		CCString * strValue = NULL;
		b2BodyDef bodyDef;
		strValue = (CCString *)tmpBox2dDict->objectForKey(KStrBodyType);
		if (strValue) {
			switch (strValue->intValue()) {
				case b2_kinematicBody:
					bodyDef.type = b2_kinematicBody;
					break;
				case b2_staticBody:
					bodyDef.type = b2_staticBody;
					break;
				case b2_dynamicBody:
				default:
					bodyDef.type = b2_dynamicBody;
					break;
			}
		}
		
		strValue = (CCString *)tmpBox2dDict->objectForKey(KStrGravityScale);
		bodyDef.gravityScale = strValue ? strValue->floatValue() : 1.0f;
		
		strValue = (CCString *)tmpBox2dDict->objectForKey(KStrIsBullet);
		bodyDef.bullet = strValue ? strValue->boolValue(): false;
		
		strValue = (CCString *)tmpBox2dDict->objectForKey(KStrFixedRotation);
		bodyDef.fixedRotation = strValue ? strValue->boolValue(): false;
		
		bodyDef.position.Set(this->getPosition().x/PTM_RATIO, this->getPosition().y/PTM_RATIO);
		b2Body *body = sharedGameModleInstance->box2dWorld()->CreateBody(&bodyDef);
		_physicsBody = body;
		_physicsBody->SetUserData(this);
		
		CCDictionary * box2dDict = (CCDictionary * )tmpBox2dDict->objectForKey(KStrFixtures);
		if(box2dDict){
			
			if(createFixtureToBody(KStrDefault,box2dDict)){
				_curFixture=KStrDefault;
			}
		}

	}
	
	//读取血值
	CCDictionary * property=(CCDictionary *)this->getValue(KStrProperty);
	if (property) {
		CCString * defStr=(CCString *)property->objectForKey(KStrDEF);
		if (defStr) {
			_hp = defStr->floatValue();
		}
	}

	
	return true;
}


b2Fixture * GameObject::createFixtureToBody(string fixtureName,CCDictionary * box2dDict)
{
	CCDictionary * tmpBox2dDict=box2dDict;
	if (!tmpBox2dDict) {
		tmpBox2dDict=(CCDictionary * )this->getValue(KStrBox2d);
		tmpBox2dDict=(CCDictionary *)tmpBox2dDict->objectForKey(KStrFixtures);
	}
	
	if (tmpBox2dDict && _physicsBody) {
		
		GameConfig * sharedGameConfigInstance=GameConfig::sharedInstance();
		GameModle  * sharedGameModleInstance=GameModle::sharedInstance();
		
		CCString * defaultStr=(CCString *)tmpBox2dDict->objectForKey(fixtureName);
		if (defaultStr) {
			
			CCDictionary * tmpDict=(CCDictionary *)sharedGameConfigInstance->getBox2dValue(defaultStr->getCString());
			CCString * strValue=(CCString *)tmpDict->objectForKey(KStrType);
			uint32_t typeValue=strValue->uintValue();
			
			CCSize size=this->getContentSize();
			float width = size.width / PTM_RATIO ;
			float height = size.height / PTM_RATIO;
			
			float centerX = 0.0f;
			float centerY = 0.0f;

			b2FixtureDef fixtureDef;
			b2CircleShape * circleShape=NULL;		
			
			switch (typeValue) {
				case K_BOX2D_SHAPE_RECT_SIZE:
				{
					CCString * str_w=(CCString *)tmpDict->objectForKey(KStrX);
					CCString * str_h=(CCString *)tmpDict->objectForKey(KStrY);
					width = str_w->floatValue() / PTM_RATIO;
					height = str_h->floatValue() / PTM_RATIO;
					
				}
					break;
				case K_BOX2D_SHAPE_RECT_SIZE_SCALE:
				{
					CCString * str_scale_w=(CCString *)tmpDict->objectForKey(KStrScaleX);
					CCString * str_scale_h=(CCString *)tmpDict->objectForKey(KStrScaleY);
					width = str_scale_w->floatValue() * width;
					height = str_scale_h->floatValue() * height;
				}
					break;
				case K_BOX2D_SHAPE_CIRCLE_SIZE:
				{
					circleShape=new b2CircleShape;
					CCString * str_r=(CCString *)tmpDict->objectForKey(KStrX);
					circleShape->m_radius = str_r->floatValue()/ PTM_RATIO;
					circleShape->m_p.Set(centerX, centerY);
					fixtureDef.shape = circleShape;
				}
					break;
				case K_BOX2D_SHAPE_CIRCLE_SIZE_SCALE:
				{
					circleShape=new b2CircleShape;
					CCString * str_scale_r=(CCString *)tmpDict->objectForKey(KStrScaleX);
					float half_width = size.width / 2;
					float half_height = size.height / 2;
					circleShape->m_radius = (sqrt(half_width * half_width + half_height * half_height)* str_scale_r->floatValue())/PTM_RATIO;
					circleShape->m_p.Set(centerX, centerY);
					fixtureDef.shape = circleShape;
					
				}
					break;
				default:
					break;
			}
			
			if (!circleShape) {
				b2PolygonShape dynamicBox;
				dynamicBox.SetAsBox(width,height);
				fixtureDef.shape = &dynamicBox;   
			}
			
			strValue=(CCString *)tmpDict->objectForKey(KStrDensity);
			fixtureDef.density = strValue?strValue->floatValue():1.0f;
			
			strValue=(CCString *)tmpDict->objectForKey(KStrFriction);
			fixtureDef.friction = strValue?strValue->floatValue():0.3f;
			
			strValue=(CCString *)tmpDict->objectForKey(KStrRestitution);
			fixtureDef.restitution = strValue?strValue->floatValue():1.0f;
			
			strValue=(CCString *)tmpDict->objectForKey(KStrIsSensor);
			fixtureDef.isSensor = strValue?strValue->boolValue():false;
			
			CCString * tmpBox2dFilter=(CCString *)tmpDict->objectForKey(KStrBox2dFilter);
			CCDictionary * tmpBox2dFilterDict=(CCDictionary *)sharedGameConfigInstance->getBox2dFilterValue(tmpBox2dFilter->getCString());
			if (tmpBox2dFilterDict) {
				fixtureDef.filter.groupIndex   = ((CCString *)tmpBox2dFilterDict->objectForKey(KStrGroupIndex))->intValue();
				fixtureDef.filter.categoryBits = ((CCString *)tmpBox2dFilterDict->objectForKey(KStrCategoryBits))->intValue();
				fixtureDef.filter.maskBits 	   = ((CCString *)tmpBox2dFilterDict->objectForKey(KStrMaskBits))->intValue();
			}

			b2Fixture *  fixture=_physicsBody->CreateFixture(&fixtureDef);
			_fixtures[fixtureName]=fixture;
			
			CC_SAFE_DELETE(circleShape);
			
			return fixture;
		}
	}
	
	return NULL;
}
void GameObject::destoryFixture(string fixtureName)
{
	std::map<std::string,b2Fixture *>::iterator it=_fixtures.find(fixtureName);
	if (it!=_fixtures.end()) {
		b2Fixture * tmp = (*it).second;
		if (_physicsBody) {
			_physicsBody->DestroyFixture(tmp);
		}
		_fixtures.erase(fixtureName);
	}
}

void GameObject::switchFixture(string fixtureName)
{
	if (_curFixture.compare(fixtureName) != 0) {
		destoryFixture(_curFixture);
		if(createFixtureToBody(fixtureName)){
			_curFixture=fixtureName;
		}
	}
}


GameObject * GameObject::create()
{
	GameObject * pRet = new GameObject();
	if (pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}


GameObject * GameObject::create(CCDictionary * dict)
{
	GameObject * pRet = new GameObject();
	if (pRet->initWithDictionary(dict))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void GameObject::runAnimation(const std::string & name,const float delay){
	
	if (_animationManager) {
		std::string animKey = name;
		if (animKey.length() == 0) {
			CCString * key = (CCString *) this->getValue(KStrDefaultAnim);
			if (key) {
				animKey = key->getCString();
			}else if(_animationManager->getSequences()->count() > 0){
				CCBSequence * seq = (CCBSequence *)_animationManager->getSequences()->objectAtIndex(0);
				animKey = seq->getName();
			}
		}
		
		if (animKey.length() != 0){
			CCDictionary * anim = (CCDictionary *)this->getValue(KStrAnimation);
			if (anim) {
				CCDictionary *animInfo = (CCDictionary *)anim->objectForKey(animKey);
				if (animInfo) {
					_currentAnimInfo = animInfo;
					CCString * str = (CCString *)animInfo->objectForKey(KStrAnimation);
					if (str) {
						//CCLOG("GameObject[%x]runAnimation[%s --> %s]",this,name.c_str(),str->getCString());
						_animationManager->runAnimations(str->getCString(),delay);
					}
				}
			}
			else {
				CCAssert(false, "KStrAnimation is not configed");
			}
		}
	}
}


void GameObject::stopAnimation(){
	if (_animationManager) {
		_currentAnimInfo=NULL;
		_animationManager->stopAnimation();
	}
}


void GameObject::update(const float dt){
	CCDictionary * dict=NULL;
	runAIScript(KStrMainScript,dict,dt);
}

CCObject * GameObject::getValue(const std::string & key){
	return _properties->objectForKey(key);
}

void GameObject::setValue(const std::string & key,CCObject * value){
	CCAssert(value != NULL, "GameObject::setValue-->value is invalid!!!");
	_properties->setObject(value, key);
}

void GameObject::removeValue(const std::string & key){
	_properties->removeObjectForKey(key);
}


CCObject* GameObject::gameObjectComponentForKey(const string& key){
	if(_gameObjectComponent){
		return _gameObjectComponent->objectForKey(key);
	}
	return NULL;
}

void GameObject::removeGameObjectComponent(const string & aActionkey){
	
	if(_gameObjectComponent){
		_gameObjectComponent->removeObjectForKey(aActionkey);
	}
}

void GameObject::setGameObjectComponent(CCObject* pObject, const string& key){
	if(_gameObjectComponent){
		_gameObjectComponent->setObject(pObject,key);
	}
}

void GameObject::addAIScript(const string & aActionkey, GameAIScript * object){
	if(_aiScriptDictionary){
		_aiScriptDictionary->setObject(object, aActionkey);
	}
}

void GameObject::removeAIScript(const string & aActionkey) {
	if(_aiScriptDictionary){
		_aiScriptDictionary->removeObjectForKey(aActionkey);
	}
}


void GameObject::addAction(const string & aActionkey,GameCallback * object,CCObject * param){
	if(_actionDictionary){
		
		CCDictionary * newDict = CCDictionary::create();
		newDict->setObject(object, KStrAction);
		if(param) 
			newDict->setObject(param, KStrParam);
		
		_actionDictionary->setObject(newDict, aActionkey);
	}
}

void GameObject::removeAction(const string & aActionkey){
	if(_actionDictionary){
		_actionDictionary->removeObjectForKey(aActionkey);
	}
}

bool GameObject::doCallback(const std::string & key,CCDictionary *  dict,const float dt,const bool dict_first){
	
	if(_actionDictionary){
		
		CCDictionary* newDict=(CCDictionary *)_actionDictionary->objectForKey(key);
		if (newDict) {
			
			GameCallback* action=(GameCallback *)newDict->objectForKey(KStrAction);
			
			CCObject * param=(CCObject *)newDict->objectForKey(KStrParam);
			CCDictionary * pdict=NULL;
			if (dict) {
				pdict = CCDictionary::create();
				//CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict) 2.0.X 没有实现所以不能使用
				CCDictElement * pDictElement=NULL;
				CCDICT_FOREACH(dict, pDictElement){
					pdict->setObject((CCObject *)pDictElement->getObject(), pDictElement->getStrKey());
				}
				
				CCObject * pdict_param=(CCObject *)pdict->objectForKey(KStrParam);
				if((param && !dict_first) || (!pdict_param && dict_first) ){
					pdict->setObject(param, KStrParam);
				}

			}
			else if(param){
				pdict = CCDictionary::create();
				pdict->setObject(param, KStrParam);
			}
			
			return action->doCallback(this,pdict,dt);
		}
	}
	return false;
}

bool GameObject::runAIScript(const string &aActionkey,CCDictionary * dic,const float dt){
	if (isActive()) { //对象激活状态继续运行AI
		
		if(_actionDictionary){
			GameAIScript* action=(GameAIScript *)_aiScriptDictionary->objectForKey(aActionkey);
			bool ret = false;
			if (action) {
				ret = action->runScript(this, dic,dt);
			}
			return ret;
		}
		
	}
	return false;
}

void GameObject::setPositionWithPhysics(const CCPoint& newPosition){
	CCNode::setPosition(newPosition);
    if (_physicsBody && _physicsBody->GetWorld()) {
		float angle=_physicsBody->GetAngle();
        _physicsBody->SetTransform(b2Vec2(newPosition.x / PTM_RATIO, newPosition.y / PTM_RATIO),angle);
    }
}

void GameObject::setRotationWithPhysics(float angle)
{
	CCNode::setRotation(-angle);
	if (_physicsBody && _physicsBody->GetWorld()) {
		b2Vec2 p = _physicsBody->GetPosition();
        _physicsBody->SetTransform(p,CC_DEGREES_TO_RADIANS(angle));
    }
}

bool GameObject::isActive() const {
	return (_simpleProperties  & 0x00000001) > 0;
}

void GameObject::setIsActive(const bool value){
	if (value) {
		_simpleProperties=(_simpleProperties & 0xfffffffe)|(0x00000001);
	}
	else {
		_simpleProperties=(_simpleProperties & 0xfffffffe);
	}
}


bool GameObject::isTouchable() const {
	return (_simpleProperties  & 0x00000010) > 0;
}

void GameObject::setIsTouchable(const bool value){
	if (value) {
		_simpleProperties=(_simpleProperties & 0xffffffef)|(0x00000010);
	}
	else {
		_simpleProperties=(_simpleProperties & 0xffffffef);
	}
}

void GameObject::setClassId(const uint32_t value){
	_classId=value;
}

uint32_t GameObject::getClassId()const{
	return _classId;
}

void GameObject::setState(const uint32_t value){
	_state=value;
}

uint32_t GameObject::getState()const{
	return _state;
}
void GameObject::setFlipX(bool flipX){
    if(_isFlipX != flipX) {
        setScaleX(getScaleX() * -1);
        _isFlipX = flipX;
    }
}

void GameObject::setFlipY(bool flipY){
    if(_isFlipY != flipY) {
        setScaleY(getScaleY() * -1);
        _isFlipY = flipY;
    }
}

bool GameObject::isFlipX() const{
	return _isFlipX;
}

bool GameObject::isFlipY() const{
	return _isFlipY;
}

float GameObject::getHP()const{
	return _hp;
}

void GameObject::setHP(const float value){
	_hp = value;
}

const int32_t GameObject::getRecord(){
    CCLOG("=================== recorde = %d");
	return _record;
}

void GameObject::reduceHP(GameObject * target,const float hpOffset,GameObject * source){
	if (target->getHP() <= hpOffset) {
		target->setHP(0);
	}
	else{
		target->setHP(target->getHP() - hpOffset);
	}
	
	CCLOG("GameObject::reduceHP: %x [%d]-> %x [%d]: atk = %f",source,source->getClassId(),target,target->getClassId(),hpOffset);
}



#pragma mark-
#pragma mark GameObject CCBSelectorResolver
SEL_MenuHandler GameObject::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_juqin",CCBAnimateNode::press_juqin);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler GameObject::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", CCBAnimateNode::press);
	return NULL;
}

#pragma mark-
#pragma mark GameObject CCBMemberVariableAssigner
bool GameObject::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	//CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "logo", CCSprite *, this->_logo);
	return false;
}

#pragma mark-
#pragma mark GameObject CCBNodeLoaderListener
void GameObject::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLog("GameObject node onNodeLoaded~") ;
}

#pragma mark-
#pragma mark GameObject CCBAnimationManagerDelegate
void GameObject::completedAnimationSequenceNamed(const char *name){

}

void GameObject::runningAnimationSequenceNamed(const char *name,const float time,const float dt){
	//CCLog("[0x%x]CCBAnimateNode completedAnimationSequenceNamed:%s  time=%.6f dt=%.6f",this,name,time,dt);
	if (_currentAnimInfo) {
		CCDictionary *  events=(CCDictionary *)_currentAnimInfo->objectForKey(KStrTimeEvent);
		if (events) {
			CCDictElement * pDictElement=NULL;
			CCDICT_FOREACH(events, pDictElement){
				string key = pDictElement->getStrKey();
				float time_key = atof(key.c_str());
				time_key=time - time_key;
				if (time_key >= 0 ) {
					if (time_key <= STD_DELTA) {
					    CCDictionary *  eventInfo=(CCDictionary *)pDictElement->getObject();
						CCArray * array=(CCArray * )eventInfo->objectForKey(KStrCallbacks);
						if (array && array->count() > 0 ) {
							CCObject * obj=NULL;
							CCARRAY_FOREACH(array, obj){
								doCallback(((CCString *)obj)->getCString(),eventInfo,dt);
							}
						}
					}
					break;
				}
			}
		}
	}
}

void GameObject::doCallFunc(CCObject * callFuncObject) {
	CCDictionary * dict = (CCDictionary *)callFuncObject;
	if (dict) {
		CCString   * action = (CCString *)dict->objectForKey(KStrAction);		
		this->doCallback(action->getCString(),dict,0.0f,true);
	}
}

CCAction * GameObject::createSequenceAction(GameObject * obj,CCArray * callbacks,CCDictionary * paramDict) {
	CCAction * sequence = NULL;
	if (callbacks) {
		CCArray  * arrayOfActions = CCArray::createWithCapacity(callbacks->count());
		CCObject * pElement = NULL;
	    CCARRAY_FOREACH(callbacks, pElement){
			CCDictionary * dict = (CCDictionary *)pElement;
			CCString * action = (CCString *)dict->objectForKey(KStrAction);
			string actionStr = action->getCString();
			if (!action ||actionStr.compare(KStrDelay) == 0) {
				CCString * delay = (CCString *)dict->objectForKey(KStrParam);
				if (delay) {
					arrayOfActions->addObject(CCDelayTime::create(delay->floatValue()));
				}
			}
			else {
				
				CCDictionary * callFuncObject = CCDictionary::create();
				callFuncObject->setObject(action, KStrAction);
				
				//传入一些配置参数
				CCObject * param = (CCObject *)dict->objectForKey(KStrParam);
				if (param) {
					callFuncObject->setObject(param, KStrParam);
				}
				
				//传入一些动态参数
				if (paramDict) {
					CCDictElement * pDictElement = NULL;
					CCDICT_FOREACH(paramDict, pDictElement){
						callFuncObject->setObject(pDictElement->getObject(), pDictElement->getStrKey());
					}
				}
				
				arrayOfActions->addObject(CCCallFuncO::create(obj,callfuncO_selector(GameObject::doCallFunc),callFuncObject));
			}
		}
		sequence = CCSequence::create(arrayOfActions);
	}
	return sequence;
}

bool GameObject::checkExistInExtraMap(const std::string & key,const int32_t value){

	std::map<std::string,std::list<int32_t> >::iterator find=_extraMap.find(key);
	if (find != _extraMap.end()) {
		std::list<int32_t> tmplist=(*find).second;
		for (std::list<int32_t>::iterator iter = tmplist.begin() ; iter != tmplist.end(); iter ++ ) {
			if (*iter == value) {
				return true;
			}	
		}
	}
	return false;
} 

void GameObject::addValueInExtraMap(const std::string & key,const int32_t value){
	
	std::map<std::string,std::list<int32_t> >::iterator find=_extraMap.find(key);
	if (find == _extraMap.end()) {
		std::list<int32_t> tmplist;
		tmplist.push_back(value);
		_extraMap[key] = tmplist;
	}
	else {
		((*find).second).push_back(value);
	}
}

// utils
#pragma mark-
#pragma mark GameObject utils
CCDictionary * GameObject::point2Dict(const CCPoint & p){
	CCDictionary * dict=CCDictionary::create();
	dict->setObject(CCString::createWithFormat("%f",p.x), KStrX);
	dict->setObject(CCString::createWithFormat("%f",p.y), KStrY);
	return dict;
}

CCPoint GameObject::dict2Point(CCDictionary * dict){
	CCString * str_x=(CCString *)dict->objectForKey(KStrX);
	CCString * str_y=(CCString *)dict->objectForKey(KStrY);
	return CCPoint(str_x->floatValue(), str_y->floatValue());
}

bool GameObject::isCircleContact(const CCPoint & p1,const float r1,const CCPoint & p2,const float r2){
	float x=abs(p1.x-p2.x);
	float y=abs(p1.y-p2.y);
	float dist=sqrt(x*x+y*y);
	if (dist <= (r1+r2)) {
		//CCLOG("dist=%f | p1=[%f,%f],r1=%f | p2=[%f,%f],r2=%f",dist,p1.x,p1.y,r1,p2.x,p2.y,r2);
		return true;
	}
	return false;

}

CCPoint GameObject::unitVector(const CCPoint & from,const CCPoint & to){
	
	float x=(to.x-from.x);
	float y=(to.y-from.y);
	float  modle=sqrt(x*x+y*y);
	
	return CCPoint(x/modle,y/modle);
}

float GameObject::angleFromVector(const CCPoint & vectorp){
	float radian=atan2(vectorp.y, vectorp.x);
	return radian * 180 / M_PI;
}

std::string GameObject::timeToString(const float seconds){
	std::string str="";
	uint32_t mm = (uint32_t)seconds / 60;
	uint32_t ss = (uint32_t)seconds % 60;
	uint32_t hh = (mm > 60 ? 0 : mm/60);
	mm = mm % 60;
	
	stringstream h_num;
	h_num << hh;
	if (hh < 10) {
		str.append("0");
	}
	str.append(h_num.str());
	str.append(":");
	
	
	stringstream m_num;
	m_num << mm;
	if (mm < 10) {
		str.append("0");
	}
	str.append(m_num.str());
	str.append(":");
	
	stringstream s_num;
	s_num << ss;
	if (ss < 10) {
		str.append("0");
	}
	str.append(s_num.str());
	
	return str;
}
NS_KAI_END
