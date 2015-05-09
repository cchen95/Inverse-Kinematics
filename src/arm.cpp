#include "Arm.h"
#include <cmath>
#include <iostream>
#include "joint.h"


MatrixXf Arm::pseudoInverse()
{
    MatrixXf j = jacobian();
    MatrixXf inv = (j * j.transpose());
    inv = inv.inverse();

    return (j.transpose() * inv);
}

void Arm::calculatePosition()
{
    float angle = 0;
    mPosition = VectorXf::Zero(2,1);

    for (int i = 0; i < mList.size(); i++)
    {
        angle += mList[i]->mAngle;
        mPosition(0) += mList[i]->mLength*cos(angle);
        mPosition(1) += -mList[i]->mLength*sin(angle);
    }
}

MatrixXf Arm::jacobian()
{
    MatrixXf j = MatrixXf::Zero(2,mList.size());

    for (int col = 0; col < mList.size(); col++)
    {
        float angle = 0;
        for (int i = 0; i < mList.size(); i++)
        {
            angle += mList[i]->mAngle;
            if (i >= col)
            {
                j(0, col) += (-mList[i]->mLength*sin(angle));
                j(1, col) += (mList[i]->mLength*cos(angle));
            }
        }
    }
    
    return j;
}

//move to target
void Arm::moveToPoint(const VectorXf position)
{
  mTargetPosition = position;
  mResolveTarget = true;
}

void Arm::draw()
{
    glPushMatrix();
    glTranslatef(mBasePosition(0),mBasePosition(1),0.0f);

	//draw a ball at root
	glColor3f(0,1.0,0);
    glutSolidSphere(2.0, 10, 10);

	//draws a cone for each joint and then a sphere where joints connect
    for (int i = 0; i < mList.size(); i++)
    {
        glRotatef((mList[i]->mAngle)*180/3.141592654, 1.0f, 0.0f, 0.0f);
		glColor3f(mList[i]->mR, mList[i]->mG, mList[i]->mB);
		
        //gluCylinder(mList[i]->mObj, 1, 1, mList[i]->mLength, 20, 20);
		glutSolidCone(2, mList[i]->mLength, 20, 20);
        glTranslatef(0, 0, mList[i]->mLength);
		glColor3f(1.0, 0, 0);
        glutSolidSphere(2, 20, 20);
    }

    glPopMatrix();
}

//gonna need a dz
void Arm::moveBy(float dx, float dy)
{
    VectorXf dAngles = VectorXf::Zero(mList.size(), 1);
    VectorXf dPosition = VectorXf::Zero(2, 1);

   dPosition(0) = dx;
   dPosition(1) = dy;
  
    dAngles = pseudoInverse() * dPosition;
    
    for (int i = 0; i < mList.size(); i++)
        mList[i]->mAngle += dAngles(i);
    
    calculatePosition();
}

void Arm::update()
{

  if(isTargetResolved()) return;
  
   float x = (mTargetPosition(0) - mPosition(0) > 1) ? mStep : -mStep;
   float y = (mTargetPosition(1) + mPosition(1) > 1) ? mStep : -mStep;

  moveBy(x, y);
}

//checks if arm reached target
bool Arm::isTargetResolved()
{
  if(fabs(mPosition(0) - mTargetPosition(0)) <= 2.0f &&
      fabs(mPosition(1) + mTargetPosition(1)) <= 2.0f)
  {
      mResolveTarget = false;
      return true;
  }
   
  return false;
}

//returns random point within range of the arm
VectorXf Arm::getPointWithinRange()
{
   VectorXf point = VectorXf::Zero(2, 1);
   float length = 0;
   for(int i = 0; i < mList.size(); i++)
   {
     length += mList[i]->mLength;
   }

   float p_const = sqrt(2) / 2;
   
   point(0) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length * p_const));
   point(1) = ((rand() % 2 == 0) ? -1 : 1) * (rand() % (int)(length * p_const));
   
   return point;
}

//attempted to make method so the arm follows the mouse pointer, this is not working
VectorXf Arm::followMouse(){
	POINT p;
	GetCursorPos(&p);
	if (ScreenToClient(GetActiveWindow(), &p))
	{
		VectorXf point = VectorXf::Zero(2, 1);
		point(0) = p.x;
		point(1) = p.y;
		return point;
	}
}