//
//  CCGameSWF.h
//  CCGameSWF
//
//  Created by dario on 13-02-22.
//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//  Modify by Ryan_Y(yuankaii.cn@gmail.com) on 13-11-04
//  fixed some memory leak issues and reduce memory useage.
//
#include "cocos2d.h"
#include "gameswf.h"
class CCGameSWF {
public:
    CCGameSWF();
    ~CCGameSWF();
private:
    cocos2d::CCArray *m_fscommandListeners;
    gameswf::sound_handler* m_sound;
    gameswf::render_handler* m_render;
public:
    static CCGameSWF* sharedInstance();
    static void releaseInstance();
    void addFscommandResponder( int responder,cocos2d::CCString* movieName);
    void removeFscommandResponder(int responder, cocos2d::CCString* movieName);
    bool init();
    static const int32_t getSwfCount();
    static void addOneSwf();
    static void removeOneSwf();
};


