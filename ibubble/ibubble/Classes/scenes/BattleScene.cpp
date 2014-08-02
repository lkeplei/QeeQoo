//
//  BattleScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-11.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//
#include "BattleScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConfig.h"
#include "UiTool.h"
#include "GameConstant.h"
USING_NS_CC;

#include "GameUtilities.h"

NS_KAI_BEGIN
BattleBgLayer::BattleBgLayer():BattleBaseLayer(),_animationManager(NULL){
	setTouchEnabled( true );
    setAccelerometerEnabled( true );
}

BattleBgLayer::~BattleBgLayer(){
	CC_SAFE_RELEASE_NULL(_animationManager);
}

void BattleBgLayer::updateLayer(){

}

void BattleBgLayer::load(const cocos2d::CCDictionary * dict){

}

SEL_MenuHandler BattleBgLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

cocos2d::extension::SEL_CCControlHandler BattleBgLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

bool BattleBgLayer::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	return false;
}

void BattleBgLayer::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("BattleBgLayer onNodeLoaded~") ;
}

#pragma mark-
#pragma mark CCBAnimationManagerDelegate
void BattleBgLayer::completedAnimationSequenceNamed(const char *name){
	CCLOG("BattleBgLayer completedAnimationSequenceNamed:%s",name) ;
}



BattleGameObjectLayer::BattleGameObjectLayer():BattleBaseLayer(){
	setTouchEnabled( true );
    setAccelerometerEnabled( true );
	_configInstance=GameConfig::sharedInstance();
	_modleInstance=GameModle::sharedInstance();
}

BattleGameObjectLayer::~BattleGameObjectLayer(){
	
}

void BattleGameObjectLayer::updateLayer(){
	
}

void BattleGameObjectLayer::load(const cocos2d::CCDictionary * dict){
	
}


void BattleGameObjectLayer::draw()
{
    BattleBaseLayer::draw();
	//如果是调试模式，那么现实调试信息
	if (_configInstance->isDebuging()) {
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
		kmGLPushMatrix();
		_modleInstance->box2dWorld()->DrawDebugData();
		kmGLPopMatrix();
	}
}



BattleControllerLayer::BattleControllerLayer():BattleBaseLayer(){
	setTouchEnabled( true );
    setAccelerometerEnabled( true );
	_cur_scores_node = NULL;
	_cur_menu_node = NULL;
	_pause_button = NULL;
	_back_button = NULL;
	
	_skill1Button = NULL;
	_skill2Button = NULL;
	_skill3Button = NULL;
	_skill4Button = NULL;
	_skill5Button = NULL;
	_skill6Button = NULL;
    _ChallengeRoot = NULL;
	_ChallengeMenu = NULL;
    
    _skill1text = NULL;
	_skill2text = NULL;
	_skill3text = NULL;
	_skill4text = NULL;
	_skill5text = NULL;
	_skill6text = NULL;

}

BattleControllerLayer::~BattleControllerLayer(){
	CC_SAFE_RELEASE_NULL(_cur_scores_node);
	CC_SAFE_RELEASE_NULL(_cur_menu_node);
	CC_SAFE_RELEASE_NULL(_pause_button);
	CC_SAFE_RELEASE_NULL(_back_button);	
    
	CC_SAFE_RELEASE_NULL(_skill1Button);
	CC_SAFE_RELEASE_NULL(_skill2Button);
	CC_SAFE_RELEASE_NULL(_skill3Button);
	CC_SAFE_RELEASE_NULL(_skill4Button);
	CC_SAFE_RELEASE_NULL(_skill5Button);
	CC_SAFE_RELEASE_NULL(_skill6Button);
    
    CC_SAFE_RELEASE_NULL(_skill1text);
	CC_SAFE_RELEASE_NULL(_skill2text);
	CC_SAFE_RELEASE_NULL(_skill3text);
	CC_SAFE_RELEASE_NULL(_skill4text);
	CC_SAFE_RELEASE_NULL(_skill5text);
	CC_SAFE_RELEASE_NULL(_skill6text);
    
    CC_SAFE_RELEASE_NULL(_ChallengeMenu);
	CC_SAFE_RELEASE_NULL(_ChallengeRoot);
}

void BattleControllerLayer::updateLayer(){
	if (_cur_scores_node) {
		PlayerAchievement & p = GameModle::sharedInstance()->playerAchievement();
		stringstream liveNpcCount;
		liveNpcCount << p._killNpcCount << "/" << p._totalNpcCount;
		
		_cur_scores_node->removeAllChildrenWithCleanup(true);
		
		CCSize labelSize =  _cur_scores_node->getContentSize();
		cocos2d::CCLabelBMFont * label = UiTool::createLabelBMFont(liveNpcCount.str(),UiTool::kFontBig,ccc3(255,255,255));
		CCSize cellSize =  label->getContentSize();
		label->setAnchorPoint(CCPoint(0.5, 0.5));
		label->setPosition(CCPoint(labelSize.width* 0.5 , labelSize.height* 0.5));
		_cur_scores_node->addChild(label);
	}

}

void BattleControllerLayer::load(const cocos2d::CCDictionary * dict){
	
}

void BattleControllerLayer::press_back()
{
	GameController::sharedInstance()->pauseBattle();
	if (GameModle::sharedInstance()->currentBigLevelId() == kStoryZoneMaxId){
		GameController::sharedInstance()->switchSence(GameController::K_SCENE_ACHIEVEMENT);
	}
	else{
		GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME);
	}

}

void BattleControllerLayer::press_pause()
{
	//GameController::sharedInstance()->pauseBattle();
	showMenu(true);
}
void BattleControllerLayer::showMenu(bool animation){
	if(animation){
		_cur_menu_node->stopActionByTag(88888);
		CCMoveTo * m = CCMoveTo::create(0.5, _startPos);
		m->setTag(88888);
		_cur_menu_node->runAction(m);
	}
	else{
		_cur_menu_node->setPositionX(_startPos.x);
	}
	
}

void BattleControllerLayer::hideMenu(bool animation){
	if(animation){
		_cur_menu_node->stopActionByTag(99999);
		CCPoint p = CCPoint(_startPos.x - _cur_menu_node->getContentSize().width,_startPos.y);
		CCMoveTo * m = CCMoveTo::create(0.5, p);
		m->setTag(99999);
		_cur_menu_node->runAction(m);
	}
	else{
		_cur_menu_node->setPositionX(_startPos.x - _cur_menu_node->getContentSize().width);
	}
}

void BattleControllerLayer::press_restart()
{
	int32_t currentLevelId = GameModle::sharedInstance()->currentLevelId();
	GameController::sharedInstance()->pauseBattle();
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE,CCInteger::create(currentLevelId));
}

void BattleControllerLayer::press_play(){
	//GameController::sharedInstance()->resumeBattle();
	hideMenu(true);
}

void BattleControllerLayer::press_home(){
	
	GameController::sharedInstance()->pauseBattle();
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME);
}


void BattleControllerLayer::press_condition(){
	
	GameController::sharedInstance()->pauseBattle();
	int zone = GameModle::sharedInstance()->currentBigLevelId();
	int level = GameModle::sharedInstance()->currentLevelId();
	hideMenu();
	GameController::sharedInstance()->pushSence(GameController::K_SCENE_BATTLE_PRE,
												CCInteger::create(level),
												CCInteger::create(zone));
}

void BattleControllerLayer::press_setting(){
	
	GameController::sharedInstance()->pauseBattle();
	int zone = GameModle::sharedInstance()->currentBigLevelId();
	int level = GameModle::sharedInstance()->currentLevelId();
	hideMenu();
	GameController::sharedInstance()->pushSence(GameController::K_SCENE_SETTING_IN_LEVEL,
												CCInteger::create(level),
												CCInteger::create(zone));
}

void BattleControllerLayer::press_help(){
	GameController::sharedInstance()->pauseBattle();
	int zone = GameModle::sharedInstance()->currentBigLevelId();
	int level = GameModle::sharedInstance()->currentLevelId();
	hideMenu();
	GameController::sharedInstance()->pushSence(GameController::K_SCENE_HELP_IN_LEVEL,
												CCInteger::create(level),
												CCInteger::create(zone));
}

void BattleControllerLayer::press_skill1(){
//    SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
//    skillInfo.skillLife-- ;
//    GameModle::sharedInstance()->saveSkillInfo();
//    skillUpdated(NULL);
}

void BattleControllerLayer::press_skill2(){
	PlayerAchievement & achievement = GameModle::sharedInstance()->playerAchievement();
	if (achievement._killNpcCount == 0) {
        SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
        skillInfo.skill_skip-- ;
        GameModle::sharedInstance()->saveSkillInfo();
        skillUpdated(NULL);
        
		int32_t currentLevelId = GameModle::sharedInstance()->currentLevelId();
		GameController::sharedInstance()->pauseBattle();
        if (GameModle::sharedInstance()->getBattleMode() != K_HARD_PVE_BATTLE) {
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE,CCInteger::create(currentLevelId));
        }else{
            GameController::sharedInstance()->switchSence(GameController::K_SCENE_BATTLE_CHALLENGE,
                                                          CCInteger::create(GameUtilities::getRandLevel(false)));
        }
	}
}

void BattleControllerLayer::press_skill3(){
	PlayerAchievement & achievement = GameModle::sharedInstance()->playerAchievement();
	if (achievement._killNpcCount == 0) {
        SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
        skillInfo.skill_weak-- ;
        GameModle::sharedInstance()->saveSkillInfo();
        skillUpdated(NULL);
        
		GameController::sharedInstance()->pauseBattle();
		GameModle::sharedInstance()->weakNPCs();
		GameController::sharedInstance()->resumeBattle();
	}
}

void BattleControllerLayer::press_skill4(){
	GameModle *model = GameModle::sharedInstance();
	if (model->getBattleTouchTimes() < model->getBattleTouchMaxTimes()) {
        SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
        skillInfo.skill_s_touch-- ;
        GameModle::sharedInstance()->saveSkillInfo();
        skillUpdated(NULL);
        
		GameController::sharedInstance()->pauseBattle();
		GameModle::sharedInstance()->setTouchEnableTypes(KNPCTypeStrengthId);
		GameController::sharedInstance()->resumeBattle();
	}
}

void BattleControllerLayer::press_skill5(){
    GameModle *model = GameModle::sharedInstance();
	if (model->getBattleTouchTimes() < model->getBattleTouchMaxTimes()) {
        SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
        skillInfo.skill_large_touch-- ;
        model->saveSkillInfo();
        skillUpdated(NULL);
        
		GameController::sharedInstance()->pauseBattle();
		model->setTouchEnableTypes(KNPCTypeSuperId);
		GameController::sharedInstance()->resumeBattle();
	}
}

void BattleControllerLayer::press_skill6() {
    if(GameModle::sharedInstance()->getBattleTouchMaxTimes() < 2) {
        SkillInfo & skillInfo = GameModle::sharedInstance()->getSkillInfo();
        skillInfo.skill_multi_touch-- ;
        GameModle::sharedInstance()->saveSkillInfo();
        skillUpdated(NULL);
        
        GameController::sharedInstance()->pauseBattle();
        GameModle::sharedInstance()->setBattleTouchMaxTimes(2);
        GameController::sharedInstance()->resumeBattle();
    }
}

#pragma mark-
#pragma mark BattleControllerLayer CCBSelectorResolver
SEL_MenuHandler BattleControllerLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",BattleControllerLayer::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_pause",BattleControllerLayer::press_pause);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_restart",BattleControllerLayer::press_restart);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_play",BattleControllerLayer::press_play);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_home",BattleControllerLayer::press_home);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_condition",BattleControllerLayer::press_condition);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_setting",BattleControllerLayer::press_setting);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_help",BattleControllerLayer::press_help);
	
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill1",BattleControllerLayer::press_skill1);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill2",BattleControllerLayer::press_skill2);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill3",BattleControllerLayer::press_skill3);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill4",BattleControllerLayer::press_skill4);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill5",BattleControllerLayer::press_skill5);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_skill6",BattleControllerLayer::press_skill6);

	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler BattleControllerLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	return NULL;
}

#pragma mark-
#pragma mark BattleControllerLayer CCBMemberVariableAssigner
bool BattleControllerLayer::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "cur_scores_node", CCNode *, this->_cur_scores_node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "menu_borad", CCLayer *, this->_cur_menu_node);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pause_button", CCMenuItemImage *, this->_pause_button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "back_button", CCMenuItemImage *, this->_back_button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill1_button", CCMenuItemImage *, this->_skill1Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill2_button", CCMenuItemImage *, this->_skill2Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill3_button", CCMenuItemImage *, this->_skill3Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill4_button", CCMenuItemImage *, this->_skill4Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill5_button", CCMenuItemImage *, this->_skill5Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill6_button", CCMenuItemImage *, this->_skill6Button);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ChallengeRoot", CCNode *, this->_ChallengeRoot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "ChallengeMenu", CCMenu *, this->_ChallengeMenu);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill1_text", CCNode *, this->_skill1text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill2_text", CCNode *, this->_skill2text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill3_text", CCNode *, this->_skill3text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill4_text", CCNode *, this->_skill4text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill5_text", CCNode *, this->_skill5text);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "skill6_text", CCNode *, this->_skill6text);
    
    
	return false;
}

#pragma mark-
#pragma mark BattleControllerLayer CCBNodeLoaderListener
void BattleControllerLayer::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("BattleControllerLayer onNodeLoaded~") ;
	updateLayer();
	_startPos = _cur_menu_node->getPosition();
	this->hideMenu();
	
	if (GameModle::sharedInstance()->currentBigLevelId() >= kStoryZoneMaxId) {
		_pause_button->setVisible(false);
		_pause_button->setEnabled(false);
	}
	else{
		_back_button->setVisible(false);
		_back_button->setEnabled(false);
	}
	
    if (GameModle::sharedInstance()->getBattleMode() != K_HARD_PVE_BATTLE) {
        _ChallengeMenu->setVisible(false);
        _ChallengeRoot->setVisible(false);
    }else{
        skillUpdated(NULL);
    }
}

void createSkillText(CCNode * container,int32_t val){
    if (container) {
        container->removeAllChildrenWithCleanup(true);
        std::stringstream  skillNumber;
        skillNumber << val;
        CCSize labelSize =  container->getContentSize();
        cocos2d::CCLabelBMFont * label = UiTool::createLabelBMFont(skillNumber.str(),
                                                                   UiTool::kFontSmall,
                                                                   ccc3(255,255,255));
        CCSize cellSize =  label->getContentSize();
        label->setAnchorPoint(CCPoint(0.5, 0.5));
        label->setPosition(CCPoint(labelSize.width * 0.5 , labelSize.height * 0.5));
        container->addChild(label);
    }
}

void BattleControllerLayer::skillUpdated(CCObject * params){
    const SkillInfo & info = GameModle::sharedInstance()->getSkillInfo();
    
    createSkillText(this->_skill1text, info.skillLife);
    createSkillText(this->_skill2text, info.skill_skip);
    createSkillText(this->_skill3text, info.skill_weak);
    createSkillText(this->_skill4text, info.skill_s_touch);
    createSkillText(this->_skill5text, info.skill_large_touch);
    createSkillText(this->_skill6text, info.skill_multi_touch);
    
    _skill1Button->setEnabled(info.skillLife > 0);
    _skill2Button->setEnabled(info.skill_skip > 0);
    _skill3Button->setEnabled(info.skill_weak > 0);
    _skill4Button->setEnabled(info.skill_s_touch > 0);
    _skill5Button->setEnabled(info.skill_large_touch > 0);
    _skill6Button->setEnabled(info.skill_multi_touch > 0);
}

BattleScene::BattleScene()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
	_layers = CCArray::createWithCapacity(5);//默认估计最多5层
	_layers->retain();
	_configInstance =	GameConfig::sharedInstance();
	_modleInstance =  GameModle::sharedInstance();
}

BattleScene::~BattleScene()
{
	CC_SAFE_RELEASE_NULL(_layers);
}

void BattleScene::load()
{
	//TODO:此处需要修改成配置文件
	CCDictionary * dict=CCDictionary::create();
	
	//创建layer
	int zOrder=0;
	BattleBaseLayer * layer=NULL;
	
	//加载游戏背景的layer
	
	
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("BattleBgLayer",BattleBgLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	layer =(BattleBgLayer*)ccBReader->readNodeGraphFromFile("battle_bg_layer.ccbi");
	if (layer) {
		_layers->addObject(layer);
		layer->load(dict);
		layer->setTag(KBattleNodeTageBgLayer);
		this->addChild(layer, zOrder++);
	}
	CC_SAFE_RELEASE_NULL(ccBReader);

	
	//加载游戏战斗的layer
	layer=new BattleGameObjectLayer;
	layer->setTag(KBattleNodeTagGameObjectLayer);
	layer->load(dict);
	_layers->addObject(layer);
	this->addChild(layer, zOrder++);
	CC_SAFE_RELEASE_NULL(layer);

	
	//加载控制layer
	ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("BattleControllerLayer",BattleControllerLayerLoader::loader());
	ccBReader=new CCBReader(ccNodeLoaderLibrary);
	layer =(BattleControllerLayer*)ccBReader->readNodeGraphFromFile("battle_controller_layer.ccbi");
	if (layer) {
		_layers->addObject(layer);
		layer->setTag(KBattleNodeTagControllerLayer);
		layer->load(dict);
		this->addChild(layer, zOrder++);
	}
	CC_SAFE_RELEASE_NULL(ccBReader);


}

CCScene* BattleScene::scene()
{
    CCScene *scene = CCScene::create();
    BattleScene* layer = new BattleScene();
	layer->load();
    scene->addChild(layer);
    layer->release();
    return scene;
}

void BattleScene::modleUpdated(){
	CCObject * layerObject=NULL;
	CCARRAY_FOREACH(_layers, layerObject){
		BattleBaseLayer * layer=(BattleBaseLayer *)layerObject;
		layer->updateLayer();
	}
}

void BattleScene::onEnter(){
	CCLayer::onEnter();
	GameController * gController = GameController::sharedInstance();
	if(!gController->isPushSenceFromBattle()){
		gController->onEnterScene(GameController::K_SCENE_BATTLE,this);
	}
	
}

void BattleScene::onExit(){
	CCLayer::onExit();
	GameController * gController = GameController::sharedInstance();
	gController->setIsPushSenceFromBattle(gController->isPushSence());
	if(!gController->isPushSenceFromBattle()){
		gController->onExitScene(GameController::K_SCENE_BATTLE,this);
	}
}

void BattleScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCLayer::ccTouchesEnded(touches,event);
	GameController::sharedInstance()->onTouchesEnded(touches,event);
}


NS_KAI_END