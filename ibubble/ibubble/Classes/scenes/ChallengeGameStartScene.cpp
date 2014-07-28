//
//  ChallengeGameStartScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ChallengeGameStartScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "GameConfig.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
ChallengeGameStartScene::ChallengeGameStartScene():CCLayer()
,_passCount(NULL)
,_starCount(NULL)
,_titleNode(NULL)
{

}

ChallengeGameStartScene::~ChallengeGameStartScene()
{
    CC_SAFE_RELEASE_NULL(_passCount);
    CC_SAFE_RELEASE_NULL(_starCount);
    CC_SAFE_RELEASE_NULL(_titleNode);
}

ChallengeGameStartScene* ChallengeGameStartScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ChallengeGameStartScene",ChallengeGameStartSceneLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	ChallengeGameStartScene * node = (ChallengeGameStartScene*)ccBReader->readNodeGraphFromFile("ChallengeGameStartScene.ccbi");
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

CCScene* ChallengeGameStartScene::scene(const int type)
{
	CCScene *scene = CCScene::create();
	ChallengeGameStartScene * node = createWithCCB();
	if(node != NULL){
        node->_type = type;
		scene->addChild(node);
	}
	return scene;
}

void ChallengeGameStartScene::play_next(){
    //冲第一关卡开始
    GameModle::sharedInstance()->setCurrentHardLevelId(GameUtilities::getLevelId());
    GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,
                                                  CCInteger::create(currentLevel));
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeGameStartScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "play_next",ChallengeGameStartScene::play_next);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler ChallengeGameStartScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ChallengeGameStartScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "passCount", CCNode *,_passCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "starCount", CCNode *,_starCount);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "titleNode", CCNode *,_titleNode);
	return false;
}

void createValText(CCNode * container,const std::string & text ,UiTool::EFontSize size){
    if (container) {
        container->removeAllChildrenWithCleanup(true);
        CCSize labelSize =  container->getContentSize();
        cocos2d::CCLabelBMFont * label = UiTool::createLabelBMFont(text,
                                                                   size,
                                                                   ccc3(255,255,255));
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        container->addChild(label);
    }
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ChallengeGameStartScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
    currentLevel = GameUtilities::getRandLevel();
    
    
    CCDictionary * dict = (CCDictionary *)GameConfig::sharedInstance()->getLevelsValue("4");
    CCArray * sublevels = (CCArray *)dict->objectForKey(KStrSubLevels);
    CCDictionary * currentLevelDict = (CCDictionary *)sublevels->objectAtIndex(currentLevel);
    CCArray * npcs = (CCArray *)currentLevelDict->objectForKey(KStrNPCs);//npcs
    
    CCObject * npcitem = NULL;
    int pass_count = ((CCString *)currentLevelDict->objectForKey(KStrPassCount))->intValue();
    int star_count = ((CCString *)currentLevelDict->objectForKey(KStrStarCount))->intValue();
    int count = 0;
    CCARRAY_FOREACH(npcs, npcitem){
        CCDictionary * dict = (CCDictionary *)npcitem;
        count += ((CCString *)dict->objectForKey(KStrCount))->intValue();
    }
    
    
    GameModle* model = GameModle::sharedInstance();
    
    stringstream levelStr;
    levelStr << model->currentHardLevelId();
    createValText(_titleNode, levelStr.str(), UiTool::kFontBig);
    
    stringstream passCount;
    passCount << pass_count << "/" << count;
    createValText(_passCount, passCount.str(), UiTool::kFontMidlle);
    
    stringstream starCount;
    starCount << star_count << "/" << count;
    createValText(_starCount, starCount.str(), UiTool::kFontMidlle);
}
NS_KAI_END

