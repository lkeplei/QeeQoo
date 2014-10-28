//
//  GameKitHelper.m
//  HoldOnDemo
//
//  Created by ken on 14/7/4.
//
//

#import "GameKitHelper.h"
#import "GameMacros.h"

@implementation GameKitHelper

@synthesize gameCenterAvailable;

//静态初始化 对外接口
static GameKitHelper *sharedHelper = nil;
static UIViewController* currentModalViewController = nil;
+ (GameKitHelper *) sharedGameKitHelper {
    if (!sharedHelper) {
        sharedHelper = [[GameKitHelper alloc] init];
    }
    return sharedHelper;
}

//用于验证
- (BOOL)isGameCenterAvailable {
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    // check if the device is running iOS 4.1 or later
    NSString *reqSysVer =@"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer
                                           options:NSNumericSearch] != NSOrderedAscending);
    return (gcClass && osVersionSupported);
}

- (id)init {
    if ((self = [super init])) {
        gameCenterAvailable = [self isGameCenterAvailable];
        if (gameCenterAvailable) {
            NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
            [nc addObserver:self selector:@selector(authenticationChanged) name:GKPlayerAuthenticationDidChangeNotificationName object:nil];
        }
    }
    return self;
}

//后台回调登陆验证
- (void)authenticationChanged {
    if ([GKLocalPlayer localPlayer].isAuthenticated &&!userAuthenticated) {
        NSLog(@"Authentication changed: player authenticated.");
        userAuthenticated = TRUE;
    } else if (![GKLocalPlayer localPlayer].isAuthenticated && userAuthenticated) {
        NSLog(@"Authentication changed: player not authenticated");
        userAuthenticated = FALSE;
    }
}

- (void)authenticateLocalUser {
    if (!gameCenterAvailable) return;
    NSLog(@"Authenticating local user...");
    if ([GKLocalPlayer localPlayer].authenticated == NO) {
//        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:nil];
        
        
        
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
            if (error == nil) {
                //成功处理
                //切换用户时看是否要重新复制Plist文件
                NSLog(@"成功");
                NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
                //            NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
                //            NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
                NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
                NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
                //tempPalyID=[GKLocalPlayer localPlayer].playerID;
            }else {
                //错误处理
                NSLog(@"失败  %@",error);
            }  
        }];
        
        return;
        
        [[GKLocalPlayer localPlayer] setAuthenticateHandler:(^(UIViewController* viewcontroller, NSError *error) {
            if (viewcontroller != nil) {
//                AppDelegate* delegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
//                RootViewController* root = (RootViewController*)delegate.viewController;
//                [root presentViewController:viewcontroller animated:YES completion:nil];
            }else if ([GKLocalPlayer localPlayer].authenticated)
            {
                //do some stuff
                //成功处理
                //切换用户时看是否要重新复制Plist文件
                NSLog(@"成功");
                NSLog(@"1--alias--.%@",[GKLocalPlayer localPlayer].alias);
                //            NSLog(@"2--authenticated--.%d",[GKLocalPlayer localPlayer].authenticated);
                //            NSLog(@"3--isFriend--.%d",[GKLocalPlayer localPlayer].isFriend);
                NSLog(@"4--playerID--.%@",[GKLocalPlayer localPlayer].playerID);
                NSLog(@"5--underage--.%d",[GKLocalPlayer localPlayer].underage);
                //tempPalyID=[GKLocalPlayer localPlayer].playerID;
            }  
        })];
    } else {
        NSLog(@"Already authenticated!");
    }
}

- (void)reportScore:(uint32_t)score{
    GKScore *scoreReporter = [[GKScore alloc] initWithCategory:KLeaderBoardId];
    scoreReporter.value = score;
    
    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
        if (error != nil){
            NSLog(@"上传分数出错.");
        }else {
            NSLog(@"上传分数成功");
        }
    }];
}

-(void) whetherHighestScores:(uint32_t)score{
    _isNewRecord = YES;
    
    GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
    if (leaderboardRequest != nil){
        leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
        leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardRequest.range = NSMakeRange(1,100);
        leaderboardRequest.category = KLeaderBoardId;
        [leaderboardRequest loadScoresWithCompletionHandler: ^(NSArray *scores, NSError *error) {
            if (error != nil){
                NSLog(@"下载失败");
            }
            
            if (scores != nil){
                NSLog(@"下载成功.... scores = %@", scores);
                for (GKScore *obj in leaderboardRequest.scores) {
                    if (obj.value >= score) {
                        _isNewRecord = NO;
                        NSLog(@"value = %lld, score = %d", obj.value, score);
                        break;
                    }
                }
            }
        }];
    }
}

//显示排行榜
- (void) showLeaderboard{
    if (!gameCenterAvailable) return;
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardController != nil) {
        leaderboardController.leaderboardDelegate = self;
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        currentModalViewController = [[UIViewController alloc] init];
        [window addSubview:currentModalViewController.view];
        [currentModalViewController presentModalViewController:leaderboardController animated:YES];
    }
}

//关闭排行榜回调
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController{
    if(currentModalViewController !=nil){
        [currentModalViewController dismissModalViewControllerAnimated:NO];
        [currentModalViewController release];
        [currentModalViewController.view removeFromSuperview];
        currentModalViewController = nil;
    }
}

// The achievement view has finished
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController{
    
}
@end
