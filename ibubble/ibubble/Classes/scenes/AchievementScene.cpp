//
//  AchievementScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "AchievementScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameConfig.h"
#include "UiTool.h"

#include "GameUtilities.h"
NS_KAI_BEGIN
AchievementScene::AchievementScene():CCLayer()
{
	
}

AchievementScene::~AchievementScene()
{

}

AchievementScene* AchievementScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("AchievementScene",AchievementSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	AchievementScene * node =(AchievementScene*)ccBReader->readNodeGraphFromFile("achievement_layer.ccbi");
	if (node) {
		//node->retain();
		CC_SAFE_RELEASE_NULL(ccBReader);
		node->autorelease();
	}
	else {
		CC_SAFE_RELEASE_NULL(ccBReader);
	}
	return node;
}

CCScene* AchievementScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void AchievementScene::press_back(CCObject *pSender, CCControlEvent pCCControlEvent)
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void AchievementScene::press_next(CCObject *pSender, CCControlEvent pCCControlEvent)
{

	CCDictionary * rewardsDict = (CCDictionary *)GameConfig::sharedInstance()->getValue(KStrRewards);
	if (rewardsDict) {
		int tag = ((CCNode *)pSender)->getTag();
		
#ifndef KOpenAllLevels
		if (GameUtilities::getUnlockWithId(tag))
#endif
		{
			std::stringstream key;
			key << tag;
			CCDictionary * dict =( CCDictionary * )rewardsDict->objectForKey(key.str());
			if (dict) {
				
				CCString * levelID = (CCString * )dict->objectForKey(KStrLevelId);
				CCString * zoneID = (CCString * )dict->objectForKey(KStrZoneId);
				if (levelID && zoneID) {
					GameModle::sharedInstance()->setCurrentBigLevelId(zoneID->intValue());
					GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE,CCInteger::create(levelID->intValue()),
                                                                  CCInteger::create(zoneID->intValue()));
				}
				else{
					GameController::sharedInstance()->switchSence(GameController::K_SCENE_VIEW_ACHIEVEMENT,dict);
				}
			}
		}
	}

}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler AchievementScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",AchievementScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "enter_view",AchievementScene::press_next);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler AchievementScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press_back", AchievementScene::press_back);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "enter_view", AchievementScene::press_next);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool AchievementScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void AchievementScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("AchievementScene onNodeLoaded~") ;
    for (int i = 10001; i < 10011; i++) {
        CCMenu *menu = (CCMenu *)this->getChildByTag(10000);
        if (menu) {
            CCMenuItemImage *enterItem = (CCMenuItemImage *)menu->getChildByTag(i);
            CCMenuItemImage *noneItem = (CCMenuItemImage *)menu->getChildByTag(i + 1000);
            if (enterItem && noneItem) {
                if (GameUtilities::getUnlockWithId(i)) {
                    
                } else {
                    CCSprite* sprite = CCSprite::create("btn_reward_locked.png");
                    sprite->setPosition(enterItem->getPosition());
                    addChild(sprite);
                    
                    CCSprite* sprite1 = CCSprite::create(getSpriteNameFromTag(noneItem->getTag()));
                    sprite1->setPosition(noneItem->getPosition());
                    addChild(sprite1);
                    
                    enterItem->setVisible(false);
                    noneItem->setVisible(false);
                }
            }
        }
    }
}

const char *AchievementScene::getSpriteNameFromTag(int tag) {
    switch (tag) {
        case 11001:
            return "reward_level_2_locked.png";
            break;
        case 11002:
            return "reward_level_9_locked.png";
            break;
        case 11003:
            return "reward_level_1_locked.png";
            break;
        case 11004:
            return "reward_level_5_locked.png";
            break;
        case 11005:
            return "reward_level_10_locked.png";
            break;
        case 11006:
            return "reward_level_6_locked.png";
            break;
        case 11007:
            return "reward_level_7_locked.png";
            break;
        case 11008:
            return "reward_level_8_locked.png";
            break;
        case 11009:
            return "reward_level_3_locked.png";
            break;
        case 11010:
            return "reward_level_4_locked.png";
            break;
        default:
            return "reward_level_1_locked.png";
            break;
    }
}

void AchievementScene::onEnter(){
	CCLayer::onEnter();
}

void AchievementScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

