//
//  GameUtilities.h
//  ibubble
//
//  Created by Ryan Yuan on 13-1-20.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameUtilities_h
#define ibubble_GameUtilities_h
#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

#include "GameData.h"

class GameUtilities {
public:
	static void writeDateToFile(const char * aBuffer,const int aBufferLength,const std::string &filepath);
	static std::string getFilePathInDocumentDir(const std::string &filepath);
	static bool fileExistsAtPath(const std::string &filepath);
    
    static int getRandLevel(bool add=true);
    static void saveLevelId(int levelId, kai::game::PlayerData plaerData);
    static int getLevelId();
    static int getRand(uint32_t from, uint32_t to);
};

#endif
