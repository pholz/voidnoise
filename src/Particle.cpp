//
//  Particle.cpp
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#include "Particle.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

Particle::Particle(const Vec2f& loc) :
	m_isDead(false),
	m_lifespan(Rand::randFloat(100.0, 500.0))
{
	m_loc = loc;
	m_vel = Rand::randVec2f() * Rand::randFloat();
	//m_vel = Rand::randFloat();
	m_radius = 5.0f;
	m_acc = Vec2f(0.0, 0.0);
	m_decay = 0.7;
}

void Particle::addAcc(const Vec2f &acc)
{
	m_acc += acc;
}

bool Particle::isDead()
{
	return m_isDead;
}


void Particle::update()
{
	pullToCenter(Vec2f(0.0, 0.0));
	m_vel += m_acc;
	m_loc += m_vel;
	m_vel *= m_decay;
	
	static const float maxSpeedSq = .1;
	float velLengthSqrd = m_vel.lengthSquared();
	if( velLengthSqrd > maxSpeedSq) {
		m_vel.normalize();
		m_vel *= maxSpeedSq;
	}
	
	m_age += 1.0;
	if (m_age >= m_lifespan)
	{
		m_isDead = true;
	}
}

void Particle::draw()
{
	gl::color(1.0f, 1.0f, 1.0f, 1.0f-m_age/m_lifespan);
	gl::drawSolidCircle(m_loc, m_radius * m_age/m_lifespan);
	gl::color(1.0f, 1.0f, 1.0f, 1.0f);
}

void Particle::pullToCenter(const Vec2f &center)
{
	Vec2f dirToCenter = m_loc - center;
	float distToCenter = dirToCenter.length();
	static const float maxDistance = 300.0f;
	
	if (distToCenter > maxDistance)
	{
		static const float pullStrength = 0.0001f;
		m_vel -= dirToCenter.normalized() * ((distToCenter - maxDistance) * pullStrength);
	}
}