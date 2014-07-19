//
//  CCSWFNode.m
//  GSGGameSWF
//
//  Created by dario on 13-02-25.
//  Modify by chenee (chenee543216@gmail.com) on 13-05-20
//  @chenee: 1\change Objective-C to C++ ;2\change opengl es 1.1 to opengl es 2.0
//  Modify by Ryan_Y(yuankaii.cn@gmail.com) on 13-11-04
//  fixed some memory leak issues and reduce memory useage.

#import "CCSWFNode.h"
#import "CCGameSWF.h"
#import "cocos2d.h"
#import "gameswf.h"
#import "gameswf_player.h"
#import "gameswf_root.h"
#import "ccMacros.h"
#import "gameswf_types.h"
#import "gameswf_impl.h"

using namespace cocos2d;
class CCSWFNode_touchContainer : public cocos2d::CCObject{
public:
    CCPoint m_position;
    int m_state;
public:
    CCSWFNode_touchContainer(const CCPoint & position, const int state);
    bool touchContainerWithPosition(const CCPoint & position, const int state);
    static CCSWFNode_touchContainer * create(const CCPoint & position, const int state);
};

CCSWFNode_touchContainer::CCSWFNode_touchContainer(const CCPoint & position, const int state){
    m_position = position;
    m_state = state;
}

bool CCSWFNode_touchContainer::touchContainerWithPosition(const CCPoint & position, const int state){
    return false;
}

CCSWFNode_touchContainer * CCSWFNode_touchContainer::create(const CCPoint & position, const int state){
    CCSWFNode_touchContainer *touch = new CCSWFNode_touchContainer(position,state);
    if (touch) {
        touch->autorelease();
        return touch;
    }
    return NULL;
}

class CCSWFNode_imp : public cocos2d::CCNode
{
public:
    gameswf::gc_ptr<gameswf::player> m_player;
    gameswf::gc_ptr<gameswf::root>	m_movie;
public:
    CCSWFNode_imp();
    ~CCSWFNode_imp();
    bool initWithSWFFile(const char *file);
    void cleanAll();
};

bool CCSWFNode_imp::initWithSWFFile(const char *file){
    std::string pathKey = file;
    pathKey = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(pathKey.c_str());
    if (pathKey.size() == 0){
        return false;
    }
    CCGameSWF::sharedInstance();
    m_player = new gameswf::player();
    if (m_player) {
        m_movie = m_player->load_file(pathKey.c_str());
        if (m_movie) {
            CCGameSWF::addOneSwf();
            return true;
        }
    }
    CCLOG("ERROR: Cannot open input file %s", file);
    return false;
}

CCSWFNode_imp::CCSWFNode_imp()
:m_movie(NULL)
,m_player(NULL){

}

CCSWFNode_imp::~CCSWFNode_imp(){
    CCLOG("CCSWFNode_imp::~CCSWFNode_imp()");
    m_movie = NULL;
    m_player = NULL;
    CCGameSWF::removeOneSwf();
    if (CCGameSWF::getSwfCount() <= 0) {
        CCGameSWF::releaseInstance();
    }
}

CCSWFNode::CCSWFNode()
:m_touchEvents(NULL)
,m_imp(NULL){

}

CCSWFNode::~CCSWFNode(){
    CCLOG("CCSWFNode::~CCSWFNode()");
    this->stopAction();
    CC_SAFE_DELETE(m_imp);
    CC_SAFE_RELEASE_NULL(m_touchEvents);
}

CCString* CCSWFNode::movieName(){
    return CCString::createWithFormat(m_imp->m_movie->m_movie->m_name.c_str());
}

void CCSWFNode::setMovieName(char *movieName){
    m_imp->m_movie->m_movie->m_name = movieName;
}

CCSWFNode* CCSWFNode::create(const char* file)
{
    CCSWFNode* swf = new CCSWFNode();
    if (! swf->initWithSWFFile(file)) {
        CC_SAFE_RELEASE_NULL(swf);
        return NULL;
    }
    swf->autorelease();
    return swf;
}

bool CCSWFNode::initWithSWFFile(const char* file)
{
    m_imp = new CCSWFNode_imp();
    if (!m_imp->initWithSWFFile(file)){
        CC_SAFE_RELEASE_NULL(m_imp);
        return false;
    }
    m_movieWidth = m_imp->m_movie->m_def->m_frame_size.m_x_max - m_imp->m_movie->m_def->m_frame_size.m_x_min;
    m_movieHeight = m_imp->m_movie->m_def->m_frame_size.m_y_max - m_imp->m_movie->m_def->m_frame_size.m_y_min;
    m_localScaleX = (m_imp->m_movie->get_movie_width() / m_movieWidth);
    m_localScaleY = -(m_imp->m_movie->get_movie_height() / m_movieHeight);
    m_scaleX = 1.0;
    m_scaleY = 1.0;
    
    m_touchEvents = new CCArray();//[[NSMutableArray alloc] init];
    
    this->setContentSize(CCSize(m_movieWidth, m_movieHeight));
    this->setScale(1.0f);
    this->setAnchorPoint(ccp(0.5,0.5));
    this->isGrouped = false;
    this->isRuning = false;
    this->m_listener = NULL;
    this->m_endListener = NULL;
    this->isRepeat = false;
    this->iCurrentFrame = -1;
    this->iFrameCount = m_imp->m_movie->get_frame_count();
    return true;
}
void CCSWFNode::setFlipX(bool flipX)
{
    if (flipX) {
        setScaleX(m_localScaleX * -1);
    }
}
void CCSWFNode::setFlipY(bool flipY)
{
    if (flipY) {
        setScaleY(m_localScaleY * -1);
    }
}
float CCSWFNode::scale()
{
    CCAssert(m_scaleX == m_scaleY, "CCNode#scale. ScaleX != ScaleY. Don't know which one to return");
	return m_scaleX;
}

void CCSWFNode::setScale(float scale)
{
    m_scaleX = m_scaleY = scale;
    setScaleX(m_localScaleX*m_scaleX);
    setScaleY(m_localScaleY*m_scaleY);
}

float CCSWFNode::scaleX()
{
    return m_scaleX;
}

void CCSWFNode::setScaleX(float scaleX)
{
    m_scaleX = scaleX;
    super::setScaleX(m_scaleX);
}

float CCSWFNode::scaleY()
{
    return m_scaleY;
}

void CCSWFNode::setScaleY(float scaleY)
{
    m_scaleY = scaleY;
    super::setScaleY(m_scaleY);
    
}

void CCSWFNode::onEnter()
{
    super::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

void CCSWFNode::onExit()
{
    if(isRuning){
        unschedule(schedule_selector(CCSWFNode::update));
        this->isRuning = false;
    }
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    super::onExit();
}

void CCSWFNode::runAction()
{
    if(isRuning)
        return;
    
    //if in singleton group we stop other sibling.
    if (isGrouped) {
       CCSWFNodeGroup* g = (CCSWFNodeGroup*)this->getParent();
        if (g->getSingleton()) {
            g->stopAction();
        }
    }
    this->iCurrentFrame = -1;
    this->isRuning = true;
    //reset 
    this->m_imp->m_movie->goto_frame(0);
    this->m_imp->m_movie->set_play_run();
    
    schedule(schedule_selector(CCSWFNode::update),1/2);
}


void CCSWFNode::stopAction()
{
    if(! isRuning)
        return;
    unschedule(schedule_selector(CCSWFNode::update));
    this->isRuning = false;
}

void CCSWFNode::resumeAction()
{
    if(isRuning)
        return;
    schedule(schedule_selector(CCSWFNode::update),1/2);
    this->isRuning = true;
}

void CCSWFNode::addCompletedListener(CCObject* target,SEL_MenuHandler selector){
    CCAssert(target != NULL, "swf action should not be NULL");
    m_endListener = target;
    m_pfnEndSelector = selector;
}


void CCSWFNode::addFrameListener(CCObject* target,SEL_MenuHandler selector)
{
    CCAssert(target != NULL, "swf action should not be NULL");
    m_listener = target;
    m_pfnSelector = selector;
}
void CCSWFNode::setNextAction(CCSWFNode* target)
{
    CCAssert(target != NULL, "next swf action should not be NULL");
    
    m_endListener = target;
    m_pfnEndSelector = menu_selector(CCSWFNode::runAction);
}
void CCSWFNode::setRepeat(bool flag)
{
    this->isRepeat = flag;
}
bool CCSWFNode::getRepeat()
{
    return isRepeat;
}

const CCPoint CCSWFNode::getTouchInMovieCoordinates(CCTouch *pTouch)
{
    // find the movie rect in pixels //
    CCPoint selfAnchorPoint = this->getAnchorPoint();
    CCPoint selfPosition = this->getPosition();
    CCSize  selfContentSize = this->getContentSize();
    float superScaleX = super::getScaleX();
    float superScaleY = super::getScaleY();
    CCRect movieRect = CCRectMake(
                                  selfPosition.x - (selfContentSize.width * superScaleX * selfAnchorPoint.x),
                                  selfPosition.y - (selfContentSize.height * -superScaleY * selfAnchorPoint.y),
                                  selfContentSize.width * superScaleX,
                                  selfContentSize.height * -superScaleY
                                  );

    // find the touch position in pixels //
    CCPoint pTouchPosition = CCDirector::sharedDirector()->convertToGL(pTouch->getLocationInView());
    CCPoint touchPoint = ccpMult(pTouchPosition, CC_CONTENT_SCALE_FACTOR());

    // find the touch position in respect to the movie //
    CCPoint touchInMovie = ccp(((touchPoint.x - movieRect.origin.x) / m_scaleX),
                               (((selfContentSize.height * -superScaleY) - (touchPoint.y - movieRect.origin.y)) / m_scaleY));

    return touchInMovie;
}

bool CCSWFNode::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint selfAnchorPoint = this->getAnchorPoint();
    CCPoint selfPosition = this->getPosition();
    CCSize  selfContentSize = this->getContentSize();
    float superScaleX = super::getScaleX();
    float superScaleY = super::getScaleY();
    // make rect for movie //
    CCRect movieRect = CCRectMake(0,
                                  0,
                                  (selfContentSize.width * superScaleX),
                                  (selfContentSize.height * -superScaleY));
    
    // find the touch position in pixels
    CCPoint touchPoint = this->getTouchInMovieCoordinates(pTouch);
    bool isInMovie = movieRect.containsPoint(touchPoint);
    m_touchEvents->addObject(CCSWFNode_touchContainer::create(touchPoint,0));
    m_touchEvents->addObject(CCSWFNode_touchContainer::create(touchPoint,1));
    return isInMovie;
}

void CCSWFNode::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEvents->addObject(CCSWFNode_touchContainer::create(getTouchInMovieCoordinates(pTouch),0));
}
void CCSWFNode::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEvents->addObject(CCSWFNode_touchContainer::create(getTouchInMovieCoordinates(pTouch),0));
}

void CCSWFNode::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    m_touchEvents->addObject(CCSWFNode_touchContainer::create(getTouchInMovieCoordinates(pTouch),0));
}

const int CCSWFNode::getCurrentFrame(){
    return iCurrentFrame;
}

const int CCSWFNode::getFrameCount(){
    return iFrameCount;
}

void CCSWFNode::update(float dt)
{
    if (m_touchEvents && m_touchEvents->count() > 0){
        CCSWFNode_touchContainer *touch = (CCSWFNode_touchContainer *)m_touchEvents->objectAtIndex(0);
        m_imp->m_movie->notify_mouse_state(touch->m_position.x, touch->m_position.y, touch->m_state);
        m_touchEvents->removeObjectAtIndex(0);
    }
    
    int iFrame = m_imp->m_movie->get_current_frame();
    if (iCurrentFrame != iFrame) {
        iCurrentFrame = iFrame;
        if (m_listener) {
            (m_listener->*m_pfnSelector)(this);
        }
    }

    m_imp->m_movie->advance(dt);
    
    if(! isRepeat){
        if (iFrame == (iFrameCount - 1)) {
            this->stopAction();
            if(m_endListener){
                (this->m_endListener->*m_pfnEndSelector)(this);
            }
        }
    }
    //    CCLOG(".....");
    // TODO: Enable sound //
    //sound->advance(dt);
    
}
#define STRINGIFY(A)  #A
const char* ColorVertexShader1 = STRINGIFY(
attribute vec4 a_position;
uniform mat4 Projection;
uniform mat4 u_MVPMatrix;
void main(void)
{
    vec4 tmp;
    tmp =  Projection * u_MVPMatrix * a_position;
//    tmp = u_MVPMatrix * a_position;
//    tmp = a_position;
    
    //    tmp.x *= -1.0;
//    tmp.y *= -1.0;
    gl_Position = tmp;
    
}
);

const char* ColorFragmentShader1 = STRINGIFY(
uniform lowp vec4 u_color;
                                            
void main(void)
{
    gl_FragColor = u_color;
}
);
unsigned int m_currentProgram = 0;

void ApplyOrtho(float maxX, float maxY)
{
    
    //        maxX = 1024*20/2;
    //        maxY = 768*20/2;
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0,  0, 0,
        0, b,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    };
    
    GLint projectionUniform = glGetUniformLocation(m_currentProgram, "Projection");
    glUniformMatrix4fv(projectionUniform, 1, 0, &ortho[0]);
    CHECK_GL_ERROR_DEBUG();
}
void ApplyMatrix()
{
    float	mat[16]={
//        1.0f/512,    0,              0,              0,
//        0,              1.0f/384,    0,              0,
//        0,0,-0.000976562,   0,
//        -0.273438,-0.234375,0,1,
        
        1/512,0,0,0,
        0,-1/384,0,0,
        0,0,-1/1024,0,
//        0,0,0,1,
        -372,-474,0,1,
    };

    GLint modelviewUniform = glGetUniformLocation(m_currentProgram, "u_MVPMatrix");
    glUniformMatrix4fv(modelviewUniform, 1, 0, mat);
    CHECK_GL_ERROR_DEBUG();
}
static void ApplyColor()
{
    GLfloat color[4];
    {
        color[0] = 0.5;
        color[1] = 0.5;
        color[2] = 1.0;
        color[3] = 1.0;
    }
    
    GLint lineColorSlot = glGetUniformLocation(m_currentProgram, "u_color");
    glUniform4fv(lineColorSlot, 1, color);
    CHECK_GL_ERROR_DEBUG();
    
}
GLuint BuildShader(const char* source, GLenum shaderType)
{
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, 0);
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        printf("%s", messages);
        exit(1);
    }
    
    return shaderHandle;
}

GLuint BuildProgram(const char* vertexShaderSource,
                    const char* fragmentShaderSource) 
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf("%s", messages);
        
        exit(1);
    }
    
    return programHandle;
}

void CCSWFNode::draw()
{
    if (! isRuning) {
        return;
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP );
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV );
    kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
    
#if CC_ENABLE_GL_STATE_CACHE
    ccGLUseProgram(0);//valid program is NON_ZERO unsigned int
#endif
	m_imp->m_movie->display(&matrixMVP.mat[0]);
    CHECK_GL_ERROR_DEBUG();
    glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

}

//@end
