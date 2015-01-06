//
//  LevelsSelectionScene.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#include "LevelsSelectionScene.h"
#include "GameController.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameConfig.h"
#include "GameData.h"
#include "UiTool.h"
NS_KAI_BEGIN


void CCTableViewCell4Level::Selected(){
	selected=true;
	if (defaultFrame) {
		defaultFrame->setVisible(false);
	}
	if (selectedFrame) {
		selectedFrame->setVisible(true);
	}
}


void CCTableViewCell4Level::Unselected(){
	selected=false;
	if (defaultFrame) {
		defaultFrame->setVisible(true);
	}
	
	if (selectedFrame) {
		selectedFrame->setVisible(false);
	}
}


void CCTableViewCell4Level::setDefaultFrame(CCNode * frame){
	if (defaultFrame) {
		this->removeChild(defaultFrame, true);
	}
	defaultFrame=frame;
	defaultFrame->setVisible(false);
	this->addChild(defaultFrame);
	defaultFrame->setPosition(ccp(0, 0));
	defaultFrame->setAnchorPoint(ccp(0, 0));
	_displaySize = frame->getContentSize();
}


void CCTableViewCell4Level::setSelectedFrame(CCNode * frame){
	if (selectedFrame) {
		this->removeChild(selectedFrame, true);
	}
	selectedFrame=frame;
	selectedFrame->setVisible(false);
	this->addChild(selectedFrame);
	selectedFrame->setPosition(ccp(0, 0));
	selectedFrame->setAnchorPoint(ccp(0, 0));
	_displaySize = frame->getContentSize();
}

CCNode * CCTableViewCell4Level::getDefaultFrame(){
	return defaultFrame;
}


CCNode * CCTableViewCell4Level::getSelectedFrame(){
	return selectedFrame;
}

bool CCTableViewCell4Level::isTouchInside(CCTouch* touch)
{
    CCPoint touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    CCRect bBox=boundingBox();
    return bBox.containsPoint(touchLocation);
}

void CCTableViewCell4Level2::setDefaultFrame(CCNode * frame){
	if (defaultFrame) {
		this->removeChild(defaultFrame, true);
	}
	defaultFrame=frame;
	defaultFrame->setVisible(false);
	this->addChild(defaultFrame);
	defaultFrame->setPosition(ccp(0, 0));
	_displaySize = frame->getContentSize();
}


void CCTableViewCell4Level2::setSelectedFrame(CCNode * frame){
	if (selectedFrame) {
		this->removeChild(selectedFrame, true);
	}
	selectedFrame=frame;
	selectedFrame->setVisible(false);
	this->addChild(selectedFrame);
	selectedFrame->setPosition(ccp(0, 0));
	_displaySize = frame->getContentSize();
}



LevelsSelectionScene::LevelsSelectionScene():CCLayer()
,_levelsTableViewRoot(NULL)
,_gouNodeRoot(NULL)
,_starNodeRoot(NULL)
,_rightNodeRoot(NULL)
,_leftNodeRoot(NULL)

,_levelsTableView(NULL)
,_levelsList(NULL)
,_currentCell(-1)
,_currentCellIndex(0)
,_newCell(NULL)
{
	_levelsList=new CCArray;
	_levelsList->initWithCapacity(10);
	setTouchEnabled( true );
    setAccelerometerEnabled( true );
}

LevelsSelectionScene::~LevelsSelectionScene()
{

	CC_SAFE_RELEASE_NULL(_rightNodeRoot);
	CC_SAFE_RELEASE_NULL(_leftNodeRoot);
	CC_SAFE_RELEASE_NULL(_starNodeRoot);
	CC_SAFE_RELEASE_NULL(_gouNodeRoot);
	
	CC_SAFE_RELEASE_NULL(_levelsTableViewRoot);
	CC_SAFE_RELEASE_NULL(_levelsList);
}

LevelsSelectionScene* LevelsSelectionScene::createWithCCB()
{
	CCNodeLoaderLibrary *ccNodeLoaderLibrary=CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
	ccNodeLoaderLibrary->registerCCNodeLoader("LevelsSelectionScene",LevelsSelectionSceneLayerLoader::loader());
	CCBReader * ccBReader=new CCBReader(ccNodeLoaderLibrary);
	LevelsSelectionScene * node =(LevelsSelectionScene*)ccBReader->readNodeGraphFromFile("levels_selection_layer.ccbi");
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

CCScene* LevelsSelectionScene::scene()
{
	CCScene *scene=CCScene::create();
	CCLayer * node =(CCLayer*)createWithCCB();
	if(node!=NULL){
		scene->addChild(node);
	}
	return scene;
}

void LevelsSelectionScene::press_back()
{
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_HOME,CCInteger::create(0));
}

void LevelsSelectionScene::press_left()
{
	CCLOG("LevelsSelectionScene::press_left");
	_currentCellIndex --;
	if (_currentCellIndex >= 0) {
		switchCell(_currentCellIndex,true,false);
	}
	else{
		_currentCellIndex = 0;
	}
}

void LevelsSelectionScene::press_right()
{
	CCLOG("LevelsSelectionScene::press_right");
	_currentCellIndex ++;
	if (_currentCellIndex < _levelsList->count()) {
		switchCell(_currentCellIndex,true,true);
	}
	else{
		_currentCellIndex = _levelsList->count()-1;
	}
}

bool isLevelOpen(const int32_t id) {
    bool res = true;
    
    if (id > 0) {
        std::vector<PlayerAchievement> list = GameData::Instance().findData(-1, id - 1);
        int list_size = list.size();
        if (list_size < 15) {
            res = false;
        } else if (list_size >= 15) {
            PlayerAchievement player = list[14];
            if (player._killNpcCount < player._pass_count) {
                res = false;
            }
        }
        list.clear();
    }
    
#ifdef KOpenAllLevels
    return true;
#else
    return res;
#endif
}

void LevelsSelectionScene::press_enter_battle(const int32_t id){
#ifndef KBattleTestModle
    if (isLevelOpen(id)) {
		GameController::sharedInstance()->switchSence(GameController::K_SCENE_SUB_LEVELS_SELECTION,CCInteger::create(id));
    }
#else
	GameController::sharedInstance()->switchSence(GameController::K_SCENE_SUB_LEVELS_SELECTION,CCInteger::create(id));
#endif
}

#pragma mark-
#pragma mark CCBSelectorResolver
SEL_MenuHandler LevelsSelectionScene::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName){
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_back",LevelsSelectionScene::press_back);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_left",LevelsSelectionScene::press_left);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_right",LevelsSelectionScene::press_right);
	return NULL;
	
}

cocos2d::extension::SEL_CCControlHandler LevelsSelectionScene::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName){
	//CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "press", LevelsSelectionScene::press);
	return NULL;
}

#pragma mark-
#pragma mark CCBMemberVariableAssigner
bool LevelsSelectionScene::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode){
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "list_node", CCNode *, this->_levelsTableViewRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gou_node", CCNode *, this->_gouNodeRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "star_node", CCNode *, this->_starNodeRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "right_menu_item", CCNode *, this->_rightNodeRoot);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "left_menu_item", CCNode *, this->_leftNodeRoot);
		
	return false;
}

#pragma mark-
#pragma mark CCBNodeLoaderListener
void LevelsSelectionScene::onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader){
	CCLOG("LevelsSelectionScene onNodeLoaded~") ;
	//获得大关卡列表
	CCDictionary * biglevelsDict = (CCDictionary *)GameConfig::sharedInstance()->getValue(KStrLevels);
	//GameModle * _sharedInstance=GameModle::sharedInstance();
	int index = kStoryZoneMaxId;
	for (int i = 0; i < index; i++ ) {
		std::stringstream key;
		key << i;
		CCObject * p = biglevelsDict->objectForKey(key.str());
		CCDictionary * dict = CCDictionary::create();
		dict->setObject(p, "info");
		dict->setObject(CCString::create(key.str().c_str()), "id");
		_levelsList->addObject(dict);
	}

	if (_levelsTableViewRoot) {
		switchCell(_currentCellIndex,false);
	}
	
	//显示总的分数情况
	std::vector<PlayerAchievement> list = GameData::Instance().findData(-1, 0);
	int pass_size = 0;
	int star_size = 0;
	//160,80
	for (std::vector<PlayerAchievement>::iterator iter = list.begin(); iter != list.end(); iter++) {
		pass_size ++;
		if ((*iter)._star_count > 0) {
			star_size ++;
		}
	}
    std::vector<PlayerAchievement> list1 = GameData::Instance().findData(-1, 1);
	//160,80
	for (std::vector<PlayerAchievement>::iterator iter = list1.begin(); iter != list1.end(); iter++) {
		pass_size ++;
		if ((*iter)._star_count > 0) {
			star_size ++;
		}
	}
    std::vector<PlayerAchievement> list2 = GameData::Instance().findData(-1, 2);
	//160,80
	for (std::vector<PlayerAchievement>::iterator iter = list2.begin(); iter != list2.end(); iter++) {
		pass_size ++;
		if ((*iter)._star_count > 0) {
			star_size ++;
		}
	}
	
	std::stringstream gouStr;
	gouStr << pass_size << "/" << 45;
	std::stringstream starStr;
	starStr << star_size << "/" << 45;
	
	list.clear();
    list1.clear();
    list2.clear();
	
	CCSize labelSize =  _gouNodeRoot->getContentSize();
	
	cocos2d::CCLabelAtlas * lableAtlas = NULL;

    lableAtlas = UiTool::createLabelAtlasWithBigNumber(gouStr.str());
	lableAtlas->setAnchorPoint(CCPoint(0, 0.5));
	lableAtlas->setPosition(CCPoint(0, (labelSize.height) * 0.5));
	_gouNodeRoot->addChild(lableAtlas);

	labelSize =  _starNodeRoot->getContentSize();
    lableAtlas = UiTool::createLabelAtlasWithBigNumber(starStr.str());
	lableAtlas->setAnchorPoint(CCPoint(0, 0.5));
	lableAtlas->setPosition(CCPoint(0, (labelSize.height) * 0.5));
	_starNodeRoot->addChild(lableAtlas);
}

void LevelsSelectionScene::doCallFunc_Remove(CCObject * obj){
	if (obj) {
		((CCNode * )obj)->removeFromParentAndCleanup(true);
	}
}

void LevelsSelectionScene::switchCell(const int index,const bool animation,const bool left){
	const float time  = 0.4f;
	const CCSize  contentSize = _levelsTableViewRoot->getContentSize();
	if (_newCell) {
		CCSize  pSize = _newCell->displaySize();
		if (animation) {
			CCMoveTo * mt = NULL;
			const CCPoint p  = _newCell->getPosition();
			CCPoint tp  = p;
			if (left) {
				tp.x = tp.x - pSize.width * 1.5;
			}
			else{
				tp.x = tp.x + pSize.width * 1.5;
			}
			CCLOG("a~~~move out from [%f,%f] to [%f,%f]",p.x,p.y,tp.x,tp.y);
			mt = CCMoveTo::create(time,tp);
			CCAction * seq = CCSequence::create(mt,CCCallFuncO::create(this,
																	   callfuncO_selector(LevelsSelectionScene::doCallFunc_Remove),
																	   _newCell),NULL);
			_newCell->runAction(seq);
		}
		else{
			_newCell->removeFromParentAndCleanup(true);
			_newCell = NULL;
		}
	}
	
	CCTableViewCell4Level2 * cell= new CCTableViewCell4Level2;
	cell->autorelease();
	CCDictionary * dict = (CCDictionary *)_levelsList->objectAtIndex(index);
	CCDictionary * info = (CCDictionary *)dict->objectForKey("info");
	cell->setCellData(dict);
	CCString * defaultFrame = (CCString *)info->objectForKey(KStrDefaultFrame);
	CCString * selectedFrame = (CCString *)info->objectForKey(KStrSelectedFrame);
	if (!selectedFrame) {
		selectedFrame = defaultFrame;
	}
	CCSprite * defaultFrameNode = CCSprite::createWithSpriteFrameName(defaultFrame->getCString());
	CCSprite * selectedFrameNode = CCSprite::createWithSpriteFrameName(selectedFrame->getCString());
	cell->setSelectedFrame(selectedFrameNode);
	cell->setDefaultFrame(defaultFrameNode);
	defaultFrameNode->setVisible(true);
    
	CCPoint p = CCPoint(contentSize.width * 0.5 , contentSize.height * 0.5);
	_levelsTableViewRoot->addChild(cell);
	_newCell = cell;
	
	//显示当前关卡的分数情况
	std::vector<PlayerAchievement> list = GameData::Instance().findData(-1,index);
	int star_size = 0;
	int pass_size = 0;
	//160,80
	for (std::vector<PlayerAchievement>::iterator iter =  list.begin(); iter != list.end(); iter++) {
		pass_size ++;
		if ((*iter)._star_count > 0) {
			star_size ++;
		}
	}
	
	std::stringstream gouStr;
	gouStr << pass_size << "/" << 15;
	std::stringstream starStr;
	starStr << star_size << "/" << 15;
	
	list.clear();
	
	const CCSize labelSize(160,80);
	
    CCLabelAtlas *gouLabel = UiTool::createLabelAtlasWithBigNumber(gouStr.str());
	gouLabel->setAnchorPoint(CCPoint(0.5, 0.5));
	gouLabel->setContentSize(labelSize);
	gouLabel->setPosition(CCPoint(-labelSize.width * 0.4, - contentSize.height * 0.13));
	cell->addChild(gouLabel);
	
    CCLabelAtlas *starLabel = UiTool::createLabelAtlasWithBigNumber(starStr.str());
	starLabel->setAnchorPoint(CCPoint(0.5, 0.5));
	starLabel->setContentSize(labelSize);
	starLabel->setPosition(CCPoint(labelSize.width * 0.95 , - contentSize.height * 0.13));
	cell->addChild(starLabel);
	
    // add lock
    CCString * idString = (CCString *)dict->objectForKey("id");
    if (!isLevelOpen(idString->intValue())) {
        CCSprite *lock = CCSprite::create("level_lock.png");
        lock->setPositionX(lock->getPosition().x - 10);
        lock->setPositionY(lock->getPosition().y - 20);
        cell->addChild(lock);
    }
    
	if (animation) {
		
		CCPoint fp  = p;
		if (left) {
			fp.x = p.x + cell->displaySize().width * 2;
		}
		else{
			fp.x = p.x - cell->displaySize().width * 2;
		}
		
		_newCell->setPosition(fp);
		
		CCMoveTo *mt = CCMoveTo::create(time * 2,p);
		_newCell->runAction(mt);
	}
	else{
		_newCell->setPosition(p);
	}
	
	if (index == 0 && _leftNodeRoot) {
		_leftNodeRoot->setVisible(false);
	}
	else{
		_leftNodeRoot->setVisible(true);
	}
	
	if (index == _levelsList->count() -1 && _rightNodeRoot) {
		_rightNodeRoot->setVisible(false);
	}
	else{
		_rightNodeRoot->setVisible(true);
	}
}

void LevelsSelectionScene::onEnter(){
	CCLayer::onEnter();
	if (_levelsTableView) {
		_levelsTableView->reloadData();
	}
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
    
    //add ad
    GameUtilities::resetAd(true);
}

void LevelsSelectionScene::onExit(){
	CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
    
    //hide ad
    GameUtilities::removeAd();
}

#pragma mark-
#pragma mark CCTableViewDelegate
void LevelsSelectionScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLOG("LevelsSelectionScene tableCellTouched: idx=%d",cell->getIdx());
	
	if (_levelsTableView == table) {
		
		if ( _currentCell == cell->getIdx()) {
			
			CCTableViewCell4Level * tmpcell= (CCTableViewCell4Level *) cell;
			if (!tmpcell->isSelected()) {
				tmpcell->Selected();
			}
			else {
				//处理点击事件
				CCDictionary * dict=(CCDictionary *)tmpcell->getCellData();
				CCString * idString=(CCString *)dict->objectForKey("id");
				press_enter_battle(idString->intValue());
			}
		}
		else {
			
			CCTableViewCell4Level * oldcell= (CCTableViewCell4Level *)table->cellAtIndex(_currentCell);
			if (oldcell) {
				oldcell->Unselected();
			}
			
			CCTableViewCell4Level * tmpcell= (CCTableViewCell4Level *) cell;
			tmpcell->Selected();
			_currentCell=cell->getIdx();
		}
	}
	
	
}

#pragma mark-
#pragma mark CCTableViewDataSource
CCSize LevelsSelectionScene::cellSizeForTable(CCTableView *table)
{
	return CCSize(600,400);
}

CCTableViewCell* LevelsSelectionScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell4Level * cell= new CCTableViewCell4Level;
	cell->autorelease();
	if (table == _levelsTableView){
		
		CCDictionary * dict=(CCDictionary *)_levelsList->objectAtIndex(idx);
		CCDictionary * info=(CCDictionary *)dict->objectForKey("info");
		
		cell->setCellData(dict);
		
		CCString * defaultFrame = (CCString *)info->objectForKey(KStrDefaultFrame);
		CCString * selectedFrame = (CCString *)info->objectForKey(KStrSelectedFrame);
		if (!selectedFrame) {
			selectedFrame=defaultFrame;
		}
		
		CCSprite * defaultFrameNode = CCSprite::createWithSpriteFrameName(defaultFrame->getCString());
		CCSprite * selectedFrameNode = CCSprite::createWithSpriteFrameName(selectedFrame->getCString());
		cell->setSelectedFrame(selectedFrameNode);
		cell->setDefaultFrame(defaultFrameNode);
		defaultFrameNode->setVisible(true);
	}
	return cell;
}

unsigned int LevelsSelectionScene::numberOfCellsInTableView(CCTableView *table)
{
	int count =0;
	if (table == _levelsTableView){
		count = _levelsList ? _levelsList->count() : 0;
	}
	return count;
}

CCRect LevelsSelectionScene::getNewCellBox() {
    CCRect bBox = _levelsTableViewRoot->boundingBox();
    const CCSize & size = bBox.size;
    const float scaleX = 0.6;
    const float scaleY = 0.8;
    
    float offset_w = size.width * (1- scaleX) * 0.5;
    float offset_h = size.height * (1- scaleY) * 0.5;
    
    bBox.origin.x = bBox.origin.x + offset_w;
    bBox.origin.y = bBox.origin.y + offset_h;
    
    bBox.size.width = bBox.size.width * scaleX;
    bBox.size.height = bBox.size.height * scaleY;
    
    return bBox;
}

//触摸事件
bool LevelsSelectionScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint touchLocation = pTouch->getLocation(); // Get the touch position
    
	if (_newCell) {
        movePreX = touchLocation.x;
        originalX = touchLocation.x;
        
		touchLocation = _levelsTableViewRoot->getParent()->convertToNodeSpace(touchLocation);

		if (getNewCellBox().containsPoint(touchLocation)) {
            CCDictionary * dict=(CCDictionary *)_newCell->getCellData();
            CCString * idString=(CCString *)dict->objectForKey("id");
            if (!_newCell->isSelected() && isLevelOpen(idString->intValue())) {
                _newCell->Selected();
            }
			return true;
		}
	}
    
    CCRect rect = CCRectMake(0, 0, this->getContentSize().width * 0.2, this->getContentSize().height * 0.2);
    if (rect.containsPoint(touchLocation)) {
        return false;
    }
    
	return true;
}

void LevelsSelectionScene::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    if (_newCell) {
        CCPoint touchLocation = pTouch->getLocation();
        
        float offX = touchLocation.x - movePreX;
        _newCell->setPosition(CCPointMake(_newCell->getPosition().x + offX, _newCell->getPosition().y));
        
        movePreX = touchLocation.x;
        
		if (getNewCellBox().containsPoint(touchLocation)) {
            CCDictionary * dict=(CCDictionary *)_newCell->getCellData();
            CCString * idString=(CCString *)dict->objectForKey("id");
			if (!_newCell->isSelected() && isLevelOpen(idString->intValue())) {
				_newCell->Selected();
			}
		} else {
            if (_newCell->isSelected()) {
				_newCell->Unselected();
			}
        }
	}
}

void LevelsSelectionScene::ccTouchEnded(CCTouch *pTouch, CCEvent* event){
	if (_newCell) {
        if (abs(movePreX - originalX) > 10) {
            bool moveto = false;
            if (movePreX < originalX) {
                if (_currentCellIndex < 2) {
                    press_right();
                } else {
                    moveto = true;
                }
            } else {
                if (_currentCellIndex > 0) {
                    press_left();
                } else {
                    moveto = true;
                }
            }
            
            if (moveto) {
                CCMoveTo *mt = CCMoveTo::create(0.2,
                                                CCPointMake(_newCell->getPosition().x + (originalX - movePreX), _newCell->getPosition().y));
                _newCell->runAction(mt);
            }
            
            _newCell->Unselected();
        } else {
            CCPoint touchLocation = pTouch->getLocation(); // Get the touch position
            touchLocation = _levelsTableViewRoot->getParent()->convertToNodeSpace(touchLocation);
            CCRect bBox = _levelsTableViewRoot->boundingBox();
            if (bBox.containsPoint(touchLocation)) {
                //处理点击事件
                CCDictionary * dict=(CCDictionary *)_newCell->getCellData();
                CCString * idString=(CCString *)dict->objectForKey("id");
                press_enter_battle(idString->intValue());
            }
        }
	}
}
NS_KAI_END

