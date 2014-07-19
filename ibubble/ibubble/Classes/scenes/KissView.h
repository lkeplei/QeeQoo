//
//  KissView.h
//  ibubble
//
//  Created by Ryan Yuan on 12-12-04.
//  Copyright (c) 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_KissView_h
#define ibubble_KissView_h

#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class CCSWFNodeSequence;
NS_KAI_BEGIN

#pragma mark-
#pragma mark KissView

class KissView : public cocos2d::CCLayer
,public cocos2d::extension::CCBSelectorResolver 
,public cocos2d::extension::CCBMemberVariableAssigner
,public cocos2d::extension::CCNodeLoaderListener{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(KissView, create);
    virtual ~KissView();
    KissView();
    static cocos2d::CCScene* scene();
	static KissView* createWithCCB();
	void press_back();
    void press_boy_kiss();
    void press_girl_kiss();
	virtual void onEnter();
    virtual void onExit();
private:
    void playSWF(const std::string & file);
    void handleSwfFinished(cocos2d::CCObject * obj);
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
    CCSprite * _defaultBgImg;
    CCSWFNodeSequence * _swf;
};



#pragma mark-
#pragma mark KissViewLayerLoader
class KissViewLayerLoader:public cocos2d::extension::CCLayerLoader {
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(KissViewLayerLoader, loader);
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(KissView);
};

NS_KAI_END

#endif

