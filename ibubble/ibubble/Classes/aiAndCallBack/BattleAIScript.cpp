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
NS_KAI_BEGIN
bool BattleAIScript::runScript(GameObject * aGameObject,CCDictionary * dic,float dt) {
	GameModle * gModleInstance = GameModle::sharedInstance();
	GameController * controller = GameController::sharedInstance();
	PlayerAchievement & achievement = gModleInstance->playerAchievement();
	
	if (achievement._idleTime >= achievement._idleMaxTime ) {
		
		
		if (gModleInstance->currentBigLevelId() >= kStoryZoneMaxId) {
			if (achievement._killNpcCount > 0) {
				controller->pauseBattle();
				controller->switchSence(GameController::K_SCENE_ACHIEVEMENT);
			}
		}
		else{
			if (achievement._killNpcCount >= achievement._pass_count) {
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
				
				controller->switchSence(GameController::K_SCENE_SUCCESS);
				
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
