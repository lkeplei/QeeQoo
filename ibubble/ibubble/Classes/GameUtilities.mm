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
