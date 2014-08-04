//
//  SubLevelsSelectionScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "SubLevelsSelectionScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameConfig.h"
#include "UiTool.h"
NS_KAI_BEGIN
SubLevelsSelectionScene::SubLevelsSelectionScene():CCLayer(),_levelsMenu(NULL),_levelsList(NULL)
{
	_gouNodeRoot = NULL;
	_starNodeRoot = NULL;
}

SubLevelsSelectionScene::~SubLevelsSelectionScene()
{
	CC_SAFE_RELEASE_NULL(_levelsMenu);
	CC_SAFE_RELEASE_NULL(_gouNodeRoot);
	CC_SAFE_RELEASE_NULL(_starNodeRoot);
}

SubLevelsSelectionScene* SubLevelsSelectionScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("SubLevelsSelectionScene",SubLevelsSelectionSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	SubLevelsSelectionScene * node =(SubLevelsSelectionScene*)ccBReader->readNodeGraphFromFile("sublevelsselection_layer.ccbi");
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

CCScene* SubLevelsSelectionScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void SubLevelsSelectionScene::press_back(CCObject *pSender, CCControlEvent pCCControlEvent)
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_LEVELS_SELECTION);
}

void SubLevelsSelectionScene::press_play(CCObject *pSender, CCControlEvent pCCControlEvent)
{
	CCMenuItem* item = (CCMenuItem*)pSender;
	GameModle * _sharedInstance = GameModle::sharedInstance();
	int zone = _sharedInstance->currentBigLevelId();
	if (item->getTag() == 0) {
		std::stringstream strid;
		strid << "have_play_before_" << zone << std::endl;
        if (!CCUserDefault::sharedUserDefault()->getBoolForKey(strid.str().c_str())) {
            CCUserDefault::sharedUserDefault()->setBoolForKey(strid.str().c_str(), true);
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_LEVEL_MV
                                                          ,CCInteger::create(zone)
                                                          ,CCInteger::create(item->getTag()));
            
            return;
        }
	}

    if (GameModle::sharedInstance()->getHelpIndex(item->getTag(), zone) == -1) {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_PRE,
                                                      CCInteger::create(item->getTag()));
    } else {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_HELP_IN_LEVEL
                                                      ,CCInteger::create(zone)
                                                      ,CCInteger::create(item->getTag()));
    }
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler SubLevelsSelectionScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",SubLevelsSelectionScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_play",SubLevelsSelectionScene::press_play);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler SubLevelsSelectionScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press_back", SubLevelsSelectionScene::press_back);
	CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press_play", SubLevelsSelectionScene::press_play);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool SubLevelsSelectionScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "levels_menu", CCMenu *, this->_levelsMenu);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gou_node", CCNode *, this->_gouNodeRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star_node", CCNode *, this->_starNodeRoot);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void SubLevelsSelectionScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("SubLevelsSelectionScene onNodeLoaded~") ;
	
	//获得小关卡列表
	CCDictionary * biglevelsDict=(CCDictionary *)GameConfig::sharedInstance()->getValue(KStrLevels);
	GameModle * _sharedInstance = GameModle::sharedInstance();
	std::stringstream key;
	key << _sharedInstance->currentBigLevelId();
	CCDictionary * biglevelInfoDict=(CCDictionary * )biglevelsDict->objectForKey(key.str());
	_levelsList =(CCArray *) biglevelInfoDict->objectForKey(KStrSubLevels);
	//加载对应的关卡列表
	if (_levelsMenu) {
		
		CCArray * children=_levelsMenu->getChildren();
		PlayerAchievement lastachievement;
		lastachievement._currentBigLevelId = _sharedInstance->currentBigLevelId();
		lastachievement._currentLevelId = -1;
		std::vector<PlayerAchievement> list = GameData::Instance().findData(-1,lastachievement._currentBigLevelId);
		const int list_size = list.size();
		if (list_size > 0) {
			lastachievement._currentLevelId = list[list_size -1]._currentLevelId;
		}
		
		
		//显示总的分数情况
		int pass_size = 0;
		int star_size = 0;
		//160,80
		for (std::vector<PlayerAchievement>::iterator iter = list.begin(); iter != list.end(); iter++) {
			pass_size ++;
			if ((*iter)._star_count > 0) {
				star_size ++;
			}
		}
		
		std::stringstream gouStr;
		gouStr << pass_size << "/" << 15;
		std::stringstream starStr;
		starStr << star_size << "/" << 15;

		CCSize labelSize;
		if (_gouNodeRoot) {
			labelSize =  _gouNodeRoot->getContentSize();
            CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(gouStr.str());
			label->setAnchorPoint(CCPoint(0, 0.5));
			label->setPosition(CCPoint(0, (labelSize.height) * 0.5));
			_gouNodeRoot->addChild(label);
		}

		
		if (_starNodeRoot) {
			labelSize =  _starNodeRoot->getContentSize();
            CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(starStr.str());
			label->setAnchorPoint(CCPoint(0, 0.5));
			label->setPosition(CCPoint(0, (labelSize.height) * 0.5));
			_starNodeRoot->addChild(label);
		}

		
		
		
		
		CCSpriteFrameCache * _sharedSpriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		for (int i = 0; i < children->count(); i++) {
			CCMenuItemImage* item = (CCMenuItemImage*)children->objectAtIndex(i);
			int32_t tmpLevelId = item->getTag();
			int foundIndex = -1;
			for (int j = 0; j < list.size(); j++) {
				const PlayerAchievement & achievement = list[j];
				if (achievement._currentLevelId == tmpLevelId) {
					foundIndex = j;
					break;
				}
			}
			
			CCNode * subNodeRoot = this->getChildByTag(2001 + i);
			if (subNodeRoot) {
				subNodeRoot->setVisible(false);
				if (foundIndex != -1) {
					const PlayerAchievement & achievement = list[foundIndex];
					subNodeRoot->setVisible(true);
					CCNode * txtNodeRoot = this->getChildByTag(1100 + i);
					if (txtNodeRoot) {
						std::stringstream txtStr;
						txtStr << achievement._killNpcCount << "/" << achievement._totalNpcCount;
						
						CCSize labelSize =  txtNodeRoot->getContentSize();
                        CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(txtStr.str());
						CCSize txtSize =  label->getContentSize();
						label->setAnchorPoint(CCPoint(0.5, 0.5));
						label->setPosition(CCPoint(labelSize.width * 0.5,labelSize.height* 0.5));
						txtNodeRoot->addChild(label);
					}
					
				}
			}
			
			CCSpriteFrame *normalSpriteFrame = NULL;
			CCSpriteFrame *selectedSpriteFrame = NULL;
			CCSpriteFrame *disabledSpriteFrame = NULL;

			if (foundIndex != -1) {

				item->setEnabled(true);
				
				if (list[foundIndex]._star_count >= 1) {
					
					switch (lastachievement._currentBigLevelId) {
						case 0:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_1.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_1.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_1.png");
						}
							break;
						case 1:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_2.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_2.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_2.png");
						}
							break;
						case 2:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_3.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_3.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_3.png");
						}
							break;
						default:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_3.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_3.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_perfect_sel_3.png");
							
						}
							break;
					}
				}
				else {
					
					switch (lastachievement._currentBigLevelId) {
						case 0:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_1.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_1.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_1.png");
						}
							break;
						case 1:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_2.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_2.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_2.png");
						}
							break;
						case 2:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_3.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_3.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_3.png");
						}
							break;
						default:
						{
							normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_3.png");
							selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_3.png");
							disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_pass_sel_3.png");
							
						}
							break;
					}	
				}
			}
#ifdef KOpenAllLevels
            else if(1){
#else
            else if(tmpLevelId <= lastachievement._currentLevelId + 1){
#endif
				
				item->setEnabled(true);
				switch (lastachievement._currentBigLevelId) {
					case 0:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_1.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_1.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_1.png");
					}
						break;
					case 1:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_2.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_2.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_2.png");
					}
						break;
					case 2:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_3.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_3.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_3.png");
					}
						break;
					default:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_3.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_3.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_fail_sel_3.png");
						
					}
						break;
				}
			}
			else {
#ifdef KBattleTestModle
								item->setEnabled(true);
#else
								item->setEnabled(false);
#endif
				
				switch (lastachievement._currentBigLevelId) {
					case 0:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_1.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_1.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_1.png");
					}
						break;
					case 1:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_2.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_2.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_2.png");
					}
						break;
					case 2:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
					}
						break;
					default:
					{
						normalSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
						selectedSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
						disabledSpriteFrame = _sharedSpriteFrameCache->spriteFrameByName("biglevel_lock_3.png");
						
					}
						break;
				}
				
			}
			
			item->setNormalSpriteFrame(normalSpriteFrame);
			item->setSelectedSpriteFrame(selectedSpriteFrame);
			item->setDisabledSpriteFrame(disabledSpriteFrame);
		}
	}
}

void SubLevelsSelectionScene::onEnter(){
	CCLayer::onEnter();
}

void SubLevelsSelectionScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

