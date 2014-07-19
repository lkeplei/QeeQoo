//
//  StartupScene.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_StartupScene_h
#define ibubble_StartupScene_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;

NS_KAI_BEGIN

#pragma mark-
#pragma mark StartupScene
/**启动客户端之后加载资源和其他异步操作的界面*/
class StartupScene : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(StartupScene, create);
    virtual ~StartupScene();
    StartupScene();
    static cocos2d::CCScene* scene();
	static StartupScene* createWithCCB();
	void press_juqin();
	void press_jiangli();
	void press_tiaozhan();
	void press_setting();
	void press_help();
	void press_chengji();
	void loadImagesCallback(cocos2d::CCObject * obj);
	
	void handleMenuItemSelected(cocos2d::CCObject * obj);
	void handleMenuItemUnSelected(cocos2d::CCObject * obj);
	virtual void onEnter();
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
private:
	CCSprite * _logo;
	CCMenuItemImage * _pressJuqinBTN;
	CCMenuItemImage * _pressJiangliBTN;
	CCMenuItemImage * _pressTiaozhanBTN;
	CCMenuItemImage * _pressSettingBTN;
	CCMenuItemImage * _pressHelpBTN;
	CCMenuItemImage * _pressChengjiBTN;
	
	
	CCMenuItemImage * _pressJuqin;
	CCMenuItemImage * _pressJiangli;
	CCMenuItemImage * _pressTiaozhan;
	CCMenuItemImage * _pressSetting;
	CCMenuItemImage * _pressHelp;
	CCMenuItemImage * _pressChengji;
};



#pragma mark-
#pragma mark StartupSceneLayerLoader
class StartupSceneLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StartupSceneLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StartupScene);
};

NS_KAI_END

#endif
