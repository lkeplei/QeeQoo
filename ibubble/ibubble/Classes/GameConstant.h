//
//  GameConstant.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameConstant_h
#define ibubble_GameConstant_h

enum EGameAudioBackgroundType {
	K_BG_MUSIC_BATTLE 			     =1,
	K_BG_MUSIC_OUTSIDE_OF_BATTLE     =2,
};

enum EGameBattleType {
	K_NORMAL_PVE_BATTLE     = 1,//普通模式
	K_HARD_PVE_BATTLE       = 2,//挑战模式
};

enum EGameObjectType {
	KNPCTypeId 			 =1,
	KBulletTypeId     	 =2,
	
	KNPCTypeNormalId 			 =100,
	KNPCTypeStrengthId 			 =101,
	KNPCTypeSuperId 			 =102,
};

enum EGameStateType {
	KGameStateNone 			 =0,
	KGameStateInit 			 =1,
	KGameStateRunning     	 =2,
	KGameStateFailed     	 =3,
	KGameStateSuccess     	 =4,
};

#define kUnlockidHeadString     "unlockid_"
#define kStoryZoneMaxId  (3)
#endif
