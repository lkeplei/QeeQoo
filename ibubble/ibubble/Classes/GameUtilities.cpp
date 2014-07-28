//
//  GameUtilities.cpp
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GameUtilities.h"

#include "GameModle.h"

extern void GameUtilities_writeDateToFile(const char * aBuffer,const int aBufferLength,const char *pFileName);
extern const char * GameUtilities_getFilePathInDocumentDir(const char *pFileName);
extern bool GameUtilities_fileExistsAtPath(const char *pFileName);

extern void GameUtilities_saveLevelInfo(int levelId, kai::game::PlayerData plaerData);
extern int GameUtilities_getLevelInfo();
extern int GameUtilities_getRandId(int32_t levelId);
extern int GameUtilities_getRand(int32_t from, int32_t to);

void GameUtilities::writeDateToFile(const char * aBuffer,const int aBufferLength,const std::string & filepath){
	GameUtilities_writeDateToFile(aBuffer,aBufferLength,filepath.c_str());
}

std::string GameUtilities::getFilePathInDocumentDir(const std::string &filepath){
	std::string str(GameUtilities_getFilePathInDocumentDir(filepath.c_str()));
	return str.c_str();
}

bool GameUtilities::fileExistsAtPath(const std::string &filepath){
	return GameUtilities_fileExistsAtPath(filepath.c_str());
}

#pragma data
int GameUtilities::getRandLevel(){
    kai::game::GameModle* instance = kai::game::GameModle::sharedInstance();
	int level = instance->currentHardLevelId();
    
    GameUtilities::saveLevelId(level, kai::game::GameData::Instance().playerData);
    
    return GameUtilities_getRandId(level);
}

void GameUtilities::saveLevelId(int levelId, kai::game::PlayerData plaerData){
    GameUtilities_saveLevelInfo(levelId, plaerData);
}

int GameUtilities::getLevelId(){
    return GameUtilities_getLevelInfo();
}

int GameUtilities::getRand(uint32_t from, uint32_t to){
    return GameUtilities_getRand(from, to);
}