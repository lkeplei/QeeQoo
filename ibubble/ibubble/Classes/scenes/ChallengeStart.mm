//
//  ChallengeStart.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-24.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ChallengeStart.h"

#import <Foundation/Foundation.h>

typedef void(^KAlertCallbackBlock)(int);

@interface KenAlertView : NSObject<UIAlertViewDelegate>

- (void)showAlert:(NSString *)title;

@property (nonatomic, assign)kai::game::ChallengeStart *challengeStart;
@property (nonatomic, assign)KAlertCallbackBlock callBackBlock;

@end

@implementation KenAlertView

- (void)showAlert:(NSString *)title {
    [[[UIAlertView alloc] initWithTitle:nil
                                message:title
                               delegate:self
                      cancelButtonTitle:KenLocal(@"app_cancel")
                      otherButtonTitles:KenLocal(@"app_confirm"), nil] show];
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if (self.callBackBlock) {
        self.callBackBlock(buttonIndex);
    }
}

@end




#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "UiTool.h"
#include "GameConfig.h"

#include "GameUtilities.h"

NS_KAI_BEGIN
ChallengeStart::ChallengeStart():CCLayer()
, _continue_menu(NULL)
{

}

ChallengeStart::~ChallengeStart()
{
    CC_SAFE_RELEASE_NULL(_continue_menu);
}

ChallengeStart* ChallengeStart::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("ChallengeStart",ChallengeStartLayerLoader::loader());
	CCBReader * ccBReader = new CCBReader(ccNodeLoaderLibrary);
	ChallengeStart * node = (ChallengeStart*)ccBReader->readNodeGraphFromFile("ChallengeStart.ccbi");
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

CCScene* ChallengeStart::scene(){
	CCScene *scene = CCScene::create();
	ChallengeStart * node = createWithCCB();
	if(node != NULL){
		scene->addChild(node);
	}
	return scene;
}

void ChallengeStart::press_continue(){
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeGameStart,CCInteger::create(0));
}

void ChallengeStart::press_new(){
    GameUtilities::saveGoonGame();
    
    if (GameUtilities::getGoonGame()) {
        KenAlertView *alert = [[KenAlertView alloc] init];
        [alert showAlert:KenLocal(@"new_game_ask")];
        alert.callBackBlock= ^(int index){
            if (index == 0) {
                
            } else {
                GameModle::sharedInstance()->resetSkillInfo();
                GameModle::sharedInstance()->setCurrentHardLevelId(0);
                GameUtilities::saveLevelId(GameModle::sharedInstance()->currentHardLevelId(), GameData::Instance().playerData);
                
                GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeHelpInLevel);
            }
        };
    } else {
        GameController::sharedInstance()->switchSence(GameController::K_SCENE_ChallengeHelpInLevel);
    }
}

void ChallengeStart::press_back(){
    GameController::sharedInstance()->popSence();
    GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,NULL);
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler ChallengeStart::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_continue",ChallengeStart::press_continue);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_new",ChallengeStart::press_new);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",ChallengeStart::press_back);
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler ChallengeStart::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool ChallengeStart::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "continue_menu_node", CCMenuItemImage *, _continue_menu);
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void ChallengeStart::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoade){
    if (!GameUtilities::getGoonGame()) {
        _continue_menu->setEnabled(false);
        _continue_menu->selected();
    }
}
NS_KAI_END

