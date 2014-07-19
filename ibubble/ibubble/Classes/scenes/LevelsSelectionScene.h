//
//  LevelsSelectionScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_LevelsSelectionScene_h
#define ibubble_LevelsSelectionScene_h
#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark CCTableViewCell4Level
class CCTableViewCell4Level:public CCTableViewCell{
public:
	CC_SYNTHESIZE_RETAIN(CCDictionary *, m_cellData, CellData);	
public:
	CCTableViewCell4Level():CCTableViewCell(),m_cellData(NULL),selected(false),defaultFrame(NULL),selectedFrame(NULL){}
	~CCTableViewCell4Level(){CC_SAFE_RELEASE_NULL(m_cellData);}
	bool isSelected(){return selected;}
	void Selected();
	void Unselected();
	virtual void	setDefaultFrame(CCNode * frame);
	virtual void    setSelectedFrame(CCNode * frame);
	CCNode * 	getDefaultFrame();
	CCNode * 	getSelectedFrame();
	const CCSize & displaySize(){return _displaySize;}
	bool isTouchInside(CCTouch* touch);
protected:
	CCNode * 	defaultFrame;
	CCNode * 	selectedFrame;
	bool 		selected;
	CCSize  	_displaySize;
};


class CCTableViewCell4Level2:public CCTableViewCell4Level{
public:
	virtual void	setDefaultFrame(CCNode * frame);
	virtual void    setSelectedFrame(CCNode * frame);
};

#pragma mark-
#pragma mark LevelsSelectionScene
class LevelsSelectionScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener
,public cocos2d::extension::CCTableViewDelegate
,public cocos2d::extension::CCTableViewDataSource
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LevelsSelectionScene, create);
    virtual ~LevelsSelectionScene();
    LevelsSelectionScene();
    static cocos2d::CCScene* scene();
	static LevelsSelectionScene* createWithCCB();
	void press_back();
	void press_left();
	void press_right();
	void press_enter_battle(const int32_t id);
	void loadImagesCallback(cocos2d::CCObject * obj);
	virtual void onEnter();
    virtual void onExit();
	void doCallFunc_Remove(CCObject * obj);
	void switchCell(const int index,const bool animation = false,const bool left = false);
public:
#pragma mark-
#pragma mark CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual cocos2d::extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
#pragma mark-
#pragma mark CCBMemberVariableAssigner
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
#pragma mark-
#pragma mark CCBNodeLoaderListener
	virtual void onNodeLoaded(CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	
	
public:
#pragma mark-
#pragma mark CCTableViewDelegate
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view){};
    virtual void scrollViewDidZoom(CCScrollView* view) {}
public:
#pragma mark-
#pragma mark CCTableViewDataSource
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
public:
	//触摸事件
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *touch, CCEvent* event);
private:
	CCNode   * _levelsTableViewRoot;
	CCNode   * _gouNodeRoot;
	CCNode   * _starNodeRoot;
	CCNode   * _rightNodeRoot;
	CCNode   * _leftNodeRoot;

	CCTableView * _levelsTableView;
	CCArray * _levelsList;
	 unsigned int _currentCell;
	int _currentCellIndex;
	CCTableViewCell4Level2 * _newCell;
};



#pragma mark-
#pragma mark LevelsSelectionSceneLayerLoader
class LevelsSelectionSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LevelsSelectionSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(LevelsSelectionScene);
};

NS_KAI_END

#endif

