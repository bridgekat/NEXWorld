#ifndef PLAYER_H_
#define PLAYER_H_

#include <config.h>
#include <playerobject.h>
#include "camera.h"
#include "window.h"

class Player: public PlayerObject {
public:
	Player();

	// Call every game update
	void update();
	// Call every frame, before world rendering (to improve mouse response)
	void updateRotation(const Window& win) {
		MouseState mouse = win.getMouseMotion();
		rotate(Vec3d(-mouse.y * mMouseSensitivity, -mouse.x * mMouseSensitivity, 0.0));
	}

	// In relative space, camera position is always the origin
	Camera getRelativeCamera(float width, float height, float zFar, bool inertia = false, double interp = 0.0) const {
		Camera res;
		res.setPerspective(mFOV, width / height, 0.1f, zFar);
		res.setRotation(inertia ? interpolatedRotation(interp) : rotation() + mRotateSpeed);
		return res;
	}

private:
	float mFOV, mMouseSensitivity;
	Uint8 mForwardKey, mBackwardKey, mLeftKey, mRightKey, mUpKey, mDownKey;
};

#endif
