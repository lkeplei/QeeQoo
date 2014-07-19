//
//  GameEffectManager.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameEffectManager_h
#define ibubble_GameEffectManager_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

NS_KAI_BEGIN

class PlayEffectCallback;
class GameObject;
class  GameEffectManager : public CCObject
{
public:
	enum EGameEffectType {
		K_EFFECT_PARTICLE 			 =1,
		K_EFFECT_IMAGE_ANIMATION     =2,
		K_EFFECT_AUDIO   			 =3,
	};
protected:
	GameEffectManager();
	virtual ~GameEffectManager();
	
	/** init the new created instance of GameEffectManager*/
	bool init(void);
public:
    /** returns a shared instance of GameEffectManager */
    static GameEffectManager *sharedInstance(void);
    /** purge the shared instance of GameEffectManager */
    static void purgeInstance(void);

	void loadAudioEffect(CCDictionary * audios);
	void releaseAudioEffect(CCDictionary * audios);
	void playBackground(const std::string & name,const bool stop2play=false);
	CCNode *  playEffect(const std::string & effectId,
                         CCNode *targetLayer,
                         const CCPoint & position,
                         CCAction * action=NULL);
	
	float getBackgroundMusicVolume();
	void setBackgroundMusicVolume(float volume);
	
	float getEffectsVolume();
	void setEffectsVolume(float volume);
	
private:
	CCDictionary * _properties;
	CocosDenshion::SimpleAudioEngine * _simpleAudioEngine;
	std::string  _backgroundMusicName;
};

NS_KAI_END
#endif
