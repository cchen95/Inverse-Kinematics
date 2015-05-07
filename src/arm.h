#ifndef Arm_H
#define Arm_H
#include <vector>

#include "color.h"
#include "Eigen/Dense"

using namespace Eigen;

class Joint;
class Arm
{
private:

    std::vector<Joint*> mList;
    VectorXf mBasePosition;
    VectorXf mPosition;
    VectorXf mTargetPosition;
    float mStep;
    bool mResolveTarget;
	float mR;
	float mG;
	float mB;
    
    void calculatePosition();
    MatrixXf jacobian();
    MatrixXf pseudoInverse();

    
public:

    Arm()
    {
        mBasePosition = VectorXf::Zero(2, 1);
        mPosition = VectorXf::Zero(2, 1);
	mTargetPosition = VectorXf::Zero(2, 1);
	mStep = 0.5f;
	mResolveTarget = false;
    }
    void draw();
    void moveToPoint(const VectorXf position);
    void moveBy(float dx, float dy);
    void update();
    bool isTargetResolved();
    VectorXf getPointWithinRange();
	VectorXf followMouse();
    
    inline void setStep(float step) { step = mStep; }
    inline void addJoint(Joint *Joint) { mList.push_back(Joint); calculatePosition();}
    inline VectorXf getPosition() { return mPosition; }
};

#endif // Arm_H
