#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "ParticleSystem.h"
#include "cinderSyphon.h"

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
	
	syphonServer m_srvSyphon;
	gl::Fbo m_fbo;
	gl::TextureRef m_syTex;
	
	bool m_doSyphon;
	
};

void VoidNoiseApp::setup()
{
	setWindowSize(1000, 1000);
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
	m_doSyphon = false;
	
	voidnoise::Settings::get().attractStrength = 0.1f;
	voidnoise::Settings::get().repelStrength = 0.05f;
	voidnoise::Settings::get().orientStrength = 0.1f;
	voidnoise::Settings::get().gravity = 0.04f;
	voidnoise::Settings::get().gravityDistance = 800.0f;
	voidnoise::Settings::get().maxSpeed = 20.0f;
	voidnoise::Settings::get().minSpeed = 10.0f;
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
	
	m_params->addParam("syphon", &m_doSyphon);
	
	//m_particleSystem.addParticles(500);
	
	m_srvSyphon.setName("voidnoise");
	
	gl::Fbo::Format format2;
	format2.enableDepthBuffer(true);
	format2.enableColorBuffer(true);
	format2.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	format2.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
	format2.setWrap(GL_CLAMP, GL_CLAMP);
	format2.setColorInternalFormat(GL_RGBA16F_ARB);
	format2.setSamples(4);
	
	m_fbo = gl::Fbo(REGIONSIZE, REGIONSIZE, format2);
	m_syTex = gl::Texture::create(REGIONSIZE, REGIONSIZE);
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
	bool syphon = m_doSyphon;
	
	if (syphon)
	{
		gl::setMatricesWindow(m_fbo.getSize());
		gl::setViewport(m_fbo.getBounds());
		gl::translate(m_fbo.getWidth()/2, m_fbo.getHeight()/2);
		gl::scale(0.5, 0.5);
		m_fbo.bindFramebuffer();
	}
	else
	{
		m_cam.setEyePoint(m_eye);
		m_cam.lookAt(m_eye, m_center, m_up);
		gl::setMatrices(m_cam);
	}
	gl::enableAlphaBlending();
	
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	gl::rotate(m_sceneRotation);
	
	m_particleSystem.draw();
	
//	m_srvSyphon.publishScreen();
	
	if (syphon)
	{
		m_fbo.unbindFramebuffer();
		*m_syTex = m_fbo.getTexture();
		m_srvSyphon.publishTexture(m_syTex);
//		m_fbo.blitToScreen(m_fbo.getBounds(), getWindowBounds());
	}
	
	m_params->draw();
}

CINDER_APP_NATIVE( VoidNoiseApp, RendererGl )
