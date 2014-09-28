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

extern void GameUtilities_saveLevelInfo(int levelId);
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
extern void GameUtilities_saveAchieveLevelId(int level);
extern int GameUtilities_getAchieveLevelId();
extern void GameUtilities_saveAchieveHardLevelId(int level);
extern int GameUtilities_getAchieveHardLevelId();

extern void GameUtilities_saveSkillLife(int value);
extern void GameUtilities_saveSkillMultiTouch(int value);
extern void GameUtilities_saveSkillSkip(int value);
extern void GameUtilities_saveSkillWeak(int value);
extern void GameUtilities_saveSkillSTouch(int value);
extern void GameUtilities_saveSkillLargeTouch(int value);
extern int GameUtilities_getSkillLife();
extern int GameUtilities_getSkillMultiTouch();
extern int GameUtilities_getSkillSkip();
extern int GameUtilities_getSkillWeak();
extern int GameUtilities_getSkillSTouch();
extern int GameUtilities_getSkillLargeTouch();

extern int GameUtilities_getMaxRecord();
extern void GameUtilities_saveMaxRecord(int value);
extern int GameUtilities_getMaxPass();
extern void GameUtilities_saveMaxPass(int value);
extern int GameUtilities_getMaxStar();
extern void GameUtilities_saveMaxStar(int value);

extern void GameUtilities_removeSkillInfo();

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
    
    GameUtilities::saveLevelId(level);
    
    return GameUtilities_getRandId(level, add);
}

void GameUtilities::saveLevelId(int levelId){
    GameUtilities_saveLevelInfo(levelId);
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
    int higher = GameUtilities_getMaxRecord();
    if (score > higher) {
        GameUtilities_saveMaxRecord(score);
    }
}

void GameUtilities::savePass(int score) {
    GameUtilities_savePass(score);
    int higher = GameUtilities_getMaxPass();
    if (score > higher) {
        GameUtilities_saveMaxPass(score);
    }
}

void GameUtilities::saveStar(int score) {
    GameUtilities_saveStar(score);
    int higher = GameUtilities_getMaxStar();
    if (score > higher) {
        GameUtilities_saveMaxStar(score);
    }
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

int GameUtilities::getMaxRecord() {
    return GameUtilities_getMaxRecord();
}

int GameUtilities::getMaxPass() {
    return GameUtilities_getMaxPass();
}

int GameUtilities::getMaxStar() {
    return GameUtilities_getMaxStar();
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

void GameUtilities::saveAchieveLevelId(int level) {
    GameUtilities_saveAchieveLevelId(level);
}

int GameUtilities::getAchieveLevelId() {
    return GameUtilities_getAchieveLevelId();
}

void GameUtilities::saveAchieveHardLevelId(int level) {
    GameUtilities_saveAchieveHardLevelId(level);
}

int GameUtilities::getAchieveHardLevelId() {
    return GameUtilities_getAchieveHardLevelId();
}

void GameUtilities::saveSkillLife(int value) {
    GameUtilities_saveSkillLife(value);
}
void GameUtilities::saveSkillMultiTouch(int value) {
    GameUtilities_saveSkillMultiTouch(value);
}
void GameUtilities::saveSkillSkip(int value) {
    GameUtilities_saveSkillSkip(value);
}
void GameUtilities::saveSkillWeak(int value) {
    GameUtilities_saveSkillWeak(value);
}
void GameUtilities::saveSkillSTouch(int value) {
    GameUtilities_saveSkillSTouch(value);
}
void GameUtilities::saveSkillLargeTouch(int value) {
    GameUtilities_saveSkillLargeTouch(value);
}
int GameUtilities::getSkillLife() {
    return GameUtilities_getSkillLife();
}
int GameUtilities::getSkillMultiTouch() {
    return GameUtilities_getSkillMultiTouch();
}
int GameUtilities::getSkillSkip() {
    return GameUtilities_getSkillSkip();
}
int GameUtilities::getSkillWeak() {
    return GameUtilities_getSkillWeak();
}
int GameUtilities::getSkillSTouch() {
    return GameUtilities_getSkillSTouch();
}
int GameUtilities::getSkillLargeTouch() {
    return GameUtilities_getSkillLargeTouch();
}

void GameUtilities::removeSkillInfo() {
    GameUtilities_removeSkillInfo();
}