//
//  GameData.cpp
//  MetalWar
//
//  Created by Ryan Yuan on 13-1-9.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "GameData.h"
#include "GameUtilities.h"
#include "cocos2d.h"
#include "GameConstant.h"
NS_KAI_BEGIN

GameData * GameData::_instance = NULL;

GameData & GameData::Instance(){

	if (!_instance) {
		_instance = new GameData();
		_instance->init();
	}
	return * _instance;
}


void  GameData::releaseInstance(){
	
	if (_instance) {
		delete _instance;
		_instance = NULL;
	}
}

GameData::GameData(){

}

GameData::~GameData(){

}

bool GameData::init(){
	initOpenDB();
	initCreateTables();
	return true;
}
void GameData::initOpenDB(){
	//初始化数据库
	std::string cpath = GameUtilities::getFilePathInDocumentDir("database.db");
	if (_sqlite.Open(cpath.c_str())) {
		std::cout<< "GameData::init " << cpath << " created or opened" << std::endl;
	}
	else {
		std::cout<< "GameData::init " << cpath << " couldn't open" << std::endl;
	}
	
}

void GameData::initCreateTables(){
	//初始化数据库表
	//1)创建玩家信息表
    std::string sql = "create table passrecord(levelid integer primary key,livecount integer default 0,killcount integer default 0, starcount integer default 0, records integer default 0)";
	if(_sqlite.DirectStatement(sql.c_str())){
		std::cout<<"passrecord table foo created"<<std::endl;
	}
	else{
		std::cout<<"passrecord couldn't be created"<<std::endl;
	}
    
    //2)创建玩家信息表
    sql = "create table playerData(uid integer primary key,";
    sql = sql + "_unlockedStoryLevel integer default 0,";
    sql = sql + "_unlockedHardLevel  integer default 0,";
    sql = sql + "_hardLevelStarCount integer default 0,";
    sql = sql + "_hardLevelKillCount integer default 0,";
    sql = sql + "_skill1Times integer default 0,";
    sql = sql + "_skill2Times integer default 0,";
    sql = sql + "_skill3Times integer default 0,";
    sql = sql + "_skill4Times integer default 0,";
    sql = sql + "_skill5Times integer default 0,";
    sql = sql + "_skill6Times integer default 0)";
    
	if(_sqlite.DirectStatement(sql.c_str())){
		std::cout<<"passrecord table foo created"<<std::endl;
        playerData._uid = 0;
	}
	else{
		std::cout<<"passrecord couldn't be created"<<std::endl;
	}
}


void GameData::resetData(){
	if(_sqlite.DirectStatement("delete from passrecord")){
		
		std::cout<<"drop table student"<<std::endl;
	}
	else {
		std::cout<<"drop table student failed"<<std::endl;
	}
}

void GameData::saveData(const PlayerAchievement & achievement){
	int32_t levelId = PlayerAchievement::getLevelId(achievement);
	SQLiteStatement* stmt = NULL;
	if (findData(levelId).size() > 0) {
		stmt=_sqlite.Statement("update passrecord set livecount=?,killcount=?,starcount=?,records=? where levelid = ?");
		if (stmt) {
			stmt->Bind(0,achievement._totalNpcCount);
			stmt->Bind(1,achievement._killNpcCount);
			stmt->Bind(2,achievement._star_count);
            stmt->Bind(3,achievement._records);
			stmt->Bind(4,levelId);
		}
	}
	else {
		stmt=_sqlite.Statement("insert into passrecord (levelid,livecount,killcount,starcount,records)values(?,?,?,?,?)");
		if (stmt) {
			stmt->Bind(0,levelId);
			stmt->Bind(1,achievement._totalNpcCount);
			stmt->Bind(2,achievement._killNpcCount);
            stmt->Bind(3,achievement._star_count);
			stmt->Bind(4,achievement._records);
		}
	}
	
	if (stmt) {
		if(stmt->Execute()){
			std::cout<<"statement executed"<<std::endl;
		}
		else{
			std::cout<<"error executing statement: "<<_sqlite.LastError()<<std::endl;
		}
		delete stmt;
		stmt = NULL;	
	}	
	
}

std::vector<PlayerAchievement> GameData::findData(int32_t levelid,int32_t biglevelid){
	std::vector<PlayerAchievement>  list;
	SQLiteStatement* stmt = NULL;
	std::stringstream selectsql;
	if (levelid < 0) {
		selectsql << "select levelid,livecount,killcount,starcount,records from passrecord order by levelid asc";
	}
	else {
		selectsql << "select levelid,livecount,killcount,starcount,records from passrecord where levelid =" << levelid << "";
	}
	
	stmt=_sqlite.Statement(selectsql.str());
	if (stmt) {
		PlayerAchievement achievement;
		while(stmt->NextRow()){
			int32_t id = stmt->ValueInt(0);
			achievement.initLevelId(id);
			if (biglevelid == -1 || (biglevelid >= 0 && biglevelid == achievement._currentBigLevelId)) {
				achievement._totalNpcCount = stmt->ValueInt(1);
				achievement._killNpcCount = stmt->ValueInt(2);
				achievement._star_count = stmt->ValueInt(3);
                achievement._records = stmt->ValueInt(4);
				list.push_back(achievement);	
			}
		}
	}
	return list;
}

std::pair<int, int>  GameData::totalCount(int32_t levelid ,int32_t biglevelid ){
	std::pair<int, int>  count(0,0);
	std::vector<PlayerAchievement> list = GameData::findData(levelid,biglevelid);
	for (std::vector<PlayerAchievement>::const_iterator iter = list.begin(); iter != list.end(); iter++) {
		count.first += (*iter)._star_count;
		count.second ++;
	}
	return count;
}

std::pair<int, int>  GameData::totalRecords(int32_t levelid ,int32_t biglevelid ){
	std::pair<int, int>  count(0,0);
	std::vector<PlayerAchievement> list = GameData::findData(levelid,biglevelid);
	for (std::vector<PlayerAchievement>::const_iterator iter = list.begin(); iter != list.end(); iter++) {
		count.first += (*iter)._records;
		count.second ++;
	}
	return count;
}

void saveUnlockId(int id) {
    if (id > 0) {
        GameUtilities::saveUnlockWithId(id);
    }
}

void GameData::unlockAchievement(bool openAchievement) {
    //	unlockid = 10002;       //总分  >= 30000
    //    unlockid = 10005;       //总分  >= 100000
    //    unlockid = 10006;       //开启挑战模式
    //    unlockid = 10007;       //过关10
    //    unlockid = 10008;       //星过关10
    
    if (openAchievement) {
        saveUnlockId(10006);
    }
    
    if (GameUtilities::getMaxRecord() >= 30000) {
        saveUnlockId(10002);
    }
    
    if (GameUtilities::getMaxRecord() >= 100000) {
        saveUnlockId(10005);
    }
    
    if (GameUtilities::getMaxPass()>= 10) {
        saveUnlockId(10007);
    }
    
    if (GameUtilities::getMaxStar() >= 10) {
        saveUnlockId(10008);
    }
}

void GameData::unlockStory(){
    std::pair<int, int> counts0 = GameData::Instance().totalCount(-1, 0);
    std::pair<int, int> counts1 = GameData::Instance().totalCount(-1, 1);
    std::pair<int, int> counts2 = GameData::Instance().totalCount(-1, 2);
    std::pair<int, int> counts(0, 0);
    counts.first = counts0.first + counts1.first + counts2.first;
    counts.second = counts0.second + counts1.second + counts2.second;
    
	int unlockid = 0;
	if (counts.first >= 15 ) { //star >= 15
		unlockid = 10001;
        saveUnlockId(unlockid);
	}
    
    if (counts.second >= 45 ) { //pass > 45
		unlockid = 10003;
        saveUnlockId(unlockid);
#ifdef KVersionFull
        unlockAchievement(true);
#endif
	}
    
    if (counts.first >= 45 ) { //pass > 45
		unlockid = 10004;
        saveUnlockId(unlockid);
	}
    
    if (counts.first >= 25 ) { //pass > 45
		unlockid = 10009;
        saveUnlockId(unlockid);
	}
    
    if (counts.first >= 35 ) { //pass > 45
		unlockid = 10010;
        saveUnlockId(unlockid);
	}
}
NS_KAI_END
