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
	void mouseUp( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void update();
	void draw();
		
	CameraOrtho m_cam;
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
	setFrameRate(30.0f);
	
//	m_cam.setPerspective(60.0f, getWindowAspectRatio(), 5.0f, 3000.0f);
	static const float sz = REGIONSIZE;
	m_cam.setOrtho(-sz, sz, -sz, sz, 5.0f, 3000.0f);
	m_camDistance = 1000.0f;
	m_center = Vec3f::zero();
	m_up = Vec3f::yAxis();
	m_pSysZoneRadiusSq = 7000.0;
	m_pSysSeparationThresh = 0.2;
	m_pSysAlignmentThresh = 0.7;
	
	voidnoise::Settings::get().attractStrength = 0.1f;
	voidnoise::Settings::get().repelStrength = 0.05f;
	voidnoise::Settings::get().orientStrength = 0.1f;
	voidnoise::Settings::get().gravity = 0.04f;
	voidnoise::Settings::get().gravityDistance = 300.0f;
	voidnoise::Settings::get().maxSpeed = 20.0f;
	voidnoise::Settings::get().minSpeed = 0.4f;
	voidnoise::Settings::get().decay = 0.001f;
	voidnoise::Settings::get().repulsionDist = 200.0f;
	voidnoise::Settings::get().repulsionStrength = 3000.0f;
	voidnoise::Settings::get().stemThickness = 1.0f;
	
	m_params = params::InterfaceGl::create(getWindow(), "Flocking", Vec2i(225, 300));
	m_params->addParam("Scene Rotation", &m_sceneRotation);
	m_params->addParam("Eye Distance", &m_camDistance, "min=50.0 max=1000.0 step=50.0 keyIncr=s keyDecr=w" );
	m_params->addParam("Zone Radius Sq", &m_pSysZoneRadiusSq);
	m_params->addParam("Separation Thresh", &m_pSysSeparationThresh);
	m_params->addParam("Alignment Thresh", &m_pSysAlignmentThresh);
	
	m_params->addParam("attract strength", &voidnoise::Settings::get().attractStrength);
	m_params->addParam("repel strength", &voidnoise::Settings::get().repelStrength);
	m_params->addParam("orient strength", &voidnoise::Settings::get().orientStrength);
	m_params->addParam("max speed", &voidnoise::Settings::get().maxSpeed);
	m_params->addParam("min speed", &voidnoise::Settings::get().minSpeed);
	m_params->addParam("gravity", &voidnoise::Settings::get().gravity);
	m_params->addParam("gravity dist", &voidnoise::Settings::get().gravityDistance);
	m_params->addParam("decay", &voidnoise::Settings::get().decay);
	m_params->addParam("repuls dist", &voidnoise::Settings::get().repulsionDist);
	m_params->addParam("repuls str", &voidnoise::Settings::get().repulsionStrength);
	m_params->addParam("stem thickness", &voidnoise::Settings::get().stemThickness);
	
	m_particleSystem.addParticles(50);

}

void VoidNoiseApp::mouseDown( MouseEvent event )
{
	m_particleSystem.setRepulsionPos(true, event.getPos());
}

void VoidNoiseApp::mouseDrag( MouseEvent event )
{
	m_particleSystem.setRepulsionPos(true, event.getPos());
}

void VoidNoiseApp::mouseUp( MouseEvent event )
{
	m_particleSystem.setRepulsionPos(false);
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
	
	m_cam.setEyePoint(m_eye);
	m_cam.lookAt(m_eye, m_center, m_up);
	gl::setMatrices(m_cam);
	
	gl::rotate(m_sceneRotation);
	
	m_particleSystem.draw();
	
	m_params->draw();
}

CINDER_APP_NATIVE( VoidNoiseApp, RendererGl )
