//
//  ParticleSystem.cpp
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#include "ParticleSystem.h"
#include "cinder/Rand.h"

#define NUM_PARTICLES 500

using namespace ci;

ParticleSystem::ParticleSystem() :
	m_doRepulsion(false)
{
	
}

void ParticleSystem::setRepulsionPos(bool on, const Vec2i &pos)
{
	m_doRepulsion = on;
	
	if (on)
	{
		m_repulsionPos.x = (float(pos.x)/float(app::getWindowWidth()) - 0.5f) * REGIONSIZE * 2.0;
		m_repulsionPos.y = -(float(pos.y)/float(app::getWindowHeight()) - 0.5f) * REGIONSIZE * 2.0;
	}
}

void ParticleSystem::applyForce(float zoneRadiusSquared, float separationThresh, float alignmentThresh)
{
	for (auto p1 = m_particles.begin(); p1 != m_particles.end(); p1++)
	{
		auto p2 = p1;
		
		for (p2++; p2 != m_particles.end(); p2++)
		{
			Vec2f dir = p1->pos() - p2->pos();
			float distSquared = dir.lengthSquared();
			
			if (distSquared <= 1)
			{
				continue;
			}
			
			if (distSquared <= zoneRadiusSquared)
			{
				float ratio = distSquared / zoneRadiusSquared;
				
				if (ratio < separationThresh) // separate
				{
					float F = (separationThresh / ratio - 1.0f) * voidnoise::Settings::get().repelStrength;
					dir = dir.normalized() * F;
					p1->addAcc(dir);
					p2->addAcc(-dir);
				}
				else if (ratio < alignmentThresh)
				{
					float threshDelta = alignmentThresh - separationThresh;
					float adjustedRatio = (ratio - separationThresh) / threshDelta;
					float F = (0.5f - cos(adjustedRatio * M_PI * 2.0f) * 0.5f + 0.5f) * voidnoise::Settings::get().orientStrength;
					p1->addAcc(p2->vel().normalized() * F);
					p2->addAcc(p1->vel().normalized() * F);
				}
				else // attract
				{
					float threshDelta = 1.0f - alignmentThresh;
					float adjustedRatio = (ratio - alignmentThresh) / threshDelta;
					float F = (0.5f - (cos(adjustedRatio * M_PI * 2.0f) * 0.5f + 0.5f)) * voidnoise::Settings::get().attractStrength;
					dir = dir.normalized() * F;
					p1->addAcc(-dir);
					p2->addAcc(dir);
				}
				
			}
		}
		
		if (m_doRepulsion)
		{
			float dsqrd = p1->pos().distanceSquared(m_repulsionPos);
			if (dsqrd < voidnoise::Settings::get().repulsionDist * voidnoise::Settings::get().repulsionDist)
			{
				p1->addAcc((p1->pos()-m_repulsionPos) * voidnoise::Settings::get().repulsionStrength
						   * (1.0f/(dsqrd+0.1f)));
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