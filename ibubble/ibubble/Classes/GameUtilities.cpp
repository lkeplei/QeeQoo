//
//  GameUtilities.cpp
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#include "GameUtilities.h"

extern void GameUtilities_writeDateToFile(const char * aBuffer,const int aBufferLength,const char *pFileName);
extern const char * GameUtilities_getFilePathInDocumentDir(const char *pFileName);
extern bool GameUtilities_fileExistsAtPath(const char *pFileName);

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