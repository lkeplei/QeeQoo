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

class GameUtilities {
public:
	static void writeDateToFile(const char * aBuffer,const int aBufferLength,const std::string &filepath);
	static std::string getFilePathInDocumentDir(const std::string &filepath);
	static bool fileExistsAtPath(const std::string &filepath);
};

#endif
