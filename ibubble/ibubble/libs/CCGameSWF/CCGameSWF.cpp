//
//  CCGameSWF.m
//  CCGameSWF
//
//  Created by dario on 13-02-22.
//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//  Modify by Ryan_Y(yuankaii.cn@gmail.com) on 13-11-04
//  fixed some memory leak issues and reduce memory useage.
//

#import "CCGameSWF.h"
#import "CCGameSWF.h"
#import "gameswf.h"
#import "gameswf_character.h"
#import "tu_file.h"

using namespace cocos2d;

#pragma mark - Callback declarations
// handler functions //

// fscommand handler //
static void     CCGameSWF_fscommand_handler     (gameswf::character* movie,
                                                 const char* command,
                                                 const char* arg);
 // file opener //
static tu_file* CCGameSWF_file_opener           (const char* url_or_path);

// log handler //
static void     CCGameSWF_log_handler           (bool error,
                                                 const char* message);

#pragma mark - CCGameSWF implementation
//@implementation CCGameSWF
static CCGameSWF *s_ccGameSWF_sharedInstance = NULL;
static int32_t  s_SwfCount = 0;
CCGameSWF* CCGameSWF::sharedInstance()
{
    if (!s_ccGameSWF_sharedInstance)
    {
        s_ccGameSWF_sharedInstance = new CCGameSWF();
        s_ccGameSWF_sharedInstance->init();
    }
    return s_ccGameSWF_sharedInstance;
}

void CCGameSWF::releaseInstance(){
    if (s_ccGameSWF_sharedInstance) {
        delete s_ccGameSWF_sharedInstance;
        s_ccGameSWF_sharedInstance = NULL;
    }
}

const int32_t CCGameSWF::getSwfCount(){
    return s_SwfCount;
}

void CCGameSWF::addOneSwf(){
    s_SwfCount++;
}
void CCGameSWF::removeOneSwf(){
    s_SwfCount--;
}

CCGameSWF::CCGameSWF()
:m_fscommandListeners(NULL)
,m_sound(NULL)
,m_render(NULL){

}

CCGameSWF::~CCGameSWF(){
    // Release the interface
    gameswf::set_sound_handler(NULL);
    gameswf::set_render_handler(NULL);
    gameswf::set_glyph_provider(NULL);
    CC_SAFE_DELETE(m_sound);
    CC_SAFE_DELETE(m_render);
    CC_SAFE_RELEASE_NULL(m_fscommandListeners);
}

bool CCGameSWF::init()
{
    m_fscommandListeners  = new cocos2d::CCArray();
    gameswf::register_file_opener_callback(&CCGameSWF_file_opener);
    gameswf::register_fscommand_callback(&CCGameSWF_fscommand_handler);
#ifdef DEBUG
    gameswf::register_log_callback(&CCGameSWF_log_handler);
#endif
    
    // TODO: Enable audio //
    //m_sound = gameswf::create_sound_handler_openal();
    //gameswf::set_sound_handler(m_sound);
    
    gameswf::set_sound_handler(NULL);
    
    m_render = gameswf::create_render_handler_ogles();
    gameswf::set_render_handler(m_render);
    
    gameswf::glyph_provider* provider = gameswf::create_glyph_provider_tu();
    gameswf::set_glyph_provider(provider);
    return true;
}

#pragma mark - Callback implementations

void CCGameSWF_fscommand_handler (gameswf::character* movie,
                                  const char* command,
                                  const char* arg){
    CCLOG("::CCGameSWF_fscommand_handler:: [%s:%s]",command,arg);
    //todo:support command
}

tu_file* CCGameSWF_file_opener (const char* url_or_path){
    CCLOG("::CCGameSWF_file_opener:: %s",url_or_path);
	if (strncasecmp(url_or_path, "http://", 7) == 0){
        //不支持网络下载swf
        CCAssert(false, "not support url");
	}
	return new tu_file(url_or_path, "rb");
}

void CCGameSWF_log_handler (bool error, const char* message){
    CCLOG("CCGameSWF_log_handler:: %s",message);
}



