//
//  BattleAIScript.cpp
//  ibubble
//
//  Created by Ryan Yuan on 12-11-18.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "BattleAIScript.h"
#include "GameObject.h"
#include "GameModle.h"
#include "GameConfig.h"
#include "GameController.h"
#include "GameData.h"
#include "GameConstant.h"
#include "GameCenter.h"

#include "LevelMVScene.h"
#include "GameUtilities.h"

NS_KAI_BEGIN
void countScore() {
    if (GameModle::sharedInstance()->getBattleMode() == K_HARD_PVE_BATTLE) {
        PlayerAchievement &achievement = GameModle::sharedInstance()->playerAchievement();
        //计算得分
        float percent = (float)((float)(achievement._killNpcCount * 100) / (float)achievement._totalNpcCount);
        if (percent > 80) {
            achievement._records += ((percent - 80) * 10000 + 30 * 5000 + 50 * 2000) / 100;
        } else if (percent > 50) {
            achievement._records += ((percent - 50) * 5000 + 50 * 2000) / 100;
        } else {
            achievement._records +=  percent * 2000 / 100;
        }
        
        achievement._records = percent == 100 ? achievement._records * 2 : achievement._records;
        
        int totalRecord = GameUtilities::getRecord();
        totalRecord += achievement._records;
        GameUtilities::saveRecord(totalRecord);
        
        //上传得分
        GameCenter::postScore(KLeaderBoardId, totalRecord);
    }
}

bool BattleAIScript::runScript(GameObject * aGameObject,CCDictionary * dic,float dt) {
	GameModle * gModleInstance = GameModle::sharedInstance();
	GameController * controller = GameController::sharedInstance();
	PlayerAchievement & achievement = gModleInstance->playerAchievement();
	
	if (achievement._idleTime >= achievement._idleMaxTime ) {
		if (gModleInstance->currentBigLevelId() >= kStoryZoneMaxId && gModleInstance->getBattleMode() == K_NORMAL_PVE_BATTLE) {
			if (achievement._killNpcCount > 0) {
				controller->pauseBattle();
				controller->switchSence(GameController::K_SCENE_ACHIEVEMENT);
			}
		}
		else{
			if (achievement._killNpcCount >= achievement._pass_count) {
                //数据处理
                controller->pauseBattle();
                achievement._failed = false;
                achievement._star_count = (achievement._killNpcCount >= achievement._pass_star_count) ? 1 : 0;
                std::vector<PlayerAchievement> list = GameData::Instance().findData(PlayerAchievement::getLevelId(achievement));
                if (list.size() == 0) {
                    GameData::Instance().saveData(achievement);
                    //                    GameCenter::postScore(KGameCenterScoreId, achievement._records);
                } else if(list.size() > 0) {
                    const PlayerAchievement lastRecord = list[0];
                    if (lastRecord._records < achievement._records) {
                        GameData::Instance().saveData(achievement);
                        //                        GameCenter::postScore(KGameCenterScoreId, achievement._killNpcCount);
                    }
                }
                
                //过关记录
                if (gModleInstance->getBattleMode() == K_HARD_PVE_BATTLE) {
                    //挑战模式下过关，加关卡
                    GameUtilities::saveAchieveHardLevelId(GameUtilities::getAchieveHardLevelId() + 1);
                    
                    //过关记录
                    int pass = GameUtilities::getPass();
                    pass++;
                    GameUtilities::savePass(pass);
                    if (achievement._killNpcCount >= achievement._pass_star_count) {
                        int star = GameUtilities::getStar();
                        star++;
                        GameUtilities::saveStar(star);
                    }
                } else {
                    //剧情故事
                    std::pair<int, int> counts0 = GameData::Instance().totalCount(-1, 0);
                    std::pair<int, int> counts1 = GameData::Instance().totalCount(-1, 1);
                    std::pair<int, int> counts2 = GameData::Instance().totalCount(-1, 2);
                    std::pair<int, int> count(0, 0);
                    count.first = counts0.first + counts1.first + counts2.first;
                    count.second = counts0.second + counts1.second + counts2.second;
                    GameData::Instance().unlockStory(count);
                }
                
                countScore();
                
                //战斗结束跳转
                stringstream bigLevel;
                bigLevel << gModleInstance->currentBigLevelId();
                CCDictionary * dict = (CCDictionary *)GameConfig::sharedInstance()->getLevelsValue(bigLevel.str());
                CCString* file = (CCString *)dict->objectForKey(KStrMovieEnd);
                if (file->compare("0") != 0 && gModleInstance->currentLevelId() == 14) {
                    CCScene *pScene = LevelMVScene::scene(gModleInstance->currentBigLevelId(),
                                                          gModleInstance->currentLevelId(),
                                                          KStrMovieEnd);
                    controller->controllerPushSence(pScene);
                } else {
                    controller->switchSence(GameController::K_SCENE_SUCCESS);
                }
			}
			else if(gModleInstance->getBattleTouchTimes() > 0 &&
					gModleInstance->getBattleTouchTimes() >= gModleInstance->getBattleTouchMaxTimes() ){
				
                countScore();

				controller->pauseBattle();
				achievement._failed=true;
				controller->switchSence(GameController::K_SCENE_FAILED);
            }
            
            //解锁奖励-挑战
            GameData::Instance().unlockAchievement();
		}
	}
	
	return true;
}
NS_KAI_END
