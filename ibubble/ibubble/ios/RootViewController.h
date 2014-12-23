//
//  ibubbleAppController.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-11.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GameKit/GKAchievementViewController.h>

#import "AdMoGoDelegateProtocol.h"
#import "AdMoGoView.h"
#import "AdMoGoWebBrowserControllerUserDelegate.h"

#import "AdMoGoInterstitialDelegate.h"

@interface RootViewController : UIViewController <GKAchievementViewControllerDelegate, AdMoGoDelegate, AdMoGoWebBrowserControllerUserDelegate,AdMoGoInterstitialDelegate>

@property (nonatomic, strong) AdMoGoView* adView;

-(void)resetAd;
-(void)removeAd;

-(void)initFullMogo;
-(void)showFullAd;
-(void)cancelFullAd;
-(void)clearAllAd;
@end
