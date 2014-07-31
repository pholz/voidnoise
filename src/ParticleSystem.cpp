//
//  ParticleSystem.cpp
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#include "ParticleSystem.h"
#include "cinder/Rand.h"

#define NUM_PARTICLES 300

using namespace ci;

ParticleSystem::ParticleSystem()
{
	
}

void ParticleSystem::applyForce(float zoneRadiusSquared)
{
	for (auto p1 = m_particles.begin(); p1 != m_particles.end(); p1++)
	{
		auto p2 = p1;
		
		for (p2++; p2 != m_particles.end(); p2++)
		{
			Vec2f dir = p1->pos() - p2->pos();
			float distSquared = dir.lengthSquared();
			
			if (distSquared <= 10)
			{
				continue;
			}
			
			if (distSquared <= zoneRadiusSquared)
			{
				float F = (zoneRadiusSquared / distSquared - 1.0f) * 0.01f;
				dir = dir.normalized() * F;
				p1->addAcc(dir);
				p2->addAcc(-dir);
			}
		}
	}
	
}

void ParticleSystem::update()
{
	for (std::list<Particle>::iterator it = m_particles.begin(); it != m_particles.end(); )
	{
		if (it->isDead())
		{
			it = m_particles.erase(it);
		}
		else
		{
			it->update();
			it++;
		}
	}
	
	if (m_particles.size() < NUM_PARTICLES)
	{
		addParticles(NUM_PARTICLES - m_particles.size());
	}
}

void ParticleSystem::draw()
{
	for (Particle &particle : m_particles)
	{
		particle.draw();
	}
}

void ParticleSystem::addParticles(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		float x = Rand::randFloat(app::getWindowWidth())-app::getWindowWidth()/2;
		float y = Rand::randFloat(app::getWindowHeight())-app::getWindowHeight()/2;
		m_particles.push_back(Particle(Vec2f(x, y)));
	}
}

void ParticleSystem::removeParticles(int amount)
{

}