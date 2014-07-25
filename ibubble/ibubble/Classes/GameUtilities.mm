//
//  GameUtilities.m
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//
#import <Foundation/Foundation.h>
#include "cocos2d.h"
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
void GameUtilities_saveLevelId(int levelId){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSNumber numberWithInt:levelId] forKey:@"default_levelId"];
}

int GameUtilities_getLevelId(){
	NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    return [[defaults objectForKey:@"default_levelId"] intValue];
}

int GameUtilities_getRandId(int32_t levelId){
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
    
    return (int)(from + (arc4random() % (to - from + 1)));
}