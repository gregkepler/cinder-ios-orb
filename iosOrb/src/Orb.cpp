//
//  Orb.cpp
//  OrbTool
//
//  Created by Greg Kepler on 12/23/12.
//
//

#include "cinder/gl/gl.h"
#include "cinder/TriMesh.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"

#include "Orb.h"

using namespace std;
using namespace ci;
using namespace ci::app;

static float X = 0.525731112119133606f;
static float Z = 0.850650808352039932f;

static Vec3f verts[12] = {
	Vec3f( -X, 0.0f, Z ),	Vec3f( X, 0.0f, Z ),	Vec3f( -X, 0.0f, -Z ),	Vec3f( X, 0.0f, -Z ),
	Vec3f( 0.0f, Z, X ),	Vec3f( 0.0f, Z, -X ),	Vec3f( 0.0f, -Z, X ),	Vec3f( 0.0f, -Z, -X ),
	Vec3f( Z, X, 0.0f ),	Vec3f( -Z, X, 0.0f ),	Vec3f( Z, -X, 0.0f ),	Vec3f( -Z, -X, 0.0f )};


static GLuint triIndices[20][3] = {
	{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1}, {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
	{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

Orb::Orb():
mRes(0), mRadius(100.0), mRandBumpType(4), mFlatShading( true )
{
	init();
}

Orb::Orb( int scale, float radius ):
mRes(scale), mRadius(radius), mRandBumpType(0), mColor( Color::white() ), mFlatShading( true )
{
	init();
}


void Orb::init()
{
	mBumpHeight = 0;
	mCurRadius = mRadius;
	mCurBumpHeight = mBumpHeight = 1.0;
	mMoving = true;
	mBumpSpeed = 0.1;
	createSphere( mRes );
}


void Orb::createSphere( int res )
{
	// clear orb vertices
	mOrbVerts.clear();
	mOrbIndices.clear();
	mNormals.clear();
	
	
	// create the vertex array based on the predefined verts
	for( int i=0; i < 12; i++)
	{
		mOrbVerts.push_back(verts[i]);
	}
	
	for( int i=0; i<20; i++ ){
		Vec3f vert1 = verts[triIndices[i][0]];
		Vec3f vert2 = verts[triIndices[i][1]];
		Vec3f vert3 = verts[triIndices[i][2]];
		
		int i1 = addVertex(vert1);
		int i2 = addVertex(vert2);
		int i3 = addVertex(vert3);
		
		drawSphereTri( i1, i2, i3, res );
	}
	
	updateBumps();
	drawSphere();
	
}

void Orb::drawSphere()
{
	// draw sphere mesh based on those points
	mSphereMesh.clear();
	mNormals.clear();
	for( int i = 0; i < mOrbIndices.size(); i+=3 )
	{
		Vec3f p0 = ( mOrbVerts[mOrbIndices[i]] );
		Vec3f p1 = ( mOrbVerts[mOrbIndices[i+1]] );
		Vec3f p2 = ( mOrbVerts[mOrbIndices[i+2]] );
		
		Vec3f e0 = (p2 - p0);
		Vec3f e1 = (p2 - p1);
		Vec3f n = e0.cross(e1);
		n *= 200.0;
		
		mNormals[ i ] = n;
		mNormals[ i + 1 ] = n;
		mNormals[ i + 2 ] = n;
		
		mSphereMesh.appendVertex( p0 );
		mSphereMesh.appendVertex( p1 );
		mSphereMesh.appendVertex( p2 );
		
		mSphereMesh.appendNormal( n );
		mSphereMesh.appendNormal( n );
		mSphereMesh.appendNormal( n );
		
		int vert0 = mSphereMesh.getNumVertices() - 3;
		int vert1 = mSphereMesh.getNumVertices() - 2;
		int vert2 = mSphereMesh.getNumVertices() - 1;
		mSphereMesh.appendTriangle( vert0, vert1, vert2 );
	}
}

void Orb::updateBumps()
{
	Rand::randSeed( 5 );
	
	//float rand = randFloat( 1, mCurBumpHeight );
	
	float high = 0.0;
	float low = 10.0;
	float freq = 0.1f;
	float range = 0.1f;
	float amp = 8.0;
	
	// go though all the points and adjust the points
	for( int i = 0; i < mOrbVerts.size(); i++ )
	{
		//		mOrbVerts[i] *= randFloat(1.0, 1.5); //random
		Vec3f pt = mOrbVerts[i];
//		float dist = pt.distance(Vec3f(randFloat(-1.0, 1.0), randFloat(-1.0, 1.0), randFloat(-1.0, 1.0) ) );
		float dist = pt.distance( Vec3f( 0.0, -1.0, 0.0 ) );
//		float dist = pt.distance( Vec3f( 0.0, sin( ( getElapsedFrames() * 0.1 ) ), 0.0 ) );
//		float dist = pt.distance( Vec3f( 0.0, 0.0, -1.0 ) );
//		float dist = pt.distance( Vec3f( -1.0, 0.0, 0.0 ) );
		
		dist = lmap(dist, 0.0f, 2.0f, 0.0f, 1.0f);
		
		float r = lmap( sin( (getElapsedFrames() * freq) + (dist * amp) ) , -1.0f, 1.0f, 1.0f, 1.0f + range);
		
		if(r > high) high = r;
		if(r < low ) low = r;

		mOrbVerts[i] *= r;
	}
	console() << "high: " << high << " low: " << low << endl;
	console() << "---" << endl;
	
	
}


int Orb::addVertex( Vec3f v )
{
	bool foundVert = false;
	int index;
	for( int i = 0; i < mOrbVerts.size(); ++i ){
		if( mOrbVerts[i].distance(v) == 0){
			foundVert = true;
			index = i;
			break;
		}
	}
	
	if(foundVert){
		return index;
	}else{
		mOrbVerts.push_back( v );
		return mOrbVerts.size() - 1;
	}
}


void Orb::drawSphereTri( int ia, int ib, int ic, int div )
{
	Vec3f va = mOrbVerts[ia];
	Vec3f vb = mOrbVerts[ib];
	Vec3f vc = mOrbVerts[ic];
	
	if( div <= 0 )
	{
//		if( !mFlatShading ){
			mNormals.push_back( va );
			mNormals.push_back( vb );
			mNormals.push_back( vc );
//		}
		
		// add vert indices to mOrnIndices
		mOrbIndices.push_back(ia);
		mOrbIndices.push_back(ib);
		mOrbIndices.push_back(ic);
		
	}
	else
	{
		Vec3f vab = ( ( va + vb ) * 0.5f ).normalized();
		Vec3f vac = ( ( va + vc ) * 0.5f ).normalized();
		Vec3f vbc = ( ( vb + vc ) * 0.5f ).normalized();
		
		// add these new point to the vertex array
		int iab = addVertex( vab );
		int iac = addVertex( vac );
		int ibc = addVertex( vbc );
		
		// add their index to the index array
		//		mOrbIndices.push_back( orig index 1, i1, i2 );
		//		mOrbIndices.push_back( orig index 2, i3, i1 );
		//		mOrbIndices.push_back( orig index 3, i2, i3 );
		//		mOrbIndices.push_back( i1, i2, i3 );
		
		//each triangle is split up into 4 triangles
		drawSphereTri( ia, iab, iac, div-1 );
		drawSphereTri( ib, ibc, iab, div-1 );
		drawSphereTri( ic, iac, ibc, div-1 );
		drawSphereTri( iab, ibc, iac, div-1 );
	}
}


string Orb::getString( Vec3f vec)
{
	std::string ss;
	for(size_t i = 0; i < 3; ++i)
	{
		if(i != 0)
			ss += ",";
		ss += std::to_string(vec[i]);
	}
	return ss;
}

void Orb::update()
{
	update( mRes, mRadius, mRandBumpType, mBumpHeight);
}

void Orb::update( int scale, float radius, int randBumpType, float bumpHeight )
{
	bool changed = false;
	
	if( mRandBumpType != randBumpType )
	{
		mRandBumpType = randBumpType;
		mRndmSeed = randInt();
		changed = true;
	}
	
	if(mRes != scale || mRadius != radius ||
	   mBumpHeight != bumpHeight )
	{
		mRes = scale;
		mRadius = radius;
		
		mBumpHeight = bumpHeight;
		changed = true;
		
	}
	
	if( mCurRadius != radius || mCurBumpHeight != bumpHeight )
	{
		changed = true;
	}
	
	if(mMoving)
	{
		//		changed = true;
	}
	
	
	mCurRadius += (mRadius - mCurRadius) * .3;
	if( abs(mRadius - mCurRadius) < .01) mCurRadius = mRadius;
	
	mCurBumpHeight += ( mBumpHeight - mCurBumpHeight ) * .3;
	if( abs(mBumpHeight - mCurBumpHeight) < .01) mCurBumpHeight = mBumpHeight;
	
	
	changed = true;
	
	if(changed)
	{
		mFloatMap.clear();
		mRandMap.clear();
		
		createSphere(mRes);
	}
}


void Orb::setMoving( bool moving, float movingRange, float bumpSpeed )
{
	if( moving != mMoving )
	{
		mMoving = moving;
		mFloatMap.clear();
		mRandMap.clear();
		createSphere(mRes);
	}
	
	if( movingRange != !mMovingRange )
	{
		mMovingRange = movingRange;
	}
	
	if( bumpSpeed != !mBumpSpeed )
	{
		mBumpSpeed = bumpSpeed;
	}
}

void Orb::setColor( Color color )
{
	if( mColor != color )
	{
		mColor = color;
	}
}

void Orb::setFlatShading( bool shading )
{
	if( shading != mFlatShading ){
		mFlatShading = shading;
		mFloatMap.clear();
		mRandMap.clear();
		createSphere(mRes);
	}
}

void Orb::draw()
{
	glLightfv( GL_LIGHT0, GL_AMBIENT, mColor );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, mColor );
	glLightfv( GL_LIGHT0, GL_SPECULAR, Color::white() );
	
	glPushMatrix();
	gl::scale( mCurRadius, mCurRadius, mCurRadius );
	gl::draw(mSphereMesh);
	glPopMatrix();
	
	//	gl::drawSphere(Vec3f(0, 0, 0), 100);
	//	gl::drawCube( Vec3f(0, 0, 0), Vec3f(100, 100, 100));
}


void Orb::drawLines()
{
	console() << "verts: "<< mOrbVerts.size() << endl;
	for( int x = 0; x < mOrbVerts.size(); ++x )
	{
		//OrbVertex *mOrbVertex = mOrbVertices[x];
		//Vec3f pt = mOrbVertex->getVertex();
		Vec3f pt = mOrbVerts[x];
		float len = 1.0;
		//		float len = 1;
		len *= 100;
		
		pt.normalize();
		pt *= Vec3f( len, len, len );
		
		//		Vec3f startPt = Vec3f::zero() * Vec3f( mRadius, mRadius, mRadius );
		gl::drawLine( pt, pt + pt );
		
	}	
}



OrbVertex::OrbVertex( ci::Vec3f vertex )
{
	mVertex = vertex;
	mLineLen = randFloat( 0.0, 1.0 );
}

OrbVertex::~OrbVertex()
{
	
}

