//
//  ParticleLogic.h
//  gtf
//
//  Created by David Gallardo on 12/06/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFRHI.h"

enum class EmitterType : unsigned char
{
    ET_GRAVITY,
    ET_RADIAL
};

struct Particle
{
    float position[2];
	float direction[2];
    float startPos[2];
	float color[4];
	float deltaColor[4];
    float rotation;
    float rotationDelta;
    float radialAcceleration;
    float tangentialAcceleration;
	float radius;
	float radiusDelta;
	float angle;
	float degreesPerSecond;
	float particleSize;
	float particleSizeDelta;
	float timeToLive;
    
    //frame animation
    unsigned int frame;
    float timeInFrame;
};

class ParticleEmitter
{
public:
    EmitterType emitterType;
    GTFRHITexture2DPtr texture;
    float frameSize[2];
    float animationTime;
    
	float sourcePosition[2], sourcePositionVariance[2];
	float angle, angleVariance;
	float speed, speedVariance;
    float radialAcceleration, tangentialAcceleration;
    float radialAccelVariance, tangentialAccelVariance;
	float gravity[2];
	float particleLifespan, particleLifespanVariance;
	float startColor[4], startColorVariance[4];
	float finishColor[4], finishColorVariance[4];
	float startParticleSize, startParticleSizeVariance;
	float finishParticleSize, finishParticleSizeVariance;
	unsigned int maxParticles;
	int particleCount;
	float emissionRate;
	float emitCounter;
	float elapsedTime;
	float duration;
    float rotationStart, rotationStartVariance;
    float rotationEnd, rotationEndVariance;
    
    EGTFRHIBlendFnc blendFuncSource, blendFuncDestination;
    bool _opacityModifyRGB;
    
    //////////////////// Particle ivars only used when a maxRadius value is provided.  These values are used for
	//////////////////// the special purpose of creating the spinning portal emitter
	float maxRadius;						// Max radius at which particles are drawn when rotating
	float maxRadiusVariance;				// Variance of the maxRadius
	float radiusSpeed;					// The speed at which a particle moves from maxRadius to minRadius
	float minRadius;						// Radius from source below which a particle dies
	float minRadiusVariance;				// Variance of the minRadius
	float rotatePerSecond;				// Numeber of degress to rotate a particle around the source pos per second
	float rotatePerSecondVariance;		// Variance in degrees for rotatePerSecond

    void update(float deltaTime);
    void render();
private:
    void addParticle();
    void initParticle();
};

