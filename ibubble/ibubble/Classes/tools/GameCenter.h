#ifndef kk_GAMECENTER_H
#define kk_GAMECENTER_H
#include "cocos2d.h"
#include <string>
#include "GameMacros.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
NS_KAI_BEGIN
class GameCenter
{
public:
    static void login();
    static bool showAchievements();
    static void postAchievement(const char* idName, int percentComplete);
    static void clearAllAchievements();
    static bool showScores();
    static void postScore(const char* idName, int score);
    static void clearAllScores();
};
NS_KAI_END
#endif /* kk_GAMECENTER_H */
