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
    return [[defaults objectForKey:[kUnlockidHeadString stringByAppendingFormat:@"%d", achieveId]] boolValue];
}

void GameUtilities_saveLevelInfo(int levelId, kai::game::PlayerData plaerData){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    
    NSMutableDictionary* infoDic = [[NSMutableDictionary alloc] init];
    [infoDic setObject:[NSNumber numberWithInt:levelId] forKey:@"info_levelId"];

    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skillLife] forKey:@"info_skill_life"];
    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skill_multi_touch] forKey:@"info_skill_multi_touch"];
    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skill_skip] forKey:@"info_skill_skip"];
    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skill_weak] forKey:@"info_skill_weak"];
    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skill_s_touch] forKey:@"info_skill_s_touch"];
    [infoDic setObject:[NSNumber numberWithInt:plaerData.skillInfo.skill_large_touch] forKey:@"info_skill_large_touch"];
    
    [defaults setObject:infoDic forKey:@"default_levelInfo"];
    [defaults synchronize];
}

int GameUtilities_getLevelInfo(){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary* infoDic = [defaults objectForKey:@"default_levelInfo"];
    
    kai::game::PlayerData plaerData = kai::game::GameData::Instance().playerData;
    plaerData.skillInfo.skillLife = [[infoDic valueForKey:@"info_skill_life"] intValue];
    plaerData.skillInfo.skill_multi_touch = [[infoDic valueForKey:@"info_skill_multi_touch"] intValue];
    plaerData.skillInfo.skill_skip = [[infoDic valueForKey:@"info_skill_skip"] intValue];
    plaerData.skillInfo.skill_weak = [[infoDic valueForKey:@"info_skill_weak"] intValue];
    plaerData.skillInfo.skill_s_touch = [[infoDic valueForKey:@"info_skill_s_touch"] intValue];
    plaerData.skillInfo.skill_large_touch = [[infoDic valueForKey:@"info_skill_large_touch"] intValue];

    return [[infoDic valueForKey:@"info_levelId"] intValue];
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