//
//  CCSWFNodeExt.cpp
//
//  Created by Ryan Yuan on 13-11-19.
//
//

#include "CCSWFNodeExt.h"
#include "SimpleAudioEngine.h"

CCSWFNodeExt::CCSWFNodeExt():m_logicDict(NULL),m_loadEffects(NULL),m_stopBgMusic(true),m_frameChangedListener(NULL){

}

CCSWFNodeExt::~CCSWFNodeExt(){
    CC_SAFE_RELEASE_NULL(m_logicDict);
    CC_SAFE_RELEASE_NULL(m_loadEffects);
}

CCSWFNodeExt* CCSWFNodeExt::create(const char* swfFile,
                                   const char* logicFile){
    CCSWFNodeExt* swf = new CCSWFNodeExt();
    if (! swf->initWithSWFFile(swfFile,logicFile)) {
        CC_SAFE_RELEASE_NULL(swf);
        return NULL;
    }
    swf->addFrameListener(swf,menu_selector(CCSWFNodeExt::handleFrameChanged));
    swf->autorelease();
    return swf;
}

bool CCSWFNodeExt::initWithSWFFile(const char* swfFile,
                                   const char* logicFile){
    CCSWFNode::initWithSWFFile(swfFile);
    if(logicFile){
        m_loadEffects = new CCArray;
        m_logicDict = CCDictionary::createWithContentsOfFile(logicFile);
        if(m_logicDict)
            m_logicDict->retain();
    }
    return true;
}

void CCSWFNodeExt::runAction(){
    m_music = "";
    m_loadEffects->removeAllObjects();
    CCSWFNode::runAction();
}

void CCSWFNodeExt::handleFrameChanged(cocos2d::CCObject * obj){
    CCSWFNode * node = (CCSWFNode *)obj;
    if (m_logicDict) {
        
        CocosDenshion::SimpleAudioEngine * audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
        std::stringstream frameKey;
        frameKey << (node->getCurrentFrame() + 1);
        CCDictionary * dict = (CCDictionary *)m_logicDict->objectForKey(frameKey.str());
        if (dict) {
            //切换背景音乐
            CCString * music = (CCString *)dict->objectForKey("music");
            if (music && m_music.compare(music->m_sString) != 0) {
                m_music = music->m_sString;
                if (m_music.length() > 0) {
                    CCLOG("CSWFNodeExt::handleFrameChanged %d playBackgroundMusic %s",node->getCurrentFrame(),m_music.c_str());
                    audioEngine->stopBackgroundMusic(true);
                    audioEngine->preloadBackgroundMusic(m_music.c_str());
                    audioEngine->playBackgroundMusic(m_music.c_str(),true);
                }
            }
            
            //播放音效
            CCString * effect = (CCString *)dict->objectForKey("effect");
            if (effect && effect->length() > 0) {
                const std::string tag = "#";
                std::string srcString = effect->m_sString;
                size_t startPos = srcString.find_first_of(tag);
                if (startPos != std::string::npos) {
                    std::string tmp;
                    do {
                        tmp = srcString.substr(0,startPos);
                        CCLOG("CSWFNodeExt::handleFrameChanged %d playEffect %s",node->getCurrentFrame(),tmp.c_str());
                        audioEngine->preloadEffect(tmp.c_str());
                        audioEngine->playEffect(tmp.c_str(), false);
                        srcString = srcString.substr(startPos+tag.length());
                        startPos = srcString.find_first_of(tag);
                        if(startPos == std::string::npos){
                            CCLOG("CSWFNodeExt::handleFrameChanged %d playEffect %s",node->getCurrentFrame(),tmp.c_str());
                            audioEngine->preloadEffect(tmp.c_str());
                            audioEngine->playEffect(tmp.c_str(), false);
                        }
                    } while(startPos != std::string::npos) ;
                }else{
                    CCLOG("CSWFNodeExt::handleFrameChanged %d playEffect %s",node->getCurrentFrame(),srcString.c_str());
                    audioEngine->preloadEffect(srcString.c_str());
                    audioEngine->playEffect(srcString.c_str(), false);
                }
            }
        }
        
        if (node->getCurrentFrame() == node->getFrameCount() - 1) {
            audioEngine->stopAllEffects();
            if (m_stopBgMusic) {
               audioEngine->stopBackgroundMusic(true);
            }
            CCObject * effectFile = NULL;
            CCARRAY_FOREACH(m_loadEffects, effectFile){
                audioEngine->unloadEffect(((CCString *)effectFile)->getCString());
            }
            m_loadEffects->removeAllObjects();
        }
    }
    
    if(m_frameChangedListener){
        (m_frameChangedListener->*m_frameChangedEndSelector)(this);
    }
}

void CCSWFNodeExt::setStopBgMusic(bool stop){
    m_stopBgMusic = stop;
}

void CCSWFNodeExt::addFrameChangedListener(CCObject* target,SEL_MenuHandler selector){
    m_frameChangedListener = target;
    m_frameChangedEndSelector = selector;
}

CCSWFNodeSequence::CCSWFNodeSequence()
:m_Sequence(NULL)
,m_currentSWFNode(NULL)
,m_endListener(NULL)
,m_frameListener(NULL){

}

CCSWFNodeSequence::~CCSWFNodeSequence(){
    CC_SAFE_RELEASE_NULL(m_Sequence);
}

CCSWFNodeSequence* CCSWFNodeSequence::create(const char* swfFile){
    CCSWFNodeSequence * seq = new CCSWFNodeSequence;
    if(seq && seq->initWithFile(swfFile)){
        seq->autorelease();
        return seq;
    }
    CC_SAFE_DELETE(seq);
    return NULL;
}

bool CCSWFNodeSequence::initWithFile(const char* swfFile){
    CCDictionary * dict = CCDictionary::createWithContentsOfFile(swfFile);
    m_Sequence = (CCArray *)dict->objectForKey("swf");
    m_Sequence->retain();
    setAnchorPoint(ccp(0.5, 0.5));
    return true;
}


void CCSWFNodeSequence::stopSWFSequence(){
    if (m_currentSWFNode) {
        m_currentSWFNode->stopAction();
    }
}

void CCSWFNodeSequence::resumeSWFSequence(){
    if (m_currentSWFNode) {
        m_currentSWFNode->resumeSchedulerAndActions();
    }
}


void CCSWFNodeSequence::runSWFSequence(){
    m_index = 0;
    runSWF(m_index);
}

const int CCSWFNodeSequence::getCurrentSequenceIndex(){
    return m_index;
}

const int CCSWFNodeSequence::getSequenceSize(){
    return m_Sequence->count();
}

void CCSWFNodeSequence::runSWF(int index){
    if (m_currentSWFNode) {
        m_currentSWFNode->stopAction();
        m_currentSWFNode->removeFromParentAndCleanup(true);
        m_currentSWFNode = NULL;
    }
    CCString * swf = (CCString *)m_Sequence->objectAtIndex(index);
    std::stringstream logicFile;
    logicFile << swf->m_sString << ".plist";
    m_currentSWFNode = CCSWFNodeExt::create(swf->getCString(), logicFile.str().c_str());
    m_currentSWFNode->setVisible(true);
    m_currentSWFNode->setRepeat(false);
    m_currentSWFNode->setStopBgMusic(false);
    this->addChild(m_currentSWFNode);
    m_currentSWFNode->addCompletedListener(this,menu_selector(CCSWFNodeSequence::handleSWFFinished));
    m_currentSWFNode->addFrameChangedListener(this,menu_selector(CCSWFNodeSequence::handleSWFFrameChanged));
    m_currentSWFNode->runAction();
}

void CCSWFNodeSequence::addCompletedListener(CCObject* target,SEL_MenuHandler selector){
    m_endListener = target;
    m_pfnEndSelector = selector;
}

void CCSWFNodeSequence::addFrameListener(CCObject* target,SEL_MenuHandler selector){
    m_frameListener = target;
    m_frameEndSelector = selector;
}

void CCSWFNodeSequence::handleSWFFrameChanged(cocos2d::CCObject * obj){
    if(m_frameListener){
        (m_frameListener->*m_frameEndSelector)(obj);
    }
}

void CCSWFNodeSequence::handleSWFFinished(cocos2d::CCObject * obj){
    ++m_index;
    if (m_index < m_Sequence->count()) {
        runSWF(m_index);
    } else if(m_endListener) {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        (m_endListener->*m_pfnEndSelector)(this);
    }
}