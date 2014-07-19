//
//  GameObject.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameObject_h
#define ibubble_GameObject_h

#include "Box2D.h"
#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
NS_KAI_BEGIN

#pragma mark-
#pragma mark GameDisplayNode

class GameDisplayNode:public cocos2d::CCNode
,public cocos2d::extension::CCBAnimationManagerDelegate
,public cocos2d::extension::CCBSelectorResolver
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	GameDisplayNode();
	virtual ~GameDisplayNode();
	static GameDisplayNode * create(CCDictionary * dict);
	const std::string & getDefaultFrame(){ return _defaultFrame; }
	const std::string & getDefaultAnimation(){return _defaultAnimation; }
	void  runAnimation(const std::string & name,const float delay = 0.0f);
protected:
	bool initWithDictionary(CCDictionary * dict);
public:
#pragma mark-
#pragma mark CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
#pragma mark-
#pragma mark CCBMemberVariableAssigner
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
#pragma mark-
#pragma mark CCBNodeLoaderListener
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
#pragma mark-
#pragma mark CCBAnimationManagerDelegate
    virtual void completedAnimationSequenceNamed(const char *name);
	virtual void runningAnimationSequenceNamed(const char *name,const float time,const float dt);
protected:
	cocos2d::extension::CCBAnimationManager *_animationManager;
	std::string _defaultFrame;
	std::string _defaultAnimation;
};


#pragma mark-
#pragma mark GameObject

/**游戏对象*/
class GameAIScript;
class GameCallback;
class GameObject:public cocos2d::CCNode
,public cocos2d::extension::CCBAnimationManagerDelegate
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener
{
public:
	enum EDisplayFileTypeId {
		K_SPRITE_FRAME    =0,
		K_SPRITE_FILE  	  =1,
		K_CCBI_FILE 	  =2,
        K_SWF_FILE                =3,
        K_CCBI_FILE_WITH_CODE 	  =4,
	};
	
	enum EBox2dTypeId {
		K_BOX2D_SHAPE_RECT_SIZE    	   =0,
		K_BOX2D_SHAPE_RECT_SIZE_SCALE  =1,
		K_BOX2D_SHAPE_CIRCLE_SIZE      =2,
		K_BOX2D_SHAPE_CIRCLE_SIZE_SCALE=3,
	};
	
	enum EState {
		K_STATE_FALL  	     =1<<0,
		K_STATE_MOVING     	 =1<<1,
		K_STATE_SKILL_OPENED =1<<2,
		K_STATE_COLLIDE		 =1<<3,
		K_STATE_DISABLE_EXPLOSION =1<<4,
	};
	
public:
	GameObject();
	virtual ~GameObject();
	static GameObject * create();
	static GameObject * create(CCDictionary * dict);
protected:
	bool init();
	bool initWithDictionary(CCDictionary * dict);
public:
	virtual void setPositionWithPhysics(const CCPoint& newPosition);
	virtual void setRotationWithPhysics(float angle);
	
	bool isActive() const;
	void setIsActive(const bool value);
	
	bool isTouchable() const;
	void setIsTouchable(const bool value);
	
	void setClassId(const uint32_t value);
	uint32_t getClassId()const;
	inline uint32_t getTypeId(){	return typeIdOfClassId(_classId);	}
	void setState(const uint32_t value);
	uint32_t getState()const;
	
	void setHP(const float value);
	float getHP()const;
	
	void setFlipX(bool flipX);
	void setFlipY(bool flipY);
    
    bool isFlipX() const;
    bool isFlipY() const;
	const int32_t getRecord();
    
	b2Body* getPhysicsBody() {return _physicsBody;}
	void 	destoryPhysicsBody();

	b2Fixture * createFixtureToBody(string fixtureName,CCDictionary * tmpBox2dDict=NULL);
	void destoryFixture(string fixtureName);
	void switchFixture(string fixtureName);
	
	void 	resetLinearVelocity();
	void 	resetAngularVelocity();
	
	CCObject* gameObjectComponentForKey(const string& key);
    void removeGameObjectComponent(const string & aActionkey);
	void setGameObjectComponent(CCObject* pObject, const string& key);
	
	void addAction(const string & aActionkey,GameCallback * object,CCObject * param = NULL);
	void removeAction(const string & aActionkey);
	
	void addAIScript(const string & aActionkey, GameAIScript * object);
    void removeAIScript(const string & aActionkey);
	
	
	CCObject * getValue(const std::string & key);
	void setValue(const std::string & key,CCObject * value);
	void removeValue(const std::string & key);
	
	virtual void update(const float dt);
	virtual bool doCallback(const std::string & key,CCDictionary *  dict = NULL,const float dt = 0.0f,const bool dict_first = false);
	virtual bool runAIScript(const string &aActionkey,CCDictionary * dic,const float dt);
	void 		 runAnimation(const std::string & name = "",const float delay=0.0f);
	void 		 stopAnimation();
public:
	virtual void doCallFunc(CCObject * callFuncObject);
	
	static  CCAction * createSequenceAction(GameObject * obj,
											CCArray * callbacks,
											CCDictionary * paramDict = NULL);
	
	bool checkExistInExtraMap(const std::string & key,const int32_t value);
	void addValueInExtraMap(const std::string & key,const int32_t value);
	
	static void reduceHP(GameObject * target,const float hpOffset,GameObject * source = NULL);
public:
	// utils
	static CCDictionary * point2Dict(const CCPoint & p);
	static CCPoint dict2Point(CCDictionary * dict);
	static uint32_t typeIdOfClassId(uint32_t classId){	return classId/100000;	}
	static bool isCircleContact(const CCPoint & p1,const float r1,const CCPoint & p2,const float r2);
	static CCPoint unitVector(const CCPoint & from,const CCPoint & to);
	static float angleFromVector(const CCPoint & vectorp);
	static std::string timeToString(const float seconds);
public:
	CC_SYNTHESIZE(GameObject * ,_owner,Owner);
private:	
	CCNode * 		_displayNode;
	b2Body *   		_physicsBody;
	CCDictionary *  _properties;
	CCDictionary *  _gameObjectComponent;
	CCDictionary *  _aiScriptDictionary;
	CCDictionary *  _actionDictionary;
	CCDictionary *  _currentAnimInfo;
	uint32_t		_simpleProperties;
	uint32_t		_classId;
	uint32_t		_state;
    int32_t         _record;
	std::map<std::string,b2Fixture *> _fixtures;
	std::map<std::string,std::list<int32_t> > _extraMap; //用来保存一些特殊列表标记数据
	std::string     _curFixture;
	bool _isFlipX;
    bool _isFlipY;
	float		_hp;
public:
#pragma mark-
#pragma mark CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
#pragma mark-
#pragma mark CCBMemberVariableAssigner
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
#pragma mark-
#pragma mark CCBNodeLoaderListener
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
#pragma mark-
#pragma mark CCBAnimationManagerDelegate
    virtual void completedAnimationSequenceNamed(const char *name);
	virtual void runningAnimationSequenceNamed(const char *name,const float time,const float dt);
protected:
	cocos2d::extension::CCBAnimationManager *_animationManager;
};
NS_KAI_END
#endif
