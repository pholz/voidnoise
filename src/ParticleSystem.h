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

using namespace ci;

class ParticleSystem
{
public:
	ParticleSystem();
	void update();
	void draw();
	void addParticles(int amount);
	void removeParticles(int amount);
	
	void applyForce(float zoneRadiusSquared);
	
private:
	std::list<Particle> m_particles;
};

#endif /* defined(__VoidNoise__ParticleSystem__) */