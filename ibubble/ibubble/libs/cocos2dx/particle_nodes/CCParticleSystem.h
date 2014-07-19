/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCPARTICLE_SYSTEM_H__
#define __CCPARTICLE_SYSTEM_H__

#include "CCProtocols.h"
#include "base_nodes/CCNode.h"
#include "cocoa/CCDictionary.h"
#include "cocoa/CCString.h"

NS_CC_BEGIN
/**
 * @addtogroup particle_nodes
 * @{
 */

class CCParticleBatchNode;

//* @enum
enum {
    /** The Particle emitter lives forever */
    kCCParticleDurationInfinity = -1,

    /** The starting size of the particle is equal to the ending size */
    kCCParticleStartSizeEqualToEndSize = -1,

    /** The starting radius of the particle is equal to the ending radius */
    kCCParticleStartRadiusEqualToEndRadius = -1,

    // backward compatible
    kParticleStartSizeEqualToEndSize = kCCParticleStartSizeEqualToEndSize,
    kParticleDurationInfinity = kCCParticleDurationInfinity,
};

//* @enum
enum {
    /** Gravity mode (A mode) */
    kCCParticleModeGravity,

    /** Radius mode (B mode) */
    kCCParticleModeRadius,    
};


/** @typedef tCCPositionType
possible types of particle positions
*/
typedef enum {
    /** Living particles are attached to the world and are unaffected by emitter repositioning. */
    kCCPositionTypeFree,

    /** Living particles are attached to the world but will follow the emitter repositioning.
    Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
    */
    kCCPositionTypeRelative,

    /** Living particles are attached to the emitter and are translated along with it. */
    kCCPositionTypeGrouped,
}tCCPositionType;

// backward compatible
enum {
    kPositionTypeFree = kCCPositionTypeFree,
    kPositionTypeGrouped = kCCPositionTypeGrouped,
}; 

/**
Structure that contains the values of each particle
*/
typedef struct sCCParticle {
    CCPoint     pos;
    CCPoint     startPos;

    ccColor4F    color;
    ccColor4F    deltaColor;

    float        size;
    float        deltaSize;

    float        rotation;
    float        deltaRotation;

    float        timeToLive;

    unsigned int    atlasIndex;

	// add for scale support
    float   heightScale;
    float   heightScaleDelta;
    
    float   widthScale;
    float   widthScaleDelta;
	// end
	
    //! Mode A: gravity, direction, radial accel, tangential accel
    struct {
        CCPoint        dir;
		
		// add for scale support
		float		angle;
		float       speed;
		float       orgSpeed;
		// end
		
        float        radialAccel;
        float        tangentialAccel;
    } modeA;

    //! Mode B: radius mode
    struct {
        float        angle;
        float        degreesPerSecond;
        float        radius;
        float        deltaRadius;
    } modeB;

}tCCParticle;

//typedef void (*CC_UPDATE_PARTICLE_IMP)(id, SEL, tCCParticle*, CCPoint);

class CCTexture2D;


/*
 Add delegate support
 */
class CCParticleSystemDelegate{
public:
	virtual bool updateParticleSize(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleSpeed(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleAngle(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleScale(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleRotation(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleRadialAccel(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleColor(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleTangentialAccel(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleRadius(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleDegreesPerSecond(tCCParticle *p,float elapsed,float dt){return false;}
	virtual bool updateParticleExtra(tCCParticle *p,float elapsed,float dt){return false;}
};

/** @brief Particle System base class.
Attributes of a Particle System:
- emission rate of the particles
- Gravity Mode (Mode A):
- gravity
- direction
- speed +-  variance
- tangential acceleration +- variance
- radial acceleration +- variance
- Radius Mode (Mode B):
- startRadius +- variance
- endRadius +- variance
- rotate +- variance
- Properties common to all modes:
- life +- life variance
- start spin +- variance
- end spin +- variance
- start size +- variance
- end size +- variance
- start color +- variance
- end color +- variance
- life +- variance
- blending function
- texture

cocos2d also supports particles generated by Particle Designer (http://particledesigner.71squared.com/).
'Radius Mode' in Particle Designer uses a fixed emit rate of 30 hz. Since that can't be guaranteed in cocos2d,
cocos2d uses a another approach, but the results are almost identical. 

cocos2d supports all the variables used by Particle Designer plus a bit more:
- spinning particles (supported when using CCParticleSystemQuad)
- tangential acceleration (Gravity mode)
- radial acceleration (Gravity mode)
- radius direction (Radius mode) (Particle Designer supports outwards to inwards direction only)

It is possible to customize any of the above mentioned properties in runtime. Example:

@code
emitter.radialAccel = 15;
emitter.startSpin = 0;
@endcode

*/
class CC_DLL CCParticleSystem : public CCNode, public CCTextureProtocol
{    
protected:
    std::string m_sPlistFile;
    //! time elapsed since the start of the system (in seconds)
    float m_fElapsed;

    // Different modes
    //! Mode A:Gravity + Tangential Accel + Radial Accel
    struct {
        /** Gravity value. Only available in 'Gravity' mode. */
        CCPoint gravity;
        /** speed of each particle. Only available in 'Gravity' mode.  */
        float speed;
        /** speed variance of each particle. Only available in 'Gravity' mode. */
        float speedVar;
        /** tangential acceleration of each particle. Only available in 'Gravity' mode. */
        float tangentialAccel;
        /** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
        float tangentialAccelVar;
        /** radial acceleration of each particle. Only available in 'Gravity' mode. */
        float radialAccel;
        /** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
        float radialAccelVar;
    } modeA;

    //! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
    struct {
        /** The starting radius of the particles. Only available in 'Radius' mode. */
        float startRadius;
        /** The starting radius variance of the particles. Only available in 'Radius' mode. */
        float startRadiusVar;
        /** The ending radius of the particles. Only available in 'Radius' mode. */
        float endRadius;
        /** The ending radius variance of the particles. Only available in 'Radius' mode. */
        float endRadiusVar;            
        /** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
        float rotatePerSecond;
        /** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
        float rotatePerSecondVar;
    } modeB;

    //! Array of particles
    tCCParticle *m_pParticles;

    // color modulate
    //    BOOL colorModulate;

    //! How many particles can be emitted per second
    float m_fEmitCounter;

    //!  particle idx
    unsigned int m_uParticleIdx;

    // Optimization
    //CC_UPDATE_PARTICLE_IMP    updateParticleImp;
    //SEL                        updateParticleSel;

    /** weak reference to the CCSpriteBatchNode that renders the CCSprite */
    CC_PROPERTY(CCParticleBatchNode*, m_pBatchNode, BatchNode);

    // index of system in batch node array
    CC_SYNTHESIZE(unsigned int, m_uAtlasIndex, AtlasIndex);

    //true if scaled or rotated
    bool m_bTransformSystemDirty;
    // Number of allocated particles
    unsigned int m_uAllocatedParticles;

    /** Is the emitter active */
    bool m_bIsActive;
    /** Quantity of particles that are being simulated at the moment */
    CC_PROPERTY_READONLY(unsigned int, m_uParticleCount, ParticleCount)
    /** How many seconds the emitter will run. -1 means 'forever' */
    CC_PROPERTY(float, m_fDuration, Duration)
    /** sourcePosition of the emitter */
    CC_PROPERTY_PASS_BY_REF(CCPoint, m_tSourcePosition, SourcePosition)
    /** Position variance of the emitter */
    CC_PROPERTY_PASS_BY_REF(CCPoint, m_tPosVar, PosVar)
    /** life, and life variation of each particle */
    CC_PROPERTY(float, m_fLife, Life)
    /** life variance of each particle */
    CC_PROPERTY(float, m_fLifeVar, LifeVar)
    /** angle and angle variation of each particle */
    CC_PROPERTY(float, m_fAngle, Angle)
    /** angle variance of each particle */
    CC_PROPERTY(float, m_fAngleVar, AngleVar)

//////////////////////////////////////////////////////////////////////////
public:
    // mode A
    virtual const CCPoint& getGravity();
    virtual void setGravity(const CCPoint& g);
    virtual float getSpeed();
    virtual void setSpeed(float speed);
    virtual float getSpeedVar();
    virtual void setSpeedVar(float speed);
    virtual float getTangentialAccel();
    virtual void setTangentialAccel(float t);
    virtual float getTangentialAccelVar();
    virtual void setTangentialAccelVar(float t);
    virtual float getRadialAccel();
    virtual void setRadialAccel(float t);
    virtual float getRadialAccelVar();
    virtual void setRadialAccelVar(float t);
    // mode B
    virtual float getStartRadius();
    virtual void setStartRadius(float startRadius);
    virtual float getStartRadiusVar();
    virtual void setStartRadiusVar(float startRadiusVar);
    virtual float getEndRadius();
    virtual void setEndRadius(float endRadius);
    virtual float getEndRadiusVar();
    virtual void setEndRadiusVar(float endRadiusVar);
    virtual float getRotatePerSecond();
    virtual void setRotatePerSecond(float degrees);
    virtual float getRotatePerSecondVar();
    virtual void setRotatePerSecondVar(float degrees);

    virtual void setScale(float s);
    virtual void setRotation(float newRotation);
    virtual void setScaleX(float newScaleX);
    virtual void setScaleY(float newScaleY);
    
    virtual bool isActive();
    virtual bool isBlendAdditive();
    virtual void setBlendAdditive(bool value);
//////////////////////////////////////////////////////////////////////////
    
    /** start size in pixels of each particle */
    CC_PROPERTY(float, m_fStartSize, StartSize)
    /** size variance in pixels of each particle */
    CC_PROPERTY(float, m_fStartSizeVar, StartSizeVar)
    /** end size in pixels of each particle */
    CC_PROPERTY(float, m_fEndSize, EndSize)
    /** end size variance in pixels of each particle */
    CC_PROPERTY(float, m_fEndSizeVar, EndSizeVar)
    /** start color of each particle */
    CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tStartColor, StartColor)
    /** start color variance of each particle */
    CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tStartColorVar, StartColorVar)
    /** end color and end color variation of each particle */
    CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tEndColor, EndColor)
    /** end color variance of each particle */
    CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tEndColorVar, EndColorVar)
    //* initial angle of each particle
    CC_PROPERTY(float, m_fStartSpin, StartSpin)
    //* initial angle of each particle
    CC_PROPERTY(float, m_fStartSpinVar, StartSpinVar)
    //* initial angle of each particle
    CC_PROPERTY(float, m_fEndSpin, EndSpin)
    //* initial angle of each particle
    CC_PROPERTY(float, m_fEndSpinVar, EndSpinVar)
    /** emission rate of the particles */
    CC_PROPERTY(float, m_fEmissionRate, EmissionRate)
    /** maximum particles of the system */
    CC_PROPERTY(unsigned int, m_uTotalParticles, TotalParticles)
    /** conforms to CocosNodeTexture protocol */
    CC_PROPERTY(CCTexture2D*, m_pTexture, Texture)
    /** conforms to CocosNodeTexture protocol */
    CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc)
    /** does the alpha value modify color */
    CC_PROPERTY(bool, m_bOpacityModifyRGB, OpacityModifyRGB)

    /** whether or not the particles are using blend additive.
    If enabled, the following blending function will be used.
    @code
    source blend function = GL_SRC_ALPHA;
    dest blend function = GL_ONE;
    @endcode
    */
    bool m_bIsBlendAdditive;
    /** particles movement type: Free or Grouped
    @since v0.8
    */
    CC_PROPERTY(tCCPositionType, m_ePositionType, PositionType)
    /** whether or not the node will be auto-removed when it has no particles left.
    By default it is false.
    @since v0.8
    */
	
	CC_SYNTHESIZE(CCParticleSystemDelegate *, m_delegate, Delegate);
protected:
    bool m_bIsAutoRemoveOnFinish;
public:
    virtual bool isAutoRemoveOnFinish();
    virtual void setAutoRemoveOnFinish(bool var);

    /** Switch between different kind of emitter modes:
    - kCCParticleModeGravity: uses gravity, speed, radial and tangential acceleration
    - kCCParticleModeRadius: uses radius movement + rotation
    */
    CC_PROPERTY(int, m_nEmitterMode, EmitterMode)

	virtual void handleParticleSystemDestory(CCParticleSystem * particleSystem);
public:
    CCParticleSystem();
    virtual ~CCParticleSystem();
    /** creates an initializes a CCParticleSystem from a plist file.
    This plist files can be created manually or with Particle Designer:
    http://particledesigner.71squared.com/
    @deprecated: This interface will be deprecated sooner or later.
    @since v0.99.3
    */
    CC_DEPRECATED_ATTRIBUTE static CCParticleSystem * particleWithFile(const char *plistFile);

    /** creates an initializes a CCParticleSystem from a plist file.
    This plist files can be created manually or with Particle Designer:
    http://particledesigner.71squared.com/
    @since v2.0
    */
    static CCParticleSystem * create(const char *plistFile);

	//! create a system with a fixed number of particles
    static CCParticleSystem* createWithTotalParticles(unsigned int numberOfParticles);

    /** initializes a CCParticleSystem*/
    bool init();
    /** initializes a CCParticleSystem from a plist file.
    This plist files can be created manually or with Particle Designer:
    http://particledesigner.71squared.com/
    @since v0.99.3
    */
    bool initWithFile(const char *plistFile);

    /** initializes a CCQuadParticleSystem from a CCDictionary.
    @since v0.99.3
    */
    bool initWithDictionary(CCDictionary *dictionary);

    //! Initializes a system with a fixed number of particles
    virtual bool initWithTotalParticles(unsigned int numberOfParticles);
    //! Add a particle to the emitter
    bool addParticle();
    //! Initializes a particle
    virtual void initParticle(tCCParticle* particle);
    //! stop emitting particles. Running particles will continue to run until they die
    void stopSystem();
    //! Kill all living particles.
    void resetSystem();
    //! whether or not the system is full
    bool isFull();

    //! should be overridden by subclasses
    virtual void updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition);
    //! should be overridden by subclasses
    virtual void postStep();

    virtual void update(float dt);
    virtual void updateWithNoTime(void);
protected:
    virtual void updateBlendFunc();
};

// end of particle_nodes group
/// @}

NS_CC_END

#endif //__CCPARTICLE_SYSTEM_H__
