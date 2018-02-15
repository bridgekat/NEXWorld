#ifndef PLAYEROBJECT_H_
#define PLAYEROBJECT_H_

#include <string>
#include "object.h"

class PlayerObject: public Object {
public:
	virtual void update() override {
		Object::update();
		mSpeed *= 0.8;
		mRotateSpeed *= 0.0;
	}

private:
	//std::string mName;
};

#endif
