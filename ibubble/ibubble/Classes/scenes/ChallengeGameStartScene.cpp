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

void ChallengeGameStartScene::press_next(){
    //冲第一关卡开始
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,
                                                  CCInteger::create(0));
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeGameStartScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_next",ChallengeGameStartScene::press_next);
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
    //TODO:读取数据
    createValText(_titleNode,"25",UiTool::kFontBig);
    createValText(_passCount,"25/100",UiTool::kFontMidlle);
    createValText(_starCount,"25/100",UiTool::kFontMidlle);
}
NS_KAI_END

