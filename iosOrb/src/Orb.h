//
//  Orb.h
//  OrbTool
//
//  Created by Greg Kepler on 12/23/12.
//
//

#pragma once
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Vbo.h"
//#include "cinder/TriMesh.h"

// Object to hold properties for each vertex of the orb
class OrbVertex {
	ci::Vec3f	mVertex;
	float		mLineLen;
	
public:
	OrbVertex( ci::Vec3f vertex );
	~OrbVertex();
	
	
	ci::Vec3f		getVertex(){ return mVertex; };
	float			getLineLen(){ return mLineLen; };
};



class Orb {
	ci::TriMesh						mSphereMesh;
	//std::vector<ci::Vec3f>			mPosCoords;
	std::vector<ci::Vec3f>			mNormals;
	//std::vector<OrbVertex*>			mOrbVertices;
	

	std::vector<int>				mOrbIndices;	// store the indices of each vertex
	std::vector<ci::Vec3f>			mOrbVerts;		// store the vertices that corellate with the indices
	
	std::map<std::string, float>	mFloatMap, mRandMap;
	int								mRes;
	float							mRadius, mCurRadius;
	int								mRandBumpType;
	float							mBumpHeight, mCurBumpHeight;
	int								mRndmSeed;
	bool							mMoving, mFlatShading;
	float							mMovingRange;
	float							mBumpSpeed;
	ci::Color						mColor;
	
	ci::TriMesh						mesh;
	
	int							addVertex( ci::Vec3f );
	//void							addVertex( ci::Vec3f );
	void							drawPyramid();
	
public:
	Orb();
	Orb(int scale, float radius);
	
	void		init();
	void		createSphere( int res );
	//void		drawSphereTri( ci::Vec3f va, ci::Vec3f vb, ci::Vec3f vc, int div, float order );
	void		drawSphereTri( int ia, int ib, int ic, int div );
	std::string	getString( ci::Vec3f vec );
	void		update( int scale, float radius, int randBumpType, float bumpHeight );
	void		update();
	void		draw();
	void		drawLines();
	void		drawSphere();
	
	void		setMoving( bool moving, float movingRange, float bumpSpeed );
	void		setColor( ci::Color color );
	void		setFlatShading( bool shading );
	void		updateBumps();
};


