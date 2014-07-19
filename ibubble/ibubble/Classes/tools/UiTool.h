//
//  UiTool.h
//  ibubble
//
//  Created by Ryan_Y on 13-5-30.
//
//

#ifndef ibubble_UiTool_h
#define ibubble_UiTool_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include <string>

class UiTool {
public:
	enum EFontSize
	{
		kFontSmall = 0,
		kFontMidlle,
		kFontBig,
	};
public:
	static cocos2d::CCLabelAtlas * createLabelAtlasWithBigNumber(const std::string & text);
	static cocos2d::CCLabelAtlas * createLabelAtlasWithLittleNumber(const std::string & text);
	static cocos2d::CCLabelAtlas * createLabelAtlasWithStarNumber(const std::string & text);	
	
	static cocos2d::CCLabelTTF * createLabel(const std::string & text,
											 const EFontSize type,
											 const cocos2d::ccColor3B & color);
	static cocos2d::CCLabelBMFont * createLabelBMFont(const std::string & text,
													   const EFontSize type,
													  const cocos2d::ccColor3B & color);
	static cocos2d::CCRenderTexture * createStroke(cocos2d::CCLabelTTF *label,
												  float size,
												  cocos2d::ccColor3B color);
	
	static cocos2d::CCSprite* createLableWithStroke(const std::string & text,
											const EFontSize type,
											const cocos2d::ccColor3B & color,
											const cocos2d::ccColor3B & strokecColor);
	
	
	static  cocos2d::CCLabelAtlas * createLabelAtlas(const std::string & text,
													 const EFontSize type,
													 const cocos2d::ccColor3B & color);
	
	
	static const std::vector<std::string>  string2Array(const std::string & text,const std::string & tag);
};
#endif
