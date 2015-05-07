#ifndef Joint_H
#define Joint_H

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <GL/glut.h>
#include <vector>
#include "color.h"

class Joint
{
public:
  
    Joint(float R, float G, float B)
    {
        mObj = gluNewQuadric();
        gluQuadricDrawStyle(mObj, GLU_FILL);
        gluQuadricOrientation(mObj, GLU_OUTSIDE);
        gluQuadricNormals(mObj, GLU_SMOOTH);
		mR = R;
		mG = G;
		mB = B;
    }
    
    float mLength;
    float mAngle;
    GLUquadricObj *mObj;
    float mR, mG, mB;
};

#endif // Joint_H
