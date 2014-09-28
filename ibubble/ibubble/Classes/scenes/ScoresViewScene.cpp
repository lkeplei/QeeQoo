//
//  ScoresViewScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ScoresViewScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameObject.h"
#include "UiTool.h"
#include "GameCenter.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
ScoresViewScene::ScoresViewScene():CCLayer()
, _displaySpriteRoot(NULL)
, _gouNode1(NULL)
, _starNode1(NULL)
, _rewardNode1(NULL)
, _gouNode2(NULL)
, _starNode2(NULL)
, _rewardNode2(NULL)
, _rewardGoodNode2(NULL)
{

}

ScoresViewScene::~ScoresViewScene()
{
	CC_SAFE_RELEASE_NULL(_displaySpriteRoot);
    CC_SAFE_RELEASE_NULL(_gouNode1);
    CC_SAFE_RELEASE_NULL(_starNode1);
    CC_SAFE_RELEASE_NULL(_rewardNode1);
    CC_SAFE_RELEASE_NULL(_gouNode2);
    CC_SAFE_RELEASE_NULL(_starNode2);
    CC_SAFE_RELEASE_NULL(_rewardNode2);
    CC_SAFE_RELEASE_NULL(_rewardGoodNode2);
}

ScoresViewScene* ScoresViewScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ScoresViewScene",ScoresViewSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	ScoresViewScene * node =(ScoresViewScene*)ccBReader->readNodeGraphFromFile("scores_layer.ccbi");
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

CCScene* ScoresViewScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void ScoresViewScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME);
}

void ScoresViewScene::press_rank_list()
{
    GameCenter::showScores();
}

bool ScoresViewScene::init(){
	return  true;
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ScoresViewScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",ScoresViewScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_rank_list",ScoresViewScene::press_rank_list);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler ScoresViewScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", ScoresViewScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ScoresViewScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "display_node", CCNode *, _displaySpriteRoot);

    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gou_node_1", CCNode *, _gouNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star_node_1", CCNode *, _starNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reward_node_1", CCNode *, _rewardNode1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gou_node_2", CCNode *, _gouNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star_node_2", CCNode *, _starNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reward_node_2", CCNode *, _rewardNode2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reward_good_node_2", CCNode *, _rewardGoodNode2);
    
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
int getStoryUnlock() {
    int number = 0;
    
    if (GameUtilities::getUnlockWithId(10001))
        number++;
    if (GameUtilities::getUnlockWithId(10003))
        number++;
    if (GameUtilities::getUnlockWithId(10004))
        number++;
    if (GameUtilities::getUnlockWithId(10009))
        number++;
    if (GameUtilities::getUnlockWithId(10010))
        number++;
    
    return number;
}

int getAchievementUnlock() {
    int number = 0;

    if (GameUtilities::getUnlockWithId(10002))
        number++;
    if (GameUtilities::getUnlockWithId(10005))
        number++;
    if (GameUtilities::getUnlockWithId(10006))
        number++;
    if (GameUtilities::getUnlockWithId(10007))
        number++;
    if (GameUtilities::getUnlockWithId(10008))
        number++;
    
    return number;
}

void ScoresViewScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
    std::pair<int, int> counts0 = GameData::Instance().totalCount(-1, 0);
    std::pair<int, int> counts1 = GameData::Instance().totalCount(-1, 1);
    std::pair<int, int> counts2 = GameData::Instance().totalCount(-1, 2);
    std::pair<int, int> count(0, 0);
    count.first = counts0.first + counts1.first + counts2.first;
    count.second = counts0.second + counts1.second + counts2.second;
    GameData::Instance().unlockStory(count);
    
    
    std::stringstream gouStr;
    gouStr << count.second << "/" << 45;
    _gouNode1->addChild(UiTool::createLabelAtlasWithBigNumber(gouStr.str()));
    
    std::stringstream starStr;
    starStr << count.first << "/" << 45;
    _starNode1->addChild(UiTool::createLabelAtlasWithBigNumber(starStr.str()));

    std::stringstream rewardStr;
    rewardStr << getStoryUnlock() << "/" << 5;
    _rewardNode1->addChild(UiTool::createLabelAtlasWithBigNumber(rewardStr.str()));
    
    std::stringstream gouStr2;
    gouStr2 << GameUtilities::getMaxPass();
    _gouNode2->addChild(UiTool::createLabelAtlasWithBigNumber(gouStr2.str()));
    
    std::stringstream starStr2;
    starStr2 << GameUtilities::getMaxStar();
    _starNode2->addChild(UiTool::createLabelAtlasWithBigNumber(starStr2.str()));
    
    std::stringstream rewardStr2;
    rewardStr2 << getAchievementUnlock() << "/" << 5;
    _rewardNode2->addChild(UiTool::createLabelAtlasWithBigNumber(rewardStr2.str()));
    
    stringstream rewardGoodNode2;
    rewardGoodNode2 << GameUtilities::getMaxRecord();
    _rewardGoodNode2->addChild(UiTool::createLabelAtlasWithBigNumber(rewardGoodNode2.str()));
}

void ScoresViewScene::onEnter(){
	CCLayer::onEnter();
}

void ScoresViewScene::onExit(){
	
	CCLayer::onExit();
}

NS_KAI_END

