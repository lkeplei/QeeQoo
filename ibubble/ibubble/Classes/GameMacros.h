//
//  GameMacros.h
//  ibubble
//
//  Created by Ryan Yuan on 12-11-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef ibubble_GameMacros_h
#define ibubble_GameMacros_h


#define KenLocal(x, ...) NSLocalizedString(x, nil)       //定义国际化使用


#define PTM_RATIO 32
#define STD_DELTA 0.016
#define KExplosionDelta 0.6
#define KRocketExplosionDelta 0.1
#define NS_KAI_BEGIN                 namespace kai { namespace game {
#define NS_KAI_END                       }}
#define USING_NS_KAI                 using namespace kai::game

#define KBattleTestModle
#define KOpenAllLevels              //开启所有关卡


#define KBattleNodeTageBgLayer 		  99000
#define KBattleNodeTagGameObjectLayer 99001
#define KBattleNodeTagControllerLayer 99002

//constant

/**AI 配置key*/
#define KStrBattleAIScriptDefault        "battle_ai_script_default"
#define KStrGameAIScriptBubble           "game_ai_script_bubble"
#define KStrGameAIScriptBullet           "game_ai_script_bullet"
#define KStrGameAIScriptBulletBezier     "game_ai_script_bullet_bezier"

/**callback 配置key*/
#define KStrCallbackBubbleExplosion   		"bubble_explosion"	
#define KStrCallbackBubbleExplosionEffect   "bubble_explosion_effect"	
#define KStrCallbackBubbleShoot   	   		"bubble_shoot"
#define KStrCallbackBubbleShootLaser   		"bubble_shoot_laser"


#define KStrCallbackBubbleOpenSkillShoot   		"open_skill_shoot"
#define KStrCallbackBubbleOpenSkillShootLaser   "open_skill_shoot_laser"
#define KStrCallbackBubbleOpenSkillSpurtFire    "open_skill_spurt_fire"
#define KStrCallbackRocketExplosion   			"rocket_explosion"	
#define KStrCallbackRocketExplosionEffect   	"rocket_explosion_effect"	

#define KStrExplosion   	  "explosion"
#define KStrExplosionEffect   "explosion_effect"
#define KStrShoot   	"shoot"
#define KStrShootLaser  "shoot_laser"
#define KStrOpenSkill   "open_skill"
#define KStrTarget   	"target"
#define KStrNextLevel   "next_level"
#define KStrSpurtFireAtk   	"spurt_fire_atk"


/*过关的条件*/
#define KStrPassCount   	"pass_count"
#define KStrPassTime   		"pass_time"
#define KStrStarCount   	"star_count"
#define KStrIdleTime   		"idle_time"

/**配置文件key*/
#define KStrNPCs    "npcs"   	/**游戏内所有精灵类型列表*/
#define KStrLayers  "layers" 	/**关卡内的游戏层列表*/
#define KStrAudios  "audios" 	/**关卡内的游戏音效列表*/
#define KStrImages  "images"	/**关卡内的图片资源列表*/
#define KStrEffect   "effect"	/**关卡内的效果列表*/

#define KStrLastPosition    "last_position"
#define KStrCount    "count"
#define KStrDesc     "desc"
#define KStrId     "id"

#define KStrPlist    "plist"
#define KStrImage     "image"
#define KStrMainScript    "main_script"

#define KStrDefault  		"default"
#define KStrDefaultFrame  	"default_frame"
#define KStrSelectedFrame  	"selected_frame"
#define KStrDefaultAnim   	"default_anim"
#define KStrFile      "file"
#define KStrFileArray        "file_array"
#define KStrType      "type"
#define KStrNpc       "npc"
#define KStrBullet       "bullet"
#define KStrLevels    "levels"
#define KStrLevelId    "level"
#define KStrZoneId    "zone"
#define KStrSubLevels    "sub_levels"
#define KStrDisplay   "display"
#define KStrBox2d     "box2d"
#define KStrBox2dFilter  "box2d_filter"
#define KStrRewards   	"rewards"
#define KStrCategoryBits     "category"
#define KStrMaskBits         "mask"
#define KStrGroupIndex     "group"
#define KStrMovieBegin    "movie_begin"
#define KStrMovieEnd      "movie_end"
#define KStrMaxReliveTime      "max_relive_time"

#define KStrFriction     "friction"
#define KStrRestitution  "restitution"
#define KStrDensity      "density"
#define KStrIsSensor     "is_sensor"
#define KStrIsBullet     "is_bullet"
#define KStrGravityScale     "gravity_scale"
#define KStrFixedRotation     "fixed_rotation"
#define KStrBodyType     "body_type"
#define KStrFixtures  "fixtures"
#define KStrATK    "atk"
#define KStrDEF    "def"
#define KStrATK_RANGE_SCALE    "atk_range_scale"
#define KStrATK_RANGE    "atk_range"
#define KStrATK_SOURCE    "atk_source"
#define KStrSKillATK_SOURCE    "skill_atk_source"

#define KStrProperty     "property"
#define KStrScaleX     "scale_x"
#define KStrScaleY     "scale_y"
#define KStrX     "x"
#define KStrY     "y"
#define KStrPosition     "position"
#define KStrAction       "action"
#define KStrAIScript     "aiscript"
#define KStrTouchable     "touchable"
#define KStrRecord        "record"

#define KStrOwner       "owner"
#define KStrParam       "param"
#define KStrSpeed       "speed"
#define KStrDirection       "direction"
#define KStrAnimation          "animation"
#define KStrEffect    	       "effect"
#define KStrAudioName    	       "audio"

#define KStrBezierConfig   "bezier_config"
#define KStrSpeedScale   "speed_scale"
#define KStrDefalutForcer   "defalut_forcer"

#define KStrDelay       "delay"

#define KStrController   	   "controller"
#define KStrBackgound   	   "backgound"			/**背景音乐或图片背景*/
#define KStrInBubble    	   "in_bubble"			/**精灵关在气泡中的动画*/
#define KStrFreeFall    	   "free_fall"			/**精灵破开气泡下落的动画*/
#define KStrSpurtFire    	   "spurt_fire"			/**精灵破开气泡下落的动画*/

#define KStrBubbleExplosion    "bubble_explosion"	/**精灵破开气泡时候的特效*/
#define KStrFinishedCallback   "finished_callback"	/**效果动画等播放完成回调*/
#define KStrExplosionCallback   "explosion_callback"	/**效果动画等播放完成回调*/

#define KStrTimeEvent   "time_event"				/**效果动画等播放过程中时间回调*/
#define KStrCallbacks   "callbacks"					/**效果动画等播放回调*/
#define KStrDuration   "duration"					/**效果动画等播放回调*/
#define KStrCallback   "callback"					

//fonts name
#define KFontSTHeikLight    "STHeitiK-Light"

#define KGameCenterScoreId    "ibubble_save_sprite_score"
#define KGameCenterHardModeScoreId    "ibubble_hard_mode_save_sprite_score"

#define KLeaderBoardId          "com.teamqi.qeeqoofull"           //com.teamqi.qeeqoofull、com.teamqi.qeeqoo1
#define KProductID              "teamqi.qeeqoo.achieve"


#define KBubbleToExplosionDelay    (0.4f)

#endif
