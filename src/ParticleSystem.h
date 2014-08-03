//
//  ParticleSystem.h
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#ifndef __VoidNoise__ParticleSystem__
#define __VoidNoise__ParticleSystem__

#include <iostream>
#include <list>
#include "Particle.h"
#include "VoidGlobals.h"

using namespace ci;

class ParticleSystem
{
public:
	ParticleSystem();
	void update();
	void draw();
	void addParticles(int amount);
	void removeParticles(int amount);
	void setRepulsionPos(bool on, const Vec2i &pos = Vec2f());
	
	void applyForce(float zoneRadiusSquared, float separationThresh, float alignmentThresh);
	
private:
	std::list<Particle> m_particles;
	Vec2f m_repulsionPos;
	bool m_doRepulsion;
};

#endif /* defined(__VoidNoise__ParticleSystem__) */
