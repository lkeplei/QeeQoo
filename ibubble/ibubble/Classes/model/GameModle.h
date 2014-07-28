//
//  GameModle.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameModle_h
#define ibubble_GameModle_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "GameEffect.h"
#include "GameData.h"
USING_NS_CC;
NS_KAI_BEGIN
class GameConfig;
class GameAIScript;
class GameObject;
class GameEffectManager; 
class GameCallbackFactory;

class  GameModle 
: public CCObject
,b2ContactListener
,b2QueryCallback
{
protected:
	GameModle();
	virtual ~GameModle();
	/** init the new created instance of GameModle*/
	bool init(void);
	
public:
    /** returns a shared instance of GameModle */
    static GameModle *sharedInstance(void);
    /** purge the shared instance of GameModle */
    static void purgeInstance(void);
	
	CCObject * getValue(const std::string & key);
	void setValue(const std::string & key,CCObject * value);
	CCObject * valueFromCurrentLevel(const string & key);
	void update(float dt);
	void resetCache(CCLayer * battleGameObjectlayer);
	void clearCache();
	void initPhysics();
	void releasePhysics();
	
	void initAudioEffect();
	void releaseAudioEffect();
	
	
	void generateGameObjects();
	void gameStart();
	
	
public: /**战斗场景内进行效果播放等*/
	void playEffect(const char * effectId,CCNode * effectParent,GameObject * obj,const CCPoint & pos,CCDictionary *pDict = NULL,CCAction * action = NULL);
	void playEffectById(const char * effectId,CCNode * effectParent,const CCPoint & pos,CCAction * action = NULL);
	void playBackground(const std::string & filename);
	void playBackground(const int Id);
	
	void pointPressed(const CCPoint & point);
	
	
	
public: /**战斗场景内进行角色操作等*/
	GameObject * generateGameObjectByClassId(uint32_t classId,CCDictionary *pTemplateDict = NULL);
	void doCollide(GameObject * objA,GameObject * objB);
	CCSet * collideObjects(GameObject * obj);
	
	void bubbleExplosion(GameObject * obj,float delay);
    void updateRecords(GameObject * obj);
	void bubbleExplosion(GameObject * obj);
	void bubbleToExplosion(CCObject * obj); // 外部不要调用
	bool bubbleOpenSkill(GameObject * obj,CCDictionary * param = NULL);	
	void addGameObjectToBattle(GameObject * obj);
	void addNodeToBattle(CCNode * node);
	void removeGameObjectFromBattle(GameObject * node);
	void removeNodeFromBattle(CCNode * node);
	void notifyDataChanged();
	
public:
	/**挑战模式相关*/
	void weakNPCs();
	void setTouchEnableTypes(int type);
public:
	b2World * box2dWorld();
	void  setCurrentLevelId(const uint32_t value);
	void  setCurrentBigLevelId(const uint32_t value);
    void  setCurrentHardLevelId(const uint32_t value);

	int32_t currentLevelId()const;
	int32_t currentBigLevelId()const;
    int32_t currentHardLevelId()const;
	PlayerAchievement & playerAchievement();
	CCDictionary * gameObjectsDict();
	CCLayer * battleGameObjectlayer();
	CCPoint queryTargetPoint(const CCPoint & origPos);
	GameEffectManager * effectManager();
protected:
	bool onGameObjectPressed(GameObject * obj);
	void QueryAABB(const CCPoint & point,const float size_pixel=0.032);
public:
#pragma mark-
#pragma mark b2ContactListener
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
#pragma mark-
#pragma mark b2QueryCallback
	virtual bool ReportFixture(b2Fixture* fixture);
private:
	CCDictionary * _currentLevelDict;
	CCDictionary * _gameObjectsDict;
	CCDictionary * _properties;
	CCDirector   * _director;
	GameConfig   * _sharedGameConfigInstance;
	GameEffectManager   * _sharedEffectManagerInstance;
	GameCallbackFactory * _sharedCallbackFactory;
	b2World 	 * _box2dWorld;
	CCRect 		   _box2dWorldRect;
	GLESDebugDraw * _debugDraw;
	GameAIScript * _battleSenceScript;
	CCLayer * 	   _battleGameObjectlayer;
	CCLayer * 	   _battleRootlayer;	
	vector<b2Fixture *> _tmpbox2dFixtures;
	PlayerAchievement _playerAchievement;
	uint32_t _lastTargetPointIndex;
	uint32_t  _game_state;
	std::list<GameObject *>  _activefallList;
	CCDictionary * _collideObjects;
    vector<int> levelList;
private:
	SkillInfo _skillInfo;
	int		  _battleTouchTimes;
	int		  _battleTouchMaxTimes;
public:
	SkillInfo & getSkillInfo();
    void saveSkillInfo();
	const int  getBattleMode();
	void  setBattleMode(const int mode);
	
	const int  getBattleTouchTimes();
	void  setBattleTouchTimes(const int times);
	
	const int  getBattleTouchMaxTimes();
	void  setBattleTouchMaxTimes(const int times);
    
    vector<int> getLevelList();
    void pushLevelId(int levelId);
public:
    void resetBattleInfo();
    void resetSkillInfo();
};

NS_KAI_END

#endif
