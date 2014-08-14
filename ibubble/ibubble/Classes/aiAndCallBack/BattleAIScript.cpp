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

NS_KAI_BEGIN
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
                
                if (gModleInstance->getBattleMode() == K_HARD_PVE_BATTLE) {
                    //挑战模式下过关，加关卡
                    achievement._currentHardLevelId++;
                }
                
				controller->pauseBattle();
				achievement._failed = false;
				achievement._star_count = (achievement._killNpcCount >= achievement._pass_star_count) ? 1 : 0;
				std::vector<PlayerAchievement> list = GameData::Instance().findData(PlayerAchievement::getLevelId(achievement));
				if (list.size() == 0) {
					GameData::Instance().saveData(achievement);
                    GameCenter::postScore(KGameCenterScoreId, achievement._records);
				}
				else if(list.size() > 0){
					const PlayerAchievement lastRecord = list[0];
					if (lastRecord._records < achievement._records) {
						GameData::Instance().saveData(achievement);
                        GameCenter::postScore(KGameCenterScoreId, achievement._killNpcCount);
					}
				}
				
                stringstream bigLevel;
                bigLevel << gModleInstance->currentBigLevelId();
                CCDictionary * dict = (CCDictionary *)GameConfig::sharedInstance()->getLevelsValue(bigLevel.str());
                CCString* file = (CCString *)dict->objectForKey(KStrMovieEnd);
                if (file->compare("0") == 0) {
                    controller->switchSence(GameController::K_SCENE_SUCCESS);
                } else {
                    CCScene *pScene = LevelMVScene::scene(gModleInstance->currentBigLevelId(),
                                                          gModleInstance->currentLevelId(),
                                                          KStrMovieEnd);
                    controller->controllerPushSence(pScene);
//                    
//                    controller->switchSence(GameController::K_SCENE_LEVEL_MV,
//                                            CCInteger::create(gModleInstance->currentBigLevelId()),
//                                            CCInteger::create(gModleInstance->currentLevelId()));
                }
				
				//剧情故事
				std::pair<int, int> counts = GameData::Instance().totalCount();
				GameData::Instance().unlockStory(counts);
			}
			else if(gModleInstance->getBattleTouchTimes() > 0 &&
					gModleInstance->getBattleTouchTimes() >= gModleInstance->getBattleTouchMaxTimes() ){
				
				controller->pauseBattle();
				achievement._failed=true;
				controller->switchSence(GameController::K_SCENE_FAILED);
			}
		}
	}
	
	return true;
}
NS_KAI_END
