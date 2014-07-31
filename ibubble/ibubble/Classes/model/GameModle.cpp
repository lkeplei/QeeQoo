//
//  GameModle.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "GameModle.h"
#include "GameConfig.h"
#include "GameAIScript.h"
#include "GameObject.h"
#include "GameAIScriptFactory.h"
#include "GameEffectManager.h"
#include "GameCallbackFactory.h"
#include "GameConstant.h"
#include "BattleScene.h"
#include "CCShake.h"
#include "GameData.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
static GameModle * _sharedInstance=NULL;

GameModle::GameModle():CCObject()
,_properties(NULL)
,_sharedGameConfigInstance(NULL)
,_gameObjectsDict(NULL)
,_box2dWorld(NULL)
,_debugDraw(NULL)
,_battleSenceScript(NULL)
,_battleGameObjectlayer(NULL)
,_battleRootlayer(NULL)
,_sharedEffectManagerInstance(NULL)
,_lastTargetPointIndex(0)
,_game_state(KGameStateNone)
,_collideObjects(NULL)
,_battleTouchTimes(0)
,_battleTouchMaxTimes(1)
{
	_playerAchievement._currentLevelId = 0;
	_playerAchievement._currentBigLevelId = 0;
    
    levelList.push_back(-1);
    levelList.push_back(-1);
    levelList.push_back(-1);
    levelList.push_back(-1);
    levelList.push_back(-1);
}

GameModle::~GameModle(){
	_activefallList.clear();
	_tmpbox2dFixtures.clear();
	CC_SAFE_RELEASE_NULL(_collideObjects);
	
	//释放所有对象
	if (_gameObjectsDict) {
		
		CCDictElement * pDictElement=NULL;
		CCDICT_FOREACH(_gameObjectsDict, pDictElement){
			CCSet * set=(CCSet*)pDictElement->getObject();
			CCSetIterator iter;
			for (iter = set->begin(); iter != set->end(); ++iter){
				GameObject * tmp=(GameObject *)(*iter);
				tmp->destoryPhysicsBody();
			}
		}
		
		_gameObjectsDict->removeAllObjects();
	}
	
	CC_SAFE_RELEASE_NULL(_gameObjectsDict);
	CC_SAFE_RELEASE_NULL(_properties);
	releasePhysics();
	
}

GameModle * GameModle::sharedInstance(void){
	if (! _sharedInstance)
    {
        _sharedInstance = new GameModle();
        _sharedInstance->init();
    }
	
	return _sharedInstance;
}

void GameModle::purgeInstance(void){
	CC_SAFE_RELEASE_NULL(_sharedInstance);
}

bool GameModle::init(void){

	_director = CCDirector::sharedDirector();
	_sharedGameConfigInstance = GameConfig::sharedInstance();
	_sharedEffectManagerInstance = GameEffectManager::sharedInstance();
	_sharedCallbackFactory = GameCallbackFactory::sharedInstance();
	
	_properties = new CCDictionary();
	_gameObjectsDict = new CCDictionary();	
	_collideObjects =  new CCDictionary();	
	
	//加载默认的音频文件
	CCDictionary * audios=(CCDictionary *)_sharedGameConfigInstance->getValue(KStrAudios);
	if (audios) {
		_sharedEffectManagerInstance->loadAudioEffect(audios);
	}
	
	_box2dWorldRect = CCRect(0,0,_director->getWinSize().width,_director->getWinSize().height);
	
    GameUtilities::getLevelId();
    
	return true;
}


CCObject * GameModle::getValue(const std::string & key){
	return _properties->objectForKey(key);
}

void GameModle::setValue(const std::string & key,CCObject * value){
	CCAssert(value != NULL, "GameModle::setValue-->value is invalid!!!");
	_properties->setObject(value, key);
}

CCObject * GameModle::valueFromCurrentLevel(const string & key)
{
	if (_currentLevelDict) {
		return _currentLevelDict->objectForKey(key);
	}
	return NULL;
}

#pragma mark-
#pragma mark GameWorld create,init,setting,etc.
void GameModle::resetCache(CCLayer * battleGameObjectlayer)
{
	clearCache();
	_game_state = KGameStateInit;
	_battleRootlayer = battleGameObjectlayer;
	_battleGameObjectlayer = (CCLayer *)_battleRootlayer->getChildByTag(KBattleNodeTagGameObjectLayer);
	
	stringstream currentBigStr;
	currentBigStr << _playerAchievement._currentBigLevelId;
	CCDictionary * dict = (CCDictionary *)_sharedGameConfigInstance->getLevelsValue(currentBigStr.str());
	CCArray * sublevels = (CCArray *)dict->objectForKey(KStrSubLevels);
	_currentLevelDict = (CCDictionary *)sublevels->objectAtIndex(_playerAchievement._currentLevelId);
	
	//TODO:未来需要读取配置文件决定加载哪个AI
	_battleSenceScript = GameAIScriptFactory::sharedInstance()->getGameAIScript(KStrBattleAIScriptDefault);
    
	CCString * tmpStr = NULL;
	tmpStr = (CCString * )_currentLevelDict->objectForKey(KStrPassCount);
	_playerAchievement._pass_count=tmpStr->intValue();
	tmpStr = (CCString * )_currentLevelDict->objectForKey(KStrPassTime);
	_playerAchievement._pass_time=tmpStr->floatValue();
	tmpStr = (CCString * )_currentLevelDict->objectForKey(KStrStarCount);
	_playerAchievement._pass_star_count=tmpStr->intValue();
	tmpStr = (CCString * )_currentLevelDict->objectForKey(KStrIdleTime);
	_playerAchievement._idleMaxTime=tmpStr->floatValue();
}

void GameModle::clearCache()
{
	if (_collideObjects) {
		_collideObjects->removeAllObjects();
	}
	
	if (_gameObjectsDict) {

		CCDictElement * pDictElement=NULL;
		CCDICT_FOREACH(_gameObjectsDict, pDictElement){
			CCSet * set=(CCSet*)pDictElement->getObject();
			CCSetIterator iter;
			for (iter = set->begin(); iter != set->end(); ++iter)
			{
				GameObject * tmp=(GameObject *)(*iter);
				tmp->destoryPhysicsBody();
			}
		}
		_gameObjectsDict->removeAllObjects();
		
	}
	_battleRootlayer = NULL;
	_battleGameObjectlayer = NULL;
	_game_state = KGameStateInit;
	_activefallList.clear();
	_battleTouchTimes = 0;
	_battleTouchMaxTimes = 1;
}
void GameModle::gameStart(){
	_game_state = KGameStateNone;
}

void GameModle::generateGameObjects()
{
	_playerAchievement._killNpcCount = 0;
	_playerAchievement._totalNpcCount = 0;
	
	CCSize winSize=_director->getWinSize();
	CCDictionary *npcTemplateDict=(CCDictionary *)_sharedGameConfigInstance->getValue(KStrNpc);
	CCArray *levelNpcDict=(CCArray *)_currentLevelDict->objectForKey(KStrNPCs);
	CCObject * pElement = NULL;
	CCARRAY_FOREACH(levelNpcDict, pElement){
		CCDictionary *npcConfig=(CCDictionary *)pElement;
		CCString  * npcCount=(CCString *)npcConfig->objectForKey(KStrCount);
		CCArray *  npcSelection=(CCArray *)npcConfig->objectForKey(KStrNPCs);
		int   npcCountVar = npcCount->uintValue();
		int   npcSelectionCount = npcSelection->count();
		CCPoint point;
		if (npcSelectionCount  == 1) {
			CCDictionary * npc=((CCDictionary  *)npcSelection->objectAtIndex(0));
			CCString * npcIdStr = (CCString *)npc->objectForKey(KStrId);
			for (int i=0; i < npcCountVar; i++) {
				point = ccp(random()%((int)winSize.width),random()%((int)winSize.height));
				GameObject * obj = this->generateGameObjectByClassId(npcIdStr->uintValue(),npcTemplateDict);
				obj->setPositionWithPhysics(point);
				float delay = 0.1f + 0.1f*(arc4random()%5);
				obj->runAnimation(KStrInBubble,delay);
				obj->setState(GameObject::K_STATE_MOVING);
				_playerAchievement._totalNpcCount++;
			}
			
		}
		else if (npcSelectionCount  > 1) {
			
			for (int i=0; i < npcCountVar; i++) {
				
				int index = (random() % npcSelectionCount);
				
				CCDictionary * npc=((CCDictionary  *)npcSelection->objectAtIndex(index));
				CCString * npcIdStr = (CCString *)npc->objectForKey(KStrId);
				
				point = ccp(random()%((int)winSize.width),random()%((int)winSize.height));
				GameObject * obj = this->generateGameObjectByClassId(npcIdStr->uintValue(),npcTemplateDict);
				obj->setPositionWithPhysics(point);
				float delay = 0.1f + 0.1f*(arc4random()%5);
				obj->runAnimation(KStrInBubble,delay);
				obj->setState(GameObject::K_STATE_MOVING);
				_playerAchievement._totalNpcCount++;
			}
		}
	}
	

	notifyDataChanged();
}

GameObject * GameModle::generateGameObjectByClassId(uint32_t classId,CCDictionary *pTemplateDict){
	GameObject * obj=NULL;
	CCDictionary *templateDict=pTemplateDict;
	if (!templateDict) {
		const uint32_t typeId=GameObject::typeIdOfClassId(classId);
		switch (typeId) {
			case KNPCTypeId:
				templateDict=(CCDictionary *)_sharedGameConfigInstance->getValue(KStrNpc);
				break;
			case KBulletTypeId:
				templateDict=(CCDictionary *)_sharedGameConfigInstance->getValue(KStrBullet);
				break;
			default:
				break;
		}
	}

	if (templateDict) {
		stringstream key;
		key << classId;
		CCDictionary * gdict=(CCDictionary *)templateDict->objectForKey(key.str());
		if (gdict) {
			obj=GameObject::create(gdict);
			obj->setClassId(classId);
			addGameObjectToBattle(obj);
		}
	}
	return obj;
}

void GameModle::addGameObjectToBattle(GameObject * obj)
{
	CCAssert(_battleGameObjectlayer!=NULL, "addGameObjectToBattle _battleGameObjectlayer == NUL invalid");
	CCSet * set=(CCSet*)_gameObjectsDict->objectForKey(obj->getClassId());
	if (set == NULL) {
		set=new CCSet;
		_gameObjectsDict->setObject(set, obj->getClassId());
		CC_SAFE_RELEASE(set);
	}
	set->addObject(obj);
	_battleGameObjectlayer->addChild(obj);
	obj->runAnimation();
	obj->stopAnimation();
}

void GameModle::addNodeToBattle(CCNode * node)
{
	CCAssert(_battleGameObjectlayer!=NULL, "addNodeToBattle _battleGameObjectlayer == NUL invalid");
	_battleGameObjectlayer->addChild(node);
}

void GameModle::removeGameObjectFromBattle(GameObject * obj)
{
	CCAssert(obj!=NULL, "removeGameObjectFromBattle obj == NUL invalid");
	CCSet * set=(CCSet*)_gameObjectsDict->objectForKey(obj->getClassId());
	if (set) {
		set->removeObject(obj);
	}
	obj->removeAllChildrenWithCleanup(true);
}

void GameModle::removeNodeFromBattle(CCNode * node)
{
	CCAssert(_battleGameObjectlayer!=NULL, "removeNodeFromBattle _battleGameObjectlayer == NUL invalid");
	node->removeAllChildrenWithCleanup(true);
}

void GameModle::initAudioEffect()
{
	CCDictionary * audios=(CCDictionary *)_currentLevelDict->objectForKey(KStrAudios);
	if (audios) {
		_sharedEffectManagerInstance->loadAudioEffect(audios);
	}
}

void GameModle::releaseAudioEffect()
{
	CCDictionary * audios=(CCDictionary *)_currentLevelDict->objectForKey(KStrAudios);
	if (audios) {
		_sharedEffectManagerInstance->releaseAudioEffect(audios);
	}
}

void GameModle::releasePhysics()
{
	CC_SAFE_DELETE(_debugDraw);
	CC_SAFE_DELETE(_box2dWorld);
}

void GameModle::initPhysics()
{
	
    CCSize s = _box2dWorldRect.size;
    
	b2Vec2 gravity;
    //gravity.Set(0.0f, -10.0f);
	gravity.Set(0.0f, 0.0f);
    _box2dWorld = new b2World(gravity);
	_box2dWorld->SetContactListener(this);
    _box2dWorld->SetAllowSleeping(true);
    _box2dWorld->SetContinuousPhysics(true);
	_debugDraw = new GLESDebugDraw( PTM_RATIO );
	_box2dWorld->SetDebugDraw(_debugDraw);
	
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
    _debugDraw->SetFlags(flags);
	
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    b2Body* groundBody = _box2dWorld->CreateBody(&groundBodyDef);
    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}

#pragma mark-
#pragma mark b2ContactListener
void GameModle::BeginContact(b2Contact* contact){
	GameObject *objectA = dynamic_cast<GameObject*>((CCObject *)contact->GetFixtureA()->GetBody()->GetUserData());
	GameObject *objectB = dynamic_cast<GameObject*>((CCObject *)contact->GetFixtureB()->GetBody()->GetUserData());
	if(objectA && objectB){
		doCollide(objectA,objectB);
	}
}

void GameModle::EndContact(b2Contact* contact){
	b2ContactListener::EndContact(contact);
}

#pragma mark-
#pragma mark GameModle::update
void GameModle::update(float dt){
	
	//运行时间累加
	_playerAchievement._playingTime += dt;
	_playerAchievement._idleTime += dt;
	
	//clear collideObjects
	_collideObjects->removeAllObjects();
	
	CCSet * tmpSet=new CCSet;
	//物理世界的计算
    const int velocityIterations = _sharedGameConfigInstance->box2dVelocityIterations();
    const int positionIterations = _sharedGameConfigInstance->box2dPositionIterations();
    _box2dWorld->Step(dt, velocityIterations, positionIterations);
    for (b2Body* b = _box2dWorld->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
			CCNode * node=(CCNode *)b->GetUserData();
			if (node) {
				node->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
				node->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
				
				//假如是游戏对象再次进行其他处理
				GameObject* myActor = dynamic_cast<GameObject*>(node);
				if (myActor) {
					if (!_box2dWorldRect.containsPoint(myActor->getPosition())) {
						myActor->setIsActive(false);
					}
				}
			}
        }    
    }
	
	//运行所有游戏对象的AI
	CCDictElement * pDictElement=NULL;
	CCDICT_FOREACH(_gameObjectsDict, pDictElement){
		CCSet * set=(CCSet*)pDictElement->getObject();
		CCSetIterator iter;
		for (iter = set->begin(); iter != set->end(); ++iter)
		{
			GameObject * tmp=(GameObject *)(*iter);
			if (tmp->isActive()) {
				tmp->update(dt);
			}
			else {
				tmpSet->addObject(tmp);
			}
		}
	}
	
	//清除掉无效的对象
	CCSetIterator iter;
	for (iter = tmpSet->begin(); iter != tmpSet->end(); ++iter)
	{
		GameObject * tmp=(GameObject *)(*iter);
		
		for (std::list<GameObject *>::iterator iter=_activefallList.begin(); iter!=_activefallList.end(); iter++) {
			if ((*iter) == tmp) {
				_activefallList.erase(iter);
				break;
			}
		}
		
		tmp->destoryPhysicsBody();
		
		this->removeGameObjectFromBattle(tmp);
	}
	
	CC_SAFE_RELEASE_NULL(tmpSet);
	
	//运行场景的AI
	if (_battleSenceScript) {
		_battleSenceScript->runScript(NULL, NULL,dt);
	}
	
}

#pragma mark-
#pragma mark GameModle::pointPressed
void GameModle::pointPressed(const CCPoint & point){
	
	//筛选一个技能没有被使用的，并且使用技能
	for (std::list<GameObject *>::iterator iter = _activefallList.begin(); iter != _activefallList.end(); iter++) {
		GameObject * _currentBubble = (*iter);
		if (_currentBubble && !(_currentBubble->getState() & GameObject::K_STATE_SKILL_OPENED)) {
			CCDictionary * param = CCDictionary::create();
			param->setObject(GameObject::point2Dict(point), KStrPosition);
			bubbleOpenSkill(_currentBubble,param);
			break;
		}
	}

	QueryAABB(point);
	GameObject* handleActor=NULL;
	const bool touchable = !(getBattleTouchTimes() > 0 && getBattleTouchTimes() >= getBattleTouchMaxTimes());
	if (touchable) {
		for (vector<b2Fixture *>::iterator it=_tmpbox2dFixtures.begin();
			 it!=_tmpbox2dFixtures.end(); it++) {
			b2Fixture * fixture = *it;
			CCNode * node=(CCNode *)fixture->GetBody()->GetUserData();
			if (node) {
				GameObject* myActor = dynamic_cast<GameObject*>(node);
				if (myActor && myActor->isTouchable() && myActor->getTypeId() == KNPCTypeId) {
					if (onGameObjectPressed(myActor)) {
						setBattleTouchTimes(getBattleTouchTimes() + 1);
						handleActor = myActor;
						break;
					}
				}
			}
		}
	}
	_tmpbox2dFixtures.clear();
}

void GameModle::QueryAABB(const CCPoint & point,const float size_pixel){
	//搜出这个点上的刚体
	b2Vec2 p(point.x/PTM_RATIO,point.y/PTM_RATIO);
	b2Vec2 d;
	float r = size_pixel/PTM_RATIO;
	d.Set(r,r);
	b2AABB aabb;
	aabb.lowerBound=p-d;
	aabb.upperBound=p+d;
	_tmpbox2dFixtures.clear();
	if(_box2dWorld) 
		_box2dWorld->QueryAABB(this, aabb);
}

bool GameModle::ReportFixture(b2Fixture * fixture){
	if (fixture) {
		_tmpbox2dFixtures.push_back(fixture);
	}
	return true;
}

bool GameModle::onGameObjectPressed(GameObject * obj){
	
	//泡泡不是下落过程中
	uint32 state = obj->getState();
	if (!(state & GameObject::K_STATE_FALL) && !(state & GameObject::K_STATE_SKILL_OPENED)) {
		bubbleExplosion(obj);
		return true;
	}
	
	return false;
}

void GameModle::playEffect(const char * effectName,CCNode * effectParent,GameObject * obj,const CCPoint & pos,CCDictionary *pDict,CCAction * action){
	
	if (effectName) {
		
		//优先播放指定游戏对象的效果
		if (obj) {
			CCDictionary *effectConfig=(CCDictionary *)obj->getValue(KStrEffect);
			if (effectConfig) {
				CCDictionary *effectInfo=(CCDictionary *)effectConfig->objectForKey(effectName);
				if (effectInfo) {
					
					//优先使用外部指定的回调
					CCAction * sequenceAction = action;
					if (!sequenceAction) {
						CCArray * callbacks = (CCArray *)effectInfo->objectForKey(KStrCallbacks);
						if (callbacks) {
							sequenceAction = GameObject::createSequenceAction(obj,callbacks,pDict);
						}
					}
					
					
					CCString * effectId=(CCString *)effectInfo->objectForKey(KStrEffect);
					uint32_t id = effectId->uintValue();
					stringstream idstr;
					idstr << id;
					//播放一个动画
					_sharedEffectManagerInstance->playEffect(idstr.str(),effectParent,pos,sequenceAction);
				}
			}
		}
		else {
			//TODO:播放关卡设计的特效
		}
	}
}

void GameModle::playEffectById(const char * effectId,CCNode * effectParent,const CCPoint & pos,CCAction * action){
	if (effectId) {
		_sharedEffectManagerInstance->playEffect(effectId,effectParent,pos,action);
	}
}

void GameModle::playBackground(const std::string & filename){
	_sharedEffectManagerInstance->playBackground(filename);
}

GameEffectManager * GameModle::effectManager(){
	return _sharedEffectManagerInstance;
}

void GameModle::playBackground(const int Id){

	switch (Id) {
		case K_BG_MUSIC_BATTLE:
			{
				CCDictionary *audio=(CCDictionary *)_currentLevelDict->objectForKey(KStrAudios);
				if (audio) {
					CCString *music=(CCString *)audio->objectForKey(KStrBackgound);
					playBackground(music->getCString());
				}
			}
			break;
		case K_BG_MUSIC_OUTSIDE_OF_BATTLE:
		default:
			{
				CCDictionary *audio=(CCDictionary *)_sharedGameConfigInstance->getValue(KStrAudios);
				if (audio) {
					CCString *music=(CCString *)audio->objectForKey(KStrBackgound);
					playBackground(music->getCString());
				}
			}
			break;
	}
}

void GameModle::doCollide(GameObject * objA,GameObject * objB){
	
	CCLog("GameModle::doCollide(%d,%d) 0x%x  0x%x",
		  objA->getClassId(),
		  objB->getClassId(),
		  objA,
		  objB);


	objA->setState((objA->getState() | GameObject::K_STATE_COLLIDE));
	std::stringstream objectAkey;
	objectAkey <<  (int32_t)objA;
	CCSet * objAset = (CCSet *)_collideObjects->objectForKey(objectAkey.str());
	if (!objAset) {
		objAset = new CCSet;
		_collideObjects->setObject(objAset, objectAkey.str());
		objAset->release();
	}
	objAset->addObject(objB);
	
	objB->setState((objB->getState() | GameObject::K_STATE_COLLIDE));
	std::stringstream objectBkey;
	objectBkey <<  (int32_t)objB;
	CCSet * objBset = (CCSet *)_collideObjects->objectForKey(objectBkey.str());
	if (!objBset) {
		objBset = new CCSet;
		_collideObjects->setObject(objBset, objectBkey.str());
		objBset->release();
	}
	objBset->addObject(objA);

}

CCSet * GameModle::collideObjects(GameObject * obj){
	std::stringstream key;
	key <<  (int32_t)obj;
	return (CCSet *)_collideObjects->objectForKey(key.str());
}

void GameModle::bubbleExplosion(GameObject * obj,float delay){
	CCLOG("bubbleExplosion [0x%x] hp:%d",obj,obj->getHP());
	if (!(obj->getState() & GameObject::K_STATE_DISABLE_EXPLOSION)) {
		
		obj->setState((obj->getState() | GameObject::K_STATE_DISABLE_EXPLOSION));
		
		if (delay > 0) {
			const int KBubbleToExplosion = 19999;
			if (!obj->getActionByTag(KBubbleToExplosion)) {
				CCAction  * sequence=CCSequence::createWithTwoActions(CCDelayTime::create(delay), CCCallFuncO::create(this,callfuncO_selector(GameModle::bubbleToExplosion),obj));
				sequence->setTag(KBubbleToExplosion);
				obj->runAction(sequence);
				CCLOG("bubbleExplosion[%d]=0x%x delay=%f",obj->getClassId(),obj,delay);
			}
		}
		else {
			bubbleExplosion(obj);
		}
	}
}
void GameModle::bubbleToExplosion(CCObject * obj){
	CCLOG("2222bubbleExplosion=0x%x",obj);
	bubbleExplosion((GameObject*)obj);
}


void GameModle::bubbleExplosion(GameObject * obj){
	CCLOG("bubbleExplosion=0x%x",obj);

	obj->setState((obj->getState() | GameObject::K_STATE_DISABLE_EXPLOSION));
	
	_activefallList.push_back(obj);
	
	if((!(obj->getState() & GameObject::K_STATE_FALL)) && obj->getTypeId() == KNPCTypeId) {
        updateRecords(obj);
	}
	
	//执行爆炸操作
	obj->doCallback(KStrExplosion);
	
	notifyDataChanged();
	

	
	//只要有爆炸就重置
	_playerAchievement._idleTime = 0;
	
#if 0
	//震动屏幕
	const int shakeActionTag=900000;
	const float duration =1.0f;
	CCAction * shakeAction=_battleRootlayer->getActionByTag(shakeActionTag);
	if (shakeAction) {
		_battleRootlayer->stopAction(shakeAction);
	}
	shakeAction=CCShake::actionWithDuration(duration,ccp(5 + CCRANDOM_0_1() * 5, 5 + CCRANDOM_0_1() * 5), true);
	shakeAction->setTag(shakeActionTag);
	_battleRootlayer->runAction(shakeAction);
#endif
	
}

void GameModle::updateRecords(GameObject * obj){
    //增加解救的数量
    //高击破率奖励得分合计。超过50%击破率，每个泡泡增加20分；超过70%击破率，每个泡泡增加50分；
    //超过90%击破率，每个泡泡增加100分。当泡泡全击破时，本关总得分加倍。
    _playerAchievement._killNpcCount++;
    _playerAchievement._records += obj->getRecord();
}

bool GameModle::bubbleOpenSkill(GameObject * obj,CCDictionary * param){
	return obj->doCallback(KStrOpenSkill,param);
}

void GameModle::notifyDataChanged(){
	BattleScene * battle=dynamic_cast<BattleScene * >(_battleRootlayer);
	if (battle) {
		battle->modleUpdated();
	}
}

CCPoint GameModle::queryTargetPoint(const CCPoint & origPos){

	int xParts = 5;
    int yParts = 4;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int xDelta = winSize.width / xParts;
    int yDelta = winSize.height / yParts;
    
    int orig_vecpos = (int)(origPos.x / xDelta) + (int)(origPos.y / yDelta) * xParts;
    int orig_X = orig_vecpos % xParts;
    int orig_Y = orig_vecpos / xParts;
	
    std::vector<int32_t> counter(xParts * yParts,0);
	
	CCDictElement * pDictElement=NULL;
	CCDICT_FOREACH(_gameObjectsDict, pDictElement){
		CCSet * set=(CCSet*)pDictElement->getObject();
		CCSetIterator iter;
		for (iter = set->begin(); iter != set->end(); ++iter)
		{
			GameObject * tmp=(GameObject *)(*iter);
			if (tmp->isActive()) {
				CCPoint position=tmp->getPosition();
				int vecpos = (int)(position.x / xDelta) + (int)(position.y / yDelta) * xParts;
				counter[vecpos]++;
			}
		}
	}
    
    int maxIndex = 0;
    int maxValue = 0;
    
    //不能相邻或在同一个区域
    for (int i=0;i<counter.size();i++) {
        if (counter[i] >= maxValue 
            && orig_vecpos != i
            && orig_vecpos != (i-1)
            && orig_vecpos != (i+1)
            && orig_vecpos != (i-xParts)
            && orig_vecpos != (i-xParts - 1)
            && orig_vecpos != (i-xParts + 1)
            && orig_vecpos != (i+xParts)
            && orig_vecpos != (i+xParts - 1)
            && orig_vecpos != (i+xParts + 1)
            && _lastTargetPointIndex != i) {
            maxValue = counter[i];
            maxIndex = i;
        }
    }
	
	_lastTargetPointIndex = maxIndex;
    
	int maxX = maxIndex % xParts;
    int maxY = maxIndex / xParts;
    CCLog("orig(%d) (%d, %d), max(%d)  (%d, %d)", orig_vecpos, orig_X, orig_Y, maxIndex, maxX, maxY);
    return ccp((0.5f+maxX) * xDelta , (0.5f + maxY) * yDelta);

}
void GameModle::weakNPCs(){
	
	if (_collideObjects) {
		_collideObjects->removeAllObjects();
	}
	_activefallList.clear();
	
	if (_gameObjectsDict) {
		CCDictElement * pDictElement=NULL;
		CCDICT_FOREACH(_gameObjectsDict, pDictElement){
			if (GameObject::typeIdOfClassId(pDictElement->getIntKey()) == KNPCTypeId) {
				const int classType = (pDictElement->getIntKey() / 1000);
				if (classType != KNPCTypeNormalId) {
					const int  classId = KNPCTypeNormalId * 1000 + (pDictElement->getIntKey() % (classType * 1000)) ;
					CCSet * set = (CCSet*)pDictElement->getObject();
					for (CCSetIterator iter = set->begin(); iter != set->end(); ++iter)
					{
						GameObject * tmp = (GameObject *)(*iter);
						tmp->setIsActive(false);

						const CCPoint & point = tmp->getPosition();
						GameObject * obj = this->generateGameObjectByClassId(classId);
						obj->setPositionWithPhysics(point);
						float delay = 0.1f + 0.1f*(arc4random()%5);
						obj->runAnimation(KStrInBubble,delay);
						obj->setState(GameObject::K_STATE_MOVING);
					}
				}
			}
		}
	}
	

}

void GameModle::setTouchEnableTypes(int type){
	if (_gameObjectsDict) {
		CCDictElement * pDictElement=NULL;
		CCDICT_FOREACH(_gameObjectsDict, pDictElement){
			const int classType = (pDictElement->getIntKey() / 1000);
			if (classType == type) {
				CCSet * set = (CCSet*)pDictElement->getObject();
				for (CCSetIterator iter = set->begin(); iter != set->end(); ++iter)
				{
					GameObject * tmp = (GameObject *)(*iter);
					tmp->setIsTouchable(true);
				}
			}
		}
	}
}
b2World * GameModle::box2dWorld(){
    return _box2dWorld;
}

void  GameModle::setCurrentLevelId(const uint32_t value){
    _playerAchievement._currentLevelId = value;
}

void  GameModle::setCurrentBigLevelId(const uint32_t value){
    _playerAchievement._currentBigLevelId = value;
}

void GameModle::setCurrentHardLevelId(const uint32_t value){
    _playerAchievement._currentHardLevelId = value;
}

int32_t GameModle::currentLevelId()const{
    return _playerAchievement._currentLevelId;
}

int32_t GameModle::currentHardLevelId()const{
    return _playerAchievement._currentHardLevelId;
}

int32_t GameModle::currentBigLevelId()const{
    return _playerAchievement._currentBigLevelId;
}

int32_t GameModle::currendRecord()const{
    return _playerAchievement._totalRecords;
}

PlayerAchievement & GameModle::playerAchievement(){
    return _playerAchievement;
}

CCDictionary * GameModle::gameObjectsDict(){
    return _gameObjectsDict;
}

CCLayer * GameModle::battleGameObjectlayer(){
    return _battleGameObjectlayer;
}

SkillInfo & GameModle::getSkillInfo(){
    return GameData::Instance().playerData.skillInfo;
}
void GameModle::saveSkillInfo(){
    GameData::Instance().savePlayerData();
    GameUtilities::saveLevelId(_playerAchievement._currentHardLevelId, GameData::Instance().playerData);
}

const int  GameModle::getBattleMode(){
    return _playerAchievement._battleMode;
}

void  GameModle::setBattleMode(const int mode){
    _playerAchievement._battleMode = mode;
}

const int  GameModle::getBattleTouchTimes(){
    return _battleTouchTimes;
}

void  GameModle::setBattleTouchTimes(const int times){
    _battleTouchTimes = times;
}

const int  GameModle::getBattleTouchMaxTimes(){
    return _battleTouchMaxTimes;
}

void  GameModle::setBattleTouchMaxTimes(const int times){
    _battleTouchMaxTimes = times;
}

vector<int> GameModle::getLevelList(){
    return levelList;
}

void GameModle::pushLevelId(int levelId){
    levelList[_playerAchievement._currentHardLevelId % 5] = levelId;
}

void  GameModle::resetBattleInfo(){
    _playerAchievement.reset();
}

void GameModle::resetSkillInfo(){
    GameData::Instance().playerData.reset();
}

NS_KAI_END
