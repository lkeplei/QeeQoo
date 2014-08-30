//
//  GameUtilities.m
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>
#include "cocos2d.h"

#include "GameModle.h"
#include "GameData.h"

#define kUnlockidHeadString     @"unlockid_"
#define kAchievementPass        @"achievement_pass"
#define kAchievementStar        @"achievement_star"
#define kAchievementRecord      @"achievement_record"
#define kAchievementLevel       @"achievement_level"
#define kAchievementGononGame   @"achievement_goon"

bool GameUtilities_writeDateToFile(const char * aBuffer,const int aBufferLength,const char *pFileName){
	
	NSArray *paths=NSSearchPathForDirectoriesInDomains(NSDocumentDirectory
													   , NSUserDomainMask
													   , YES);
	
	NSString *file = [NSString stringWithCString:pFileName];
	NSString *fileName = [[paths objectAtIndex:0] stringByAppendingPathComponent:file];
	const char* pszFullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath([fileName UTF8String]);
	NSString* pPath = [NSString stringWithUTF8String:pszFullPath];
	
	NSData * contentData = [NSData dataWithBytes:aBuffer length:aBufferLength];
	if ([contentData writeToFile:pPath atomically:YES]) {
		return true;
	}
	return false;
}

const char* GameUtilities_getFilePathInDocumentDir(const char *pFileName){
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory
													   , NSUserDomainMask
													   , YES);
	NSString *file = [NSString stringWithCString:pFileName];
	NSString *fileName = [[paths objectAtIndex:0] stringByAppendingPathComponent:file];
	return [fileName UTF8String];
}


bool GameUtilities_fileExistsAtPath(const char *pFileName){
	NSString *filepath = [NSString stringWithCString:pFileName];
	NSFileManager *fileManager = [NSFileManager defaultManager];
	return ([fileManager fileExistsAtPath:filepath] == YES);
}

#pragma mark - data
void GameUtilities_saveRecord(int score) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:score] forKey:kAchievementRecord];
    [defaults synchronize];
}

void GameUtilities_saveUnlockWithId(int achieveId) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithBool:YES] forKey:[kUnlockidHeadString stringByAppendingFormat:@"%d", achieveId]];
    [defaults synchronize];
}

void GameUtilities_savePass(int score) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:score] forKey:kAchievementPass];
    [defaults synchronize];
}

void GameUtilities_saveStar(int score) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:score] forKey:kAchievementStar];
    [defaults synchronize];
}

void GameUtilities_saveGoonGame(bool goon) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithBool:goon] forKey:kAchievementGononGame];
    [defaults synchronize];
}

int GameUtilities_getRecord() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [[defaults objectForKey:kAchievementRecord] intValue];
}

int GameUtilities_getPass() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [[defaults objectForKey:kAchievementPass] intValue];
}

int GameUtilities_getStar() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [[defaults objectForKey:kAchievementStar] intValue];
}

BOOL GameUtilities_getUnlockWithId(int achieveId) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [defaults boolForKey:[kUnlockidHeadString stringByAppendingFormat:@"%d", achieveId]];
}

BOOL GameUtilities_getGoonGame() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [defaults boolForKey:kAchievementGononGame];
}

void GameUtilities_saveAchieveLevelId(int level) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:level] forKey:kAchievementLevel];
    [defaults synchronize];
}

int GameUtilities_getAchieveLevelId() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [defaults integerForKey:kAchievementLevel];
}

void GameUtilities_saveLevelInfo(int levelId){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    
    NSMutableDictionary* infoDic = [[NSMutableDictionary alloc] init];
    [infoDic setObject:[NSNumber numberWithInt:levelId] forKey:@"info_levelId"];
    [defaults setObject:infoDic forKey:@"default_levelInfo"];
    [defaults synchronize];
}

int GameUtilities_getLevelInfo(){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary* infoDic = [defaults objectForKey:@"default_levelInfo"];
    return [[infoDic valueForKey:@"info_levelId"] intValue];
}

#pragma mark - skill
void GameUtilities_saveSkillLife(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_life"];
    [defaults synchronize];
}
void GameUtilities_saveSkillMultiTouch(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_multi_touch"];
    [defaults synchronize];
}
void GameUtilities_saveSkillSkip(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_skip"];
    [defaults synchronize];
}
void GameUtilities_saveSkillWeak(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_weak"];
    [defaults synchronize];
}
void GameUtilities_saveSkillSTouch(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_s_touch"];
    [defaults synchronize];
}
void GameUtilities_saveSkillLargeTouch(int value) {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:value] forKey:@"info_skill_large_touch"];
    [defaults synchronize];
}

int GameUtilities_getSkillLife() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_life"]) {
        return [defaults integerForKey:@"info_skill_life"];
    } else {
        return 3;
    }
}
int GameUtilities_getSkillMultiTouch() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_multi_touch"]) {
        return [defaults integerForKey:@"info_skill_multi_touch"];
    } else {
        return 1;
    }
}
int GameUtilities_getSkillSkip() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_skip"]) {
        return [defaults integerForKey:@"info_skill_skip"];
    } else {
        return 1;
    }
}
int GameUtilities_getSkillWeak() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_weak"]) {
        return [defaults integerForKey:@"info_skill_weak"];
    } else {
        return 1;
    }
}
int GameUtilities_getSkillSTouch() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_s_touch"]) {
        return [defaults integerForKey:@"info_skill_s_touch"];
    } else {
        return 1;
    }
}
int GameUtilities_getSkillLargeTouch() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults valueForKey:@"info_skill_large_touch"]) {
        return [defaults integerForKey:@"info_skill_large_touch"];
    } else {
        return 1;
    }
}

void GameUtilities_removeSkillInfo() {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults removeObjectForKey:@"info_skill_life"];
    [defaults removeObjectForKey:@"info_skill_multi_touch"];
    [defaults removeObjectForKey:@"info_skill_skip"];
    [defaults removeObjectForKey:@"info_skill_weak"];
    [defaults removeObjectForKey:@"info_skill_s_touch"];
    [defaults removeObjectForKey:@"info_skill_large_touch"];
}

int GameUtilities_getRand(int32_t from, int32_t to){
    return (int)(from + (arc4random() % (to - from + 1)));
}

int GameUtilities_getRandId(int32_t levelId, bool add){
    //获取一个随机整数，范围在[from,to]，包括from，包括to
    int from = 0;
    int to = 14;
    
    if (levelId >= 0 && levelId <= 4) {
        from = 0;
        to = 14;
    } else if (levelId >= 5 && levelId <= 9) {
        from = 15;
        to = 29;
    } else if (levelId >= 10 && levelId <= 14) {
        from = 30;
        to = 44;
    } else if (levelId >= 15 && levelId <= 19) {
        from = 45;
        to = 59;
    } else if (levelId >= 20 && levelId <= 24) {
        from = 60;
        to = 74;
    } else if (levelId >= 25 && levelId <= 29) {
        from = 75;
        to = 89;
    }

    kai::game::GameModle* instance = kai::game::GameModle::sharedInstance();
    std::vector<int> levelList = instance->getLevelList();

    int level = GameUtilities_getRand(from, to);
    
    std::vector<int>::const_iterator iter = levelList.begin();
    for (; iter != levelList.end(); iter++) {
        if (level == *iter) {
            level = GameUtilities_getRandId(levelId, add);
            break;
        }
    }

    if (add) {
        instance->pushLevelId(level);
    }
    
    return level;
}