//
//  ibubbleAppDelegate.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-11.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#include "AppDelegate.h"
#include "cocos2d.h"
#include "GameConfig.h"
#include "SimpleAudioEngine.h"
#include "StartupScene.h"
#include "GameResoureManager.h"
#include "GameModle.h"
#include "GameConstant.h"
#include "GameData.h"

#include "GameCenter.h"
#import "GameKitHelper.h"

#import "MobClick.h"

USING_NS_CC;
USING_NS_KAI;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
	//释放数据库
	GameData::releaseInstance();
	CocosDenshion::SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    //GameCenter登陆
    [[GameKitHelper sharedGameKitHelper] authenticateLocalUser];
    //    GameCenter::login();
    
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	//计算加载资源的路径
	TargetPlatform target = getTargetPlatform();
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	
	if (target == kTargetIpad) {
		 CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
         CCSize designSize = CCSizeMake(1024, 768);
         CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);
	}
	else if (target == kTargetIphone){
		
		 CCSize designSize = CCSizeMake(480, 320);
		 CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
		 
		 //根据屏幕的长度
		 if (screenSize.height > 320)
		 {
		 	pDirector->setContentScaleFactor(640.0f / designSize.height);
		 }
		 else
		 {
		 	pDirector->setContentScaleFactor(320.0f / designSize.height);
		 }
		 
		 CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder);
	}

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	//初始化数据库
	GameData::Instance();
	
	//切换到启动页面
	StartupScene * layer = StartupScene::createWithCCB();
	CCScene *pScene = CCScene::create();
	if (layer) {
		pScene->addChild(layer);
	}
    pDirector->runWithScene(pScene);
	//异步加载图片
	CCArray *spriteFiles = (CCArray *)GameConfig::sharedInstance()->getValue(KStrImages);
	GameResoureManager::sharedInstance()->loadSpriteSheetAsync(spriteFiles,layer,callfuncO_selector(StartupScene::loadImagesCallback));
	
	GameModle::sharedInstance()->playBackground(K_BG_MUSIC_OUTSIDE_OF_BATTLE);
    
    pDirector->setProjection(kCCDirectorProjection2D);//指定cocos2d为2d模式
    
    //初始友盟统计
    //    [MobClick setCrashReportEnabled:NO]; // 如果不需要捕捉异常，注释掉此行
    //    [MobClick setLogEnabled:YES];  // 打开友盟sdk调试，注意Release发布时需要注释掉此行,减少io消耗
    [MobClick setAppVersion:XcodeAppVersion]; //参数为NSString * 类型,自定义app版本信息，如果不设置，默认从CFBundleVersion里取
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
        [MobClick startWithAppkey:@"5419176f56240bc9d10069fb"];
    } else {
        [MobClick startWithAppkey:@"53bcd00d56240b0c1b01f0c4"];
    }
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
