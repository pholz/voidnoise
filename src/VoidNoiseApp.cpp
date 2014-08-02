#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "ParticleSystem.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VoidNoiseApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	
	CameraPersp m_cam;
	Vec3f m_eye, m_center, m_up;
	params::InterfaceGlRef m_params;
	Quatf m_sceneRotation;
	float m_camDistance;
	
	ParticleSystem m_particleSystem;
	
	float m_pSysZoneRadiusSq;
	float m_pSysSeparationThresh;
	float m_pSysAlignmentThresh;
	
};

void VoidNoiseApp::setup()
{
	m_cam.setPerspective(60.0f, getWindowAspectRatio(), 5.0f, 3000.0f);
	m_camDistance = 500.0f;
	m_center = Vec3f::zero();
	m_up = Vec3f::yAxis();
	m_pSysZoneRadiusSq = 450.0;
	m_pSysSeparationThresh = 0.4;
	m_pSysAlignmentThresh = 0.65;
	
	m_params = params::InterfaceGl::create(getWindow(), "Flocking", Vec2i(225, 200));
	m_params->addParam("Scene Rotation", &m_sceneRotation);
	m_params->addParam("Eye Distance", &m_camDistance, "min=50.0 max=1000.0 step=50.0 keyIncr=s keyDecr=w" );
	m_params->addParam("Zone Radius Sq", &m_pSysZoneRadiusSq);
	m_params->addParam("Separation Thresh", &m_pSysSeparationThresh);
	m_params->addParam("Alignment Thresh", &m_pSysAlignmentThresh);
	
	m_particleSystem.addParticles(50);

}

void VoidNoiseApp::mouseDown( MouseEvent event )
{
}

void VoidNoiseApp::update()
{
	m_eye = Vec3f(.0f, .0f, m_camDistance);
	
	m_particleSystem.update();
	m_particleSystem.applyForce(m_pSysZoneRadiusSq, m_pSysSeparationThresh, m_pSysAlignmentThresh);
}

void VoidNoiseApp::draw()
{
	gl::enableAlphaBlending();
	
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	m_cam.lookAt(m_eye, m_center, m_up);
	gl::setMatrices(m_cam);
	
	gl::rotate(m_sceneRotation);
	
	m_particleSystem.draw();
	
	m_params->draw();
}

CINDER_APP_NATIVE( VoidNoiseApp, RendererGl )
