//
//  CCSWFNodeExt.h
//
//  Created by Ryan Yuan on 13-11-19.
//
//

#ifndef ibubble_CCSWFNodeExt_h
#define ibubble_CCSWFNodeExt_h

#include "cocos2d.h"
#include <string>
#include "CCSWFNode.h"
USING_NS_CC;
class CCSWFNodeExt :public CCSWFNode{
public:
    CCSWFNodeExt();
    ~CCSWFNodeExt();
public:
    static CCSWFNodeExt* create(const char* swfFile,
                                const char* logicFile);
    bool initWithSWFFile(const char* swfFile,
                         const char* logicFile);
public:
    virtual void runAction();
    void handleFrameChanged(cocos2d::CCObject * obj);
public:
    void setStopBgMusic(bool stop);
    void addFrameChangedListener(CCObject* target,SEL_MenuHandler selector);
private:
    bool m_stopBgMusic;
    CCDictionary * m_logicDict;
    std::string    m_music;
    CCArray * m_loadEffects;
    CCObject* m_frameChangedListener;
    SEL_MenuHandler m_frameChangedEndSelector;
};

class CCSWFNodeSequence:public CCNode{
public:
    CCSWFNodeSequence();
    ~CCSWFNodeSequence();
public:
    static CCSWFNodeSequence* create(const char* swfFile);
    bool initWithFile(const char* swfFile);
public:
    void stopSWFSequence();
    void resumeSWFSequence();
    void runSWFSequence();
    void addCompletedListener(CCObject* target,SEL_MenuHandler selector);
    void addFrameListener(CCObject* target,SEL_MenuHandler selector);
    const int getCurrentSequenceIndex();
    const int getSequenceSize();
private:
    void runSWF(int index);
    void handleSWFFrameChanged(cocos2d::CCObject * obj);
    void handleSWFFinished(cocos2d::CCObject * obj);
private:
    CCArray * m_Sequence;
    int       m_index;
    CCSWFNodeExt * m_currentSWFNode;
    CCObject* m_endListener;
    SEL_MenuHandler m_pfnEndSelector;
    
    CCObject* m_frameListener;
    SEL_MenuHandler m_frameEndSelector;
};
#endif
