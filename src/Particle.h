//
//  Particle.h
//  VoidNoise
//
//  Created by Peter Holzkorn on 31/07/2014.
//
//

#ifndef __VoidNoise__Particle__
#define __VoidNoise__Particle__

#include <iostream>
#include <deque>
#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

using namespace ci;
using namespace ci::app;

class Particle
{
public:
	Particle(const Vec2f &loc);
	virtual ~Particle();
	void update();
	void draw();
	bool isDead();
	Vec2f pos() { return m_loc; }
	Vec2f vel() { return m_vel; }
	void addAcc(const Vec2f &acc);
	
	void pullToCenter(const Vec2f &center);
	
private:
	Vec2f m_loc;
	//Vec2f m_dir;
	Vec2f m_vel;
	float m_radius;
	Vec2f m_acc;
	float m_decay;
	
	double m_age;
	double m_lifespan;
	bool m_isDead;
	
	std::deque<Vec2f> m_history;
	int m_frameCtr;
	
	gl::VboMeshRef m_vboMesh;
};

#endif /* defined(__VoidNoise__Particle__) */
