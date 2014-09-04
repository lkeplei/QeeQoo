//
//  GameData.h
//  MetalWar
//
//  Created by Ryan Yuan on 13-1-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef MetalWar_GameData_h
#define MetalWar_GameData_h
#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "SQLiteWrapper.h"
#include "GameUtilities.h"
NS_KAI_BEGIN

class PlayerAchievement {
public:
	int32_t _totalNpcCount;
	int32_t _killNpcCount;
    int32_t _records;
	float   _playingTime;
	float   _idleTime;
	float   _idleMaxTime;
	bool	_failed;
	int32_t _pass_count;
	int32_t _pass_star_count;
	float	_pass_time;
	int32_t _star_count;
	int32_t    _battleMode;
	int32_t	   _currentLevelId;
	int32_t	   _currentBigLevelId;
public:
	PlayerAchievement()
    :_totalNpcCount(0)
    ,_killNpcCount(0)
    ,_playingTime(0)
    ,_idleTime(0)
    ,_idleMaxTime(0)
    ,_failed(true)
    ,_pass_count(0)
    ,_pass_time(0)
    ,_star_count(0)
    ,_currentLevelId(0)
    ,_currentBigLevelId(0)
    ,_pass_star_count(0)
    ,_battleMode(0)
    ,_records(0)
    {

	}
    
	void initLevelId(const int32_t levelId){
		_currentBigLevelId = PlayerAchievement::bigLevelId(levelId);
		_currentLevelId = PlayerAchievement::levelId(levelId);
	}
	
	static uint32_t bigLevelId(const int32_t levelId){
		int32_t tmp = levelId % 900000;
		return  tmp / 100;
	}
	
	static uint32_t levelId(const int32_t levelId){
		int32_t tmp = levelId % 900000;
		return  tmp % 100;
	}
	
	static int32_t getLevelId(const PlayerAchievement & achievement) {
		return 900000 + 100 *achievement._currentBigLevelId + achievement._currentLevelId; 
	}
    
    void reset(){
        _totalNpcCount = 0;
		_killNpcCount = 0;
		_playingTime = 0;
		_idleTime = 0;
		_idleMaxTime = 0;
		_failed = true;
		_pass_count = 0;
		_pass_time = 0;
		_star_count = 0;
		_currentLevelId = 0;
		_currentBigLevelId = 0;
		_pass_star_count = 0;
        _battleMode = 0;
        _records = 0;
    }
};

class PlayerData {
public:
	int32_t _unlockedStoryLevel;
	int32_t _unlockedHardLevel;
	int32_t _hardLevelStarCount;
	int32_t _hardLevelKillCount;
    int32_t	_uid;
    
public:
	PlayerData(){
        _unlockedStoryLevel = 0;
        
        reset();
    }
    
    void reset(){
        _unlockedHardLevel = 0;
        _hardLevelStarCount = 0;
        _hardLevelKillCount = 0;
        _uid = -1;
    }
};

class GameData {
public:
    static GameData & Instance();
	static void releaseInstance();
private:
    GameData();
	~GameData();
	bool init();
	void initOpenDB();
	void initCreateTables();
protected:
	static GameData * _instance;
	SQLiteWrapper _sqlite;
public:
    PlayerData playerData;
public:
	//业务接口
	void resetData();
	void saveData(const PlayerAchievement & achievement);
	std::vector<PlayerAchievement> findData(int32_t levelid = -1,int32_t biglevelid = -1);
	std::pair<int, int> totalCount(int32_t levelid = -1,int32_t biglevelid = -1);
    std::pair<int, int> totalRecords(int32_t levelid = -1,int32_t biglevelid = -1);
	void unlockStory(const std::pair<int, int> & counts);
    void unlockAchievement();
private:

};
NS_KAI_END
#endif
