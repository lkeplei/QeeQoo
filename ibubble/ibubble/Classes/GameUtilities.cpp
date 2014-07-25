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

extern void GameUtilities_saveLevelId(int levelId);
extern int GameUtilities_getLevelId();
extern int GameUtilities_getRandId(int32_t levelId);

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
    
    GameUtilities::saveLevelId(level);
    
    return GameUtilities_getRandId(level);
}

void GameUtilities::saveLevelId(int levelId){
    GameUtilities_saveLevelId(levelId);
}

int GameUtilities::getLevelId(){
    return GameUtilities_getLevelId();
}