//
//  ChallengeGameOverScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "ChallengeGameOverScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"

#include "GameCenter.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
ChallengeGameOverScene::ChallengeGameOverScene():CCLayer()
,_passTitle(NULL)
,_failTitle(NULL)
,_starTitle(NULL)
,_viciTitle(NULL)
,_boobeeTitle(NULL)
,_roodaTitle(NULL)
,_woowooTitle(NULL)
,_normal_flowers(NULL)
,_normal_gou(NULL)
,_normal_good(NULL)
,_normal_stars(NULL)
,_normal_total(NULL)
,_normal_records_node(NULL)
,_normal_stars_node(NULL)
,_normal_total_node(NULL)
,_normalMenuBtn(NULL)
,_replayBtn(NULL)
,_normalNextBtn(NULL)
,_hardMenuBtn(NULL)
,_reward_node(NULL)
,_firstStep(true)
{

}

ChallengeGameOverScene::~ChallengeGameOverScene()
{
    CC_SAFE_RELEASE_NULL(_passTitle);
    CC_SAFE_RELEASE_NULL(_failTitle);
    CC_SAFE_RELEASE_NULL(_starTitle);
    CC_SAFE_RELEASE_NULL(_viciTitle);
    CC_SAFE_RELEASE_NULL(_boobeeTitle);
    CC_SAFE_RELEASE_NULL(_roodaTitle);
    CC_SAFE_RELEASE_NULL(_woowooTitle);
    CC_SAFE_RELEASE_NULL(_normal_flowers);
    CC_SAFE_RELEASE_NULL(_normal_gou);
    CC_SAFE_RELEASE_NULL(_normal_good);
    CC_SAFE_RELEASE_NULL(_normal_stars);
    CC_SAFE_RELEASE_NULL(_normal_total);
    CC_SAFE_RELEASE_NULL(_normal_records_node);
    CC_SAFE_RELEASE_NULL(_normal_stars_node);
    CC_SAFE_RELEASE_NULL(_normal_total_node);
    CC_SAFE_RELEASE_NULL(_normalMenuBtn);
    CC_SAFE_RELEASE_NULL(_replayBtn);
    CC_SAFE_RELEASE_NULL(_normalNextBtn);
    CC_SAFE_RELEASE_NULL(_hardMenuBtn);
    CC_SAFE_RELEASE_NULL(_reward_node);
    
}

ChallengeGameOverScene* ChallengeGameOverScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ChallengeGameOverScene",ChallengeGameOverSceneLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	ChallengeGameOverScene * node = (ChallengeGameOverScene*)ccBReader->readNodeGraphFromFile("ChallengeGameOverScene.ccbi");
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

CCScene* ChallengeGameOverScene::scene(bool first)
{
	CCScene *scene=CCScene::create();
	ChallengeGameOverScene * node =(ChallengeGameOverScene*)createWithCCB();
	if(node != NULL){
		scene->addChild(node);
        
        node->setStep(first);
	}
	return scene;
}

void ChallengeGameOverScene::press_menu_page()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void ChallengeGameOverScene::press_replay()
{
    GameModle::sharedInstance()->getSkillInfo().skillLife--;
    
    int32_t currentLevelId = GameModle::sharedInstance()->currentLevelId();
	GameController::sharedInstance()->pauseBattle();
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,CCInteger::create(currentLevelId));
}

void ChallengeGameOverScene::press_play_next()
{
    if (_firstStep) {
        CCScene *pScene = ChallengeGameOverScene::scene(false);
        GameController::sharedInstance()->controllerPushSence(pScene);
    } else {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,
                                                      CCInteger::create(GameUtilities::getRandLevel()));
    }
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeGameOverScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_menu_page",ChallengeGameOverScene::press_menu_page);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_replay",ChallengeGameOverScene::press_replay);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_play_next",ChallengeGameOverScene::press_play_next);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler ChallengeGameOverScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", ChallengeGameOverScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ChallengeGameOverScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "passTitle", CCSprite *, _passTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "failTitle", CCSprite *, _failTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "starTitle", CCSprite *, _starTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "viciTitle", CCSprite *, _viciTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "boobeeTitle", CCSprite *, _boobeeTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "roodaTitle", CCSprite *, _roodaTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "woowooTitle", CCSprite *, _woowooTitle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_flowers", CCSprite *, _normal_flowers);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_gou", CCSprite *, _normal_gou);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_good", CCSprite *, _normal_good);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_stars", CCSprite *, _normal_stars);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_total", CCSprite *, _normal_total);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_records_node", CCNode *, _normal_records_node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_stars_node", CCNode *, _normal_stars_node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normal_total_node", CCNode *, _normal_total_node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normalMenuBtn", CCMenuItemImage *, _normalMenuBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "replayBtn", CCMenuItemImage *, _replayBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "normalNextBtn", CCMenuItemImage *, _normalNextBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "hardMenuBtn", CCMenuItemImage *, _hardMenuBtn);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "reward_node", CCNode *, _reward_node);
    
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ChallengeGameOverScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
    
    _passTitle->setVisible(false);
    _failTitle->setVisible(false);
    _starTitle->setVisible(false);
    _viciTitle->setVisible(false);
    _boobeeTitle->setVisible(false);
    _roodaTitle->setVisible(false);
    _woowooTitle->setVisible(false);
    
    _normal_flowers->setVisible(false);
    _normal_gou->setVisible(false);
    _normal_good->setVisible(false);
    _normal_stars->setVisible(false);
    _normal_total->setVisible(false);
    
    _normal_records_node->setVisible(false);
    _normal_stars_node->setVisible(false);
    _normal_total_node->setVisible(false);
    
    _normalMenuBtn->setVisible(false);
    _replayBtn->setVisible(false);
    _normalNextBtn->setVisible(false);
    _hardMenuBtn->setVisible(false);
    _reward_node->setVisible(false);
}

void createNodeValText(CCNode * container, const std::string & text){
    if (container) {
        container->removeAllChildrenWithCleanup(true);
        CCSize labelSize =  container->getContentSize();
        CCLabelAtlas *label = UiTool::createLabelAtlasWithBigNumber(text);
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        container->addChild(label);
    }
}

void createNodeReward(CCNode * container, const CCPoint& pos, CCSpriteFrame* spriteFrame, bool clear){
    if (container && spriteFrame) {
        if (clear) {
            container->removeAllChildrenWithCleanup(true);
        }
        
        CCSprite* sprite = CCSprite::createWithSpriteFrame(spriteFrame);
        sprite->setPosition(pos);

        container->addChild(sprite);
    }
}

CCSpriteFrame* setSkill(const int id){
    SkillInfo& info = GameModle::sharedInstance()->getSkillInfo();

    CCSpriteFrameCache * gShareCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* spriteFrame = NULL;
    
    switch (id) {
        case 1:{
            info.skillLife++;
            spriteFrame = gShareCache->spriteFrameByName("skill_life_btn.png");
        }
            break;
        case 2:{
            info.skill_skip++;
            spriteFrame = gShareCache->spriteFrameByName("skill_skip_btn.png");
        }
            break;
        case 3:{
            info.skill_weak++;
            spriteFrame = gShareCache->spriteFrameByName("skill_weake_btn.png");
        }
            break;
        case 4:{
            info.skill_s_touch++;
            spriteFrame = gShareCache->spriteFrameByName("skill_type_s_touch_btn.png");
        }
            break;
        case 5:{
            info.skill_large_touch++;
            spriteFrame = gShareCache->spriteFrameByName("skill_type_large_touch_btn.png");
        }
            break;
        case 6:{
            info.skill_multi_touch++;
            spriteFrame = gShareCache->spriteFrameByName("skill_multi_touch_btn.png");
        }
            break;
        default:
            break;
    }
    
    return spriteFrame;
}

void ChallengeGameOverScene::getSkill(const int type){
    if (!_firstStep) {
        CCSize size = _reward_node->getContentSize();
        switch (type) {
            case 1:{
                createNodeReward(_reward_node, CCPointMake(size.width / 2, size.height / 2), setSkill(GameUtilities::getRand(1, 6)), true);
            }
                break;
            case 2:{
                createNodeReward(_reward_node, CCPointMake(size.width * 0.25, size.height / 2), setSkill(1), true);
                createNodeReward(_reward_node, CCPointMake(size.width * 0.75, size.height / 2), setSkill(GameUtilities::getRand(2, 6)), false);
            }
                break;
            case 3:{
                GameModle::sharedInstance()->getSkillInfo().skillLife--;
            }
                break;
            default:
                break;
        }
    }
}

void ChallengeGameOverScene::setStep(bool first){
    GameModle* gModleInstance = GameModle::sharedInstance();
    PlayerAchievement& achievement = gModleInstance->playerAchievement();
    _firstStep = first;
    
    if (_firstStep) {
        if (achievement._killNpcCount >= achievement._pass_count) {
            show(4);
        } else if (GameModle::sharedInstance()->getSkillInfo().skillLife <= 0) {
            show(6);
        } else if (gModleInstance->currentHardLevelId() >= 29) {
            show(7);
        } else {
            show(5);
        }
    } else {
        if (achievement._killNpcCount >= achievement._pass_star_count){
            show(2);
        } else if (achievement._killNpcCount >= achievement._pass_count){
            show(1);
        } else {
            show(3);
        }
    }
}

void ChallengeGameOverScene::show(int type){
    getSkill(type);
    
    PlayerAchievement& playerAchieve = GameModle::sharedInstance()->playerAchievement();
    int percent = (int)((playerAchieve._killNpcCount * 100) / playerAchieve._totalNpcCount);
    if (percent > 80) {
        playerAchieve._records += ((percent - 80) * 50000 + 30 * 20000 + 50 * 10000) / 100;
    } else if (percent > 50) {
        playerAchieve._records += ((percent - 50) * 20000 + 50 * 10000) / 100;
    } else {
        playerAchieve._records +=  percent * 10000 / 100;
    }
    
    playerAchieve._records = percent == 100 ? playerAchieve._records * 2 : playerAchieve._records;
    
    playerAchieve._totalRecords += playerAchieve._records;
    
    GameCenter::postScore(KLeaderBoardId, playerAchieve._totalRecords);
    
    switch (type) {
        case 1:{//游戏结束界面1（过关）
            _passTitle->setVisible(true);
            _reward_node->setVisible(true);
            _normal_flowers->setVisible(true);
            _normal_good->setVisible(true);
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            _normalMenuBtn->setVisible(true);
            _normalNextBtn->setVisible(true);
        }
            break;
        case 2:{//游戏结束界面2（得星
            _starTitle->setVisible(true);
            _reward_node->setVisible(true);
            _normal_flowers->setVisible(true);
            _normal_good->setVisible(true);
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            _normalMenuBtn->setVisible(true);
            _normalNextBtn->setVisible(true);
        }
            break;
        case 3:{//游戏结束界面3（失败）
            _failTitle->setVisible(true);
            _normal_flowers->setVisible(true);
            _normal_good->setVisible(true);
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            _normalMenuBtn->setVisible(true);
            _normalNextBtn->setVisible(true);
            _normalNextBtn->setEnabled(false);
        }
            break;
        case 4: {//游戏结束界面4（过关、得星）
            _viciTitle->setVisible(true);
            _reward_node->setVisible(true);
            
            _normal_gou->setVisible(true);
            _normal_stars->setVisible(true);
            
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            
            _normalMenuBtn->setVisible(true);
            _normalNextBtn->setVisible(true);
            
            _normal_total->setVisible(true);
            _normal_total_node->setVisible(true);
        }
            break;
        case 5: {//游戏结束界面5（失败））
            _boobeeTitle->setVisible(true);
            _reward_node->setVisible(true);
            
            _normal_gou->setVisible(true);
            _normal_stars->setVisible(true);
            
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            
            _normalMenuBtn->setVisible(true);
            _replayBtn->setVisible(true);
            
            _normal_total->setVisible(true);
            _normal_total_node->setVisible(true);
        }
            break;
        case 6: {//游戏结束界面6（挑战失败）
            _woowooTitle->setVisible(true);
            _reward_node->setVisible(true);
            
            _normal_gou->setVisible(true);
            _normal_stars->setVisible(true);
            
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            
            _hardMenuBtn->setVisible(true);
            
            _normal_total->setVisible(true);
            _normal_total_node->setVisible(true);
        }
            break;
        case 7: {//游戏结束界面7（挑战通关）
            _roodaTitle->setVisible(true);
            _reward_node->setVisible(true);
            
            _normal_gou->setVisible(true);
            _normal_stars->setVisible(true);
            
            _normal_records_node->setVisible(true);
            _normal_stars_node->setVisible(true);
            
            _hardMenuBtn->setVisible(true);
            
            _normal_total->setVisible(true);
            _normal_total_node->setVisible(true);
        }
            break;
        default:
            break;
    }
    
    if (type >= 1 && type <= 3) {
        //todo: 分数 _normal_records_node,_normal_stars_node
        stringstream npc;
        npc << playerAchieve._killNpcCount << "/" << playerAchieve._totalNpcCount;
        createNodeValText(_normal_records_node, npc.str());
        
        stringstream record;
        record << playerAchieve._records;
        createNodeValText(_normal_stars_node, record.str());
    } else {
        //todo: 添加总分数 _normal_total_node
        //todo: 分数 _normal_records_node,_normal_stars_node
        stringstream totalRecord;
        totalRecord << playerAchieve._totalRecords;
        createNodeValText(_normal_total_node, totalRecord.str());
        
        stringstream record;
        record << GameData::Instance().playerData._hardLevelStarCount;
        createNodeValText(_normal_records_node, record.str());
        
        stringstream star;
        star << GameData::Instance().playerData._hardLevelKillCount;
        createNodeValText(_normal_stars_node, star.str());
    }
}
NS_KAI_END

