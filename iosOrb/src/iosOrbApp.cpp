#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Orb.h"
#include "cinder/Camera.h"
#include "cinder/gl/Light.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class iosOrbApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	Orb*						mOrb;
	cinder::CameraPersp			mCam;
	cinder::Vec3f				mEye, mCenter, mUp;
	cinder::Vec3f				mRotation;
	float						mScale;
};

void iosOrbApp::setup()
{
	mOrb = new Orb( 1, 100.0 );
	mEye        = Vec3f( 0.0f, 0.0f, 500.0f );
	mCenter     = Vec3f( 0.0f, 0.0f, 0.0f );
	mUp         = Vec3f::yAxis();
	mCam.lookAt( mEye, mCenter, mUp );
	mScale = 150.0;
	
//	mRotation = Vec3d( 2.0, 20.0, 8.0 );
}

void iosOrbApp::mouseDown( MouseEvent event )
{
}

void iosOrbApp::update()
{
	mRotation += Vec3d( 0.0, 1.0, 0.0 );
	mOrb->update();
}

void iosOrbApp::draw()
{
	gl::clear( Color( 0.0, 0.0, 0.0 ) );
	
	mCam.lookAt( mEye, mCenter, mUp );
	mCam.setPerspective( 60, getWindowAspectRatio(), 1, 10000.0f );
	gl::setMatrices( mCam );
	
	gl::enableDepthRead();
	
	glEnable( GL_LIGHTING );
	
	// -- lighting stuff --
	// setup light 0
	gl::Light light(gl::Light::DIRECTIONAL, 0);
	light.lookAt( Vec3f( 0.0f, 200.0f, 1500.0f ), Vec3f( 0.0f, 0.0f, 50.0f ) );
	light.enable();
	// -- end lighting stuff --
	
	glPushMatrix();
	gl::rotate( mRotation );
	
	mOrb->setColor( Color( 0.6, 0.0, 0.0 ) );
	//	gl::enableWireframe();
	mOrb->draw();
	
		
	// wireframe
	/*gl::enableWireframe();
	glEnable( GL_POLYGON_OFFSET_LINE );
	glPolygonOffset( -1, -1 );
	mOrb->setColor( Color( 1, 0, 0) );
	mOrb->draw();
	glDisable( GL_POLYGON_OFFSET_LINE );
	mOrb->setColor( Color::white() );
	gl::disableWireframe();*/
	
		
	glDisable( GL_LIGHTING );
	glPopMatrix();
	gl::disableDepthRead();
}

CINDER_APP_NATIVE( iosOrbApp, RendererGl )
