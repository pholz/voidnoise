#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VoidNoiseApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void VoidNoiseApp::setup()
{
}

void VoidNoiseApp::mouseDown( MouseEvent event )
{
}

void VoidNoiseApp::update()
{
}

void VoidNoiseApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( VoidNoiseApp, RendererGl )
