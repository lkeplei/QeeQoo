//
//  GameUtilities.h
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_h
#define ibubble_h
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

class GameUtilities {
public:
	static void writeDateToFile(const char * aBuffer,const int aBufferLength,const std::string &filepath);
	static std::string getFilePathInDocumentDir(const std::string &filepath);
	static bool fileExistsAtPath(const std::string &filepath);
    
    static int getRandLevel(bool add=true);
    static void saveLevelId(int levelId);
    static int getLevelId();
    static int getRand(uint32_t from, uint32_t to);
    
    static void resetAchievement();
    static void saveRecord(int score);
    static void savePass(int score);
    static void saveStar(int score);
    static void saveUnlockWithId(int achieveId);
    static int getRecord();
    static int getPass();
    static int getStar();
    static int getMaxRecord();
    static int getMaxPass();
    static int getMaxStar();
    static bool getUnlockWithId(int achieveId);
    static void saveGoonGame(bool goon=true);
    static bool getGoonGame();
    static void saveAchieveLevelId(int level);
    static int getAchieveLevelId();
    static void saveAchieveHardLevelId(int level);
    static int getAchieveHardLevelId();

    static void saveSkillLife(int value);
    static void saveSkillMultiTouch(int value);
    static void saveSkillSkip(int value);
    static void saveSkillWeak(int value);
    static void saveSkillSTouch(int value);
    static void saveSkillLargeTouch(int value);
    static int getSkillLife();
    static int getSkillMultiTouch();
    static int getSkillSkip();
    static int getSkillWeak();
    static int getSkillSTouch();
    static int getSkillLargeTouch();
    
    static void removeSkillInfo();
    
};

#endif
