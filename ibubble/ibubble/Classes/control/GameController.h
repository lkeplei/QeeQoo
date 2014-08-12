//
//  GameController.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameController_h
#define ibubble_GameController_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"

USING_NS_CC;

NS_KAI_BEGIN


class GameModle;


class  GameController : public CCObject
{
public:
	enum ESceneId {
		K_SCENE_HOME    =0,
		K_SCENE_BATTLE  =1,
		K_SCENE_SETTING =2,
		K_SCENE_HELP    =3,
		K_SCENE_BATTLE_CHALLENGE  =4,
		K_SCENE_ACHIEVEMENT  =5,
		K_SCENE_LEVELS_SELECTION  =6,
		K_SCENE_ABOUT  =7,
		K_SCENE_SUCCESS  =8,
		K_SCENE_FAILED  =9,
		K_SCENE_SUB_LEVELS_SELECTION  =10,
		K_SCENE_VIEW_ACHIEVEMENT,
		K_SCENE_BATTLE_PRE,
		K_SCENE_SCORES,
		K_SCENE_HELP_IN_LEVEL,
		K_SCENE_LEVEL_MV,
		K_SCENE_SETTING_IN_LEVEL,
        K_SCENE_ChallengeStart,
        K_SCENE_ChallengeGameStart,
        K_SCENE_ChallengeHelpInLevel,
	};
protected:
	GameController();
	virtual ~GameController();
public:
    /** returns a shared instance of GameController */
    static GameController *sharedInstance(void);
    /** purge the shared instance of GameController */
    static void purgeInstance(void);
	/** init the new created instance of GameController*/
	bool init(void);
	void update(float dt);
public:
	void pushSence(const ESceneId sceneId,CCObject * param,CCObject * param2);
	bool popSence();
    
    void controllerPushSence(CCScene *pScene, bool pushSence=false);
	
	void switchSence(const ESceneId sceneId,CCObject * param = NULL,
					 CCObject * param2 = NULL,
					 bool pushSence = false);
	void startBattle();
    void pauseBattle(void);
    void resumeBattle(void);
	void onEnterScene(const ESceneId sceneId,CCLayer * layer);
	void onExitScene(const ESceneId sceneId,CCLayer* layer );
	void onTouchesEnded(CCSet* touches, CCEvent* event);
	bool isPushSenceFromBattle(){return _pushSenceFromBattle;}
	bool isPushSence(){return _pushSence;}
	void setIsPushSenceFromBattle(bool value ){ _pushSenceFromBattle = value;}
private:
	CCDirector *	_director;
	GameModle * 	_sharedGameModleInstance;
	bool _updateStoped;
	bool _pushSenceFromBattle;
	bool _pushSence;
};
NS_KAI_END
#endif
