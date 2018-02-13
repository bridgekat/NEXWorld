#ifndef OBJECT_H_
#define OBJECT_H_

#include "vec.h"
#include "mat.h"

class Object {
public:
	Object() = default;
	Object(const Vec3d& position, const Vec3d& rotation = Vec3d(0.0)):
		mPosition(position), mRotation(rotation), mSpeed(0.0), mRotateSpeed(0.0) {}
	virtual ~Object() {}

	Vec3d position() const { return mPosition; }
	Vec3d rotation() const { return mRotation; }

	void move(const Vec3d& d) { mSpeed += d; }
	void rotate(const Vec3d& d) { mRotateSpeed += d; }
	void moveOriented(const Vec3d& d, const Vec3i& mul = 1) {
		Vec3f dir = mRotation.compMult(mul);
		Mat4f trans(1.0f);
		trans *= Mat4f::rotation(dir.y, Vec3f(0.0f, 1.0f, 0.0f));
		trans *= Mat4f::rotation(dir.x, Vec3f(1.0f, 0.0f, 0.0f));
		trans *= Mat4f::rotation(dir.z, Vec3f(0.0f, 0.0f, 1.0f));
		Vec3d delta = trans.transform(Vec3f(d), 1.0f).first;
		mSpeed += delta;
	}

	virtual void update() {
		mPosition += mSpeed;
		mRotation += mRotateSpeed;
	}

protected:
	// Transformation order: x, y, z, heading, pitch, yaw
	Vec3d mPosition, mRotation;
	Vec3d mSpeed, mRotateSpeed;
};

#endif
