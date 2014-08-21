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
extern int GameUtilities_getRandId(int32_t levelId, bool add);
extern int GameUtilities_getRand(int32_t from, int32_t to);


extern void GameUtilities_saveRecord(int score);
extern void GameUtilities_savePass(int score);
extern void GameUtilities_saveStar(int score);
extern void GameUtilities_saveUnlockWithId(int achieveId);
extern void GameUtilities_saveGoonGame(bool goon);
extern int GameUtilities_getRecord();
extern int GameUtilities_getPass();
extern int GameUtilities_getStar();
extern bool GameUtilities_getUnlockWithId(int achieveId);
extern bool GameUtilities_getGoonGame();

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
int GameUtilities::getRandLevel(bool add){
    kai::game::GameModle* instance = kai::game::GameModle::sharedInstance();
	int level = instance->currentHardLevelId();
    
    GameUtilities::saveLevelId(level, kai::game::GameData::Instance().playerData);
    
    return GameUtilities_getRandId(level, add);
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

#pragma mark - record star pass
void GameUtilities::resetAchievement() {
    GameUtilities_saveRecord(0);
    GameUtilities_savePass(0);
    GameUtilities_saveStar(0);
}

void GameUtilities::saveRecord(int score) {
    GameUtilities_saveRecord(score);
}

void GameUtilities::savePass(int score) {
    GameUtilities_savePass(score);
}

void GameUtilities::saveStar(int score) {
    GameUtilities_saveStar(score);
}

void GameUtilities::saveUnlockWithId(int achieveId) {
    GameUtilities_saveUnlockWithId(achieveId);
}

int GameUtilities::getRecord() {
    return GameUtilities_getRecord();
}

int GameUtilities::getPass() {
    return GameUtilities_getPass();
}

int GameUtilities::getStar() {
    return GameUtilities_getStar();
}

bool GameUtilities::getUnlockWithId(int achieveId) {
    return GameUtilities_getUnlockWithId(achieveId);
}

void GameUtilities::saveGoonGame(bool goon) {
    GameUtilities_saveGoonGame(goon);
}

bool GameUtilities::getGoonGame() {
    return GameUtilities_getGoonGame();
}