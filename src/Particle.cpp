//
//  Particle.cpp
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#include "Particle.h"
#include "cinder/Rand.h"

#include "VoidGlobals.h"

using namespace ci;
using namespace ci::app;

Particle::Particle(const Vec2f& loc) :
	m_isDead(false),
	m_lifespan(Rand::randFloat(2000.0, 4000.0))
{
	m_loc = loc;
	m_vel = Rand::randVec2f() * Rand::randFloat();
	//m_vel = Rand::randFloat();
	m_radius = 10.0f;
	m_acc = Vec2f(0.0, 0.0);
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
	m_vel += m_acc;
	
	float maxSpeed = voidnoise::Settings::get().maxSpeed;
	float maxSpeedSq = maxSpeed * maxSpeed;
	float minSpeed = voidnoise::Settings::get().minSpeed;
	float minSpeedSq = minSpeed * minSpeed;
	float velLengthSqrd = m_vel.lengthSquared();
	if( velLengthSqrd > maxSpeedSq) {
		m_vel.normalize();
		m_vel *= maxSpeed;
	}
	else if (velLengthSqrd < minSpeedSq) {
		m_vel.normalize();
		m_vel *= minSpeed;
	}
	
	m_loc += m_vel;
	m_vel *= voidnoise::Settings::get().decay;
	m_acc *= voidnoise::Settings::get().decay;
	
	pullToCenter(Vec2f(0.0, 0.0));
	
	m_age += 1.0;
	if (m_age >= m_lifespan)
	{
		m_isDead = true;
	}
}

void Particle::draw()
{
	gl::color(1.0f, 1.0f, 1.0f, fmax(0.5f, 1.0f-m_age/m_lifespan));
	//gl::drawSolidCircle(m_loc, m_radius * fmax(0.5f, m_age/m_lifespan));
	Vec2f rv = m_vel.normalized();
	rv.rotate(M_PI_2);
	gl::drawSolidTriangle(m_loc + m_vel.normalized() * m_radius * 2.0f,
						  m_loc + rv * m_radius/2.0f, m_loc - rv * m_radius/2.0f);
	gl::color(1.0f, 1.0f, 1.0f, 1.0f);
}

void Particle::pullToCenter(const Vec2f &center)
{
	Vec2f dirToCenter = m_loc - center;
	float distToCenter = dirToCenter.length();
	
	if (distToCenter > voidnoise::Settings::get().gravityDistance)
	{
		m_acc -= dirToCenter.normalized() *
			((distToCenter - voidnoise::Settings::get().gravityDistance) * voidnoise::Settings::get().gravity);
	}
}