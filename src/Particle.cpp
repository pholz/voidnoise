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

#include <vector>

#define HISTORY_SIZE 6

#define CHKGLERR(x) { GLenum err = glGetError(); console() << "GL ERR  " << (int)err << " at " << (x) << std::endl; }

using namespace ci;
using namespace ci::app;

Particle::Particle(const Vec2f& loc) :
	m_isDead(false),
	m_lifespan(Rand::randFloat(2000.0, 4000.0)),
	m_frameCtr(0)
{
	m_loc = loc;
	m_vel = Rand::randVec2f() * Rand::randFloat();
	//m_vel = Rand::randFloat();
	m_radius = 10.0f;
	m_acc = Vec2f(0.0, 0.0);
	
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setDynamicPositions();
	
	m_vboMesh = gl::VboMesh::create(HISTORY_SIZE + 1, HISTORY_SIZE + 1, layout, GL_LINE_STRIP);
	std::vector<uint32_t> indices;
	
	for (int i = 0; i < HISTORY_SIZE + 1; i++)
	{
		indices.push_back(i);
	}
	
	m_vboMesh->bufferIndices(indices);
}

Particle::~Particle()
{
	console() << "~PARTICLE" << std::endl;
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
	
	
	
	if (m_frameCtr > 20)
	{
		m_history.push_front(m_loc);
		
//		console() << "pushed " << m_loc;
		
		if (m_history.size() > HISTORY_SIZE)
		{
			m_history.pop_back();
		}
		
		
		
		
		m_frameCtr = 0;
	}
	
	m_frameCtr++;
	
	gl::VboMesh::VertexIter iter = m_vboMesh->mapVertexBuffer();
	iter.setPosition(Vec3f(m_loc));
	for (Vec2f &vec : m_history)
	{
		if (vec.length() == 0.0)
		{
			vec = m_loc;
		}
		++iter;
		iter.setPosition(Vec3f(vec));
	}
	
	if (m_history.size() > 2)
		iter.setPosition(Vec3f(m_history[m_history.size()-2]+
						   (m_history.back()-m_history[m_history.size()-2])*(1.0-(m_frameCtr/20.0))));
	
//	console() << "upd";
	// upd mesh
	
	
}

void Particle::draw()
{
//	console() << "draw";
	
	
	gl::color(1.0f, 0.0f, 0.0f, 1.0f);
	
	Vec2f rv = m_vel.normalized();
	rv.rotate(M_PI_2);
	gl::drawSolidTriangle(m_loc + m_vel.normalized() * m_radius * 2.0f,
					  m_loc + rv * m_radius/2.0f, m_loc - rv * m_radius/2.0f);
	
	gl::color(1.0, 1.0, 1.0, 1.0f);
	
	//CHKGLERR(0)
	gl::lineWidth(voidnoise::Settings::get().stemThickness);
	gl::draw(m_vboMesh);
//	gl::begin(GL_LINE_STRIP);
//	gl::vertex(m_loc);
//	int i = 0;
//	for (Vec2f &vec : m_history)
//	{
//		gl::color(1.0, 1.0, 1.0, (10.0-(i+m_frameCtr/20.0f))/10.0f);
//		gl::vertex(vec);
//		i++;
//	}
//	gl::end();
	//CHKGLERR(1)
//	if (m_history.size() > 0)
//		gl::drawSolidCircle(m_history.back(), m_radius);
//	
	
	
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