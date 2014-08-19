//
//  UiTool.cpp
//  ibubble
//
//  Created by Ryan_Y on 13-5-30.
//
//

#include "UiTool.h"
USING_NS_CC;

CCLabelAtlas * UiTool::createLabelAtlasWithBigNumber(const std::string & text){
	CCLabelAtlas * label = CCLabelAtlas::create(text.c_str(), "scene_bignumber.png", 30, 30, '.');
	return label;
}

cocos2d::CCLabelAtlas * UiTool::createLabelAtlasWithLittleNumber(const std::string & text){
	CCLabelAtlas * label = CCLabelAtlas::create(text.c_str(), "scene_littlenumber.png", 18, 18, '.');
	return label;
}

cocos2d::CCLabelAtlas * UiTool::createLabelAtlasWithStarNumber(const std::string & text){
	CCLabelAtlas * label = CCLabelAtlas::create(text.c_str(), "startlevel_number.png", 30, 30, '-');
	return label;
}

cocos2d::CCLabelAtlas * UiTool::createLabelAtlas(const std::string & text,
										  const EFontSize type,
										  const cocos2d::ccColor3B & color){
	
	cocos2d::CCLabelAtlas * lable = NULL;
	const std::vector<std::string> array = string2Array(text,"/");
	for (int i = 0; i < array.size(); i++) {
		switch (type) {
			case kFontSmall:
				lable = createLabelAtlasWithLittleNumber(text);
				break;
			case kFontMidlle:
				lable = createLabelAtlasWithStarNumber(text);
				break;
			case kFontBig:
				lable = createLabelAtlasWithBigNumber(text);
				break;
			default:
				break;
		}
	}
	

	return lable;
}


cocos2d::CCLabelTTF * UiTool::createLabel(const std::string & text,
										  const EFontSize type,
										  const cocos2d::ccColor3B & color){
	float  fontsize  = 16;
	switch (type) {
		case kFontSmall:
			fontsize = 16;
			break;
		case kFontMidlle:
			fontsize = 28;
			break;
		case kFontBig:
			fontsize = 40;
			break;
		default:
			break;
	}
	
	cocos2d::CCLabelTTF * ttf = cocos2d::CCLabelTTF::create(text.c_str(),
															"MarkerAFelt-Thin",
															fontsize);
	ttf->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
	ttf->setColor(color);
	return ttf;
}


cocos2d::CCLabelBMFont * UiTool::createLabelBMFont(const std::string & text,
										  const EFontSize type,
										  const cocos2d::ccColor3B & color){
	cocos2d::CCLabelBMFont * ttf = NULL;
	switch (type) {
		case kFontSmall:
			ttf = cocos2d::CCLabelBMFont::create(text.c_str(),
												 "marker_felt_16_white.fnt");
			break;
		case kFontMidlle:
			ttf = cocos2d::CCLabelBMFont::create(text.c_str(),
												 "marker_felt_28_white.fnt");
			break;
		case kFontBig:
			ttf = cocos2d::CCLabelBMFont::create(text.c_str(),
												 "marker_felt_40_white.fnt");
			break;
		default:
			ttf = cocos2d::CCLabelBMFont::create(text.c_str(),
												 "marker_felt_16_white.fnt");
			break;
	}
	ttf->setAlignment(kCCTextAlignmentCenter);
	return ttf;
}
//label 描边
CCRenderTexture* UiTool::createStroke(cocos2d::CCLabelTTF *label,
									  float size,
									  cocos2d::ccColor3B color)
{
    float x = label->getTexture()->getContentSize().width + size * 2;
    float y = label->getTexture()->getContentSize().height + size * 2;
    CCRenderTexture *rt = CCRenderTexture::create(x, y);
    CCPoint originalPos = label->getPosition();
    ccColor3B originalColor = label->getColor();
    label->setColor(color);
    ccBlendFunc originalBlend = label->getBlendFunc();
    label->setBlendFunc((ccBlendFunc){GL_SRC_ALPHA,GL_ONE});
    CCPoint center = ccp(x/2+size, y/2+size);
    rt->begin();
    for (int i=0; i < 360; i += 6) {
		float _x = center.x + sin(CC_DEGREES_TO_RADIANS(i))*size;
		float _y = center.y + cos(CC_DEGREES_TO_RADIANS(i))*size;
		label->setPosition(ccp(_x, _y));
		label->visit();
    }
    rt->end();
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    float rtX = originalPos.x - size;
    float rtY = originalPos.y - size;
    rt->setPosition(ccp(rtX, rtY));
    return rt;
}


CCSprite* UiTool::createLableWithStroke(const std::string & text,
							   const EFontSize type,
							   const cocos2d::ccColor3B & color,
							   const cocos2d::ccColor3B & strokecColor){
	CCSprite * lableSprite = NULL;
	cocos2d::CCLabelTTF * label = createLabel(text,type,color);
	float  fontStrokeSize  = 1;
	CCRenderTexture * rt = createStroke(label,fontStrokeSize,strokecColor);
	CCTexture2D *texture = rt->getSprite()->getTexture();
    texture->setAliasTexParameters();
    
	CCSprite * strokeSprite = CCSprite::createWithTexture(rt->getSprite()->getTexture());
    strokeSprite->setFlipY(true);
	
	lableSprite = CCSprite::create();
	lableSprite->setContentSize(strokeSprite->getContentSize());
	lableSprite->addChild(strokeSprite);
	label->setAnchorPoint(ccp(0.5, 0.5));
	lableSprite->addChild(label);
	return lableSprite;
}

const std::vector<std::string>  UiTool::string2Array(const std::string & text,const std::string & tag){
	std::vector<std::string> vals;
	std::string srcString = text;
    size_t startPos = srcString.find_first_of(tag);
    while(startPos != std::string::npos) {
		std::string tmp = srcString.substr(0,startPos);
		vals.push_back(tmp);
        srcString = srcString.substr(startPos+tag.length());
		startPos = srcString.find_first_of(tag);
        if(startPos == std::string::npos){
			vals.push_back(srcString);
        }
    }
    return vals;
}
