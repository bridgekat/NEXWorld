#include "player.h"
#include <cmath>

Player::Player(): PlayerObject() {
	mForwardKey = char(Config::getInt("Player.ForwardKey", int(SDL_SCANCODE_W)));
	mBackwardKey = char(Config::getInt("Player.BackwardKey", int(SDL_SCANCODE_S)));
	mLeftKey = char(Config::getInt("Player.LeftKey", int(SDL_SCANCODE_A)));
	mRightKey = char(Config::getInt("Player.RightKey", int(SDL_SCANCODE_D)));
	mUpKey = char(Config::getInt("Player.UpKey", int(SDL_SCANCODE_SPACE)));
	mDownKey = char(Config::getInt("Player.DownKey", int(SDL_SCANCODE_LCTRL)));
	mFOV = float(Config::getDouble("Player.FieldOfView", 70.0));
	mMouseSensitivity = float(Config::getDouble("Player.MouseSensitivity", 0.3));
	mSelectDistance = Config::getDouble("Player.SelectDistance", 5.0);
	mSelectPrecision = Config::getDouble("Player.SelectPrecision", 200.0);
}

void Player::update(const Window& win, World& world) {
	MouseState mouse = win.getMouseMotion();
	if (mouse.left) {
		Mat4f trans(1.0f);
		trans *= Mat4f::rotation(float(mRotation.y), Vec3f(0.0f, 1.0f, 0.0f));
		trans *= Mat4f::rotation(float(mRotation.x), Vec3f(1.0f, 0.0f, 0.0f));
		trans *= Mat4f::rotation(float(mRotation.z), Vec3f(0.0f, 0.0f, 1.0f));
		Vec3d dir = trans.transform(Vec3f(0.0f, 0.0f, -1.0f), 0.0f).first.normalize();
		for (double i = 0.0; i < mSelectDistance; i += 1.0 / mSelectPrecision) {
			Vec3d pos = mPosition + dir * i;
			Vec3i blockPos = Vec3i(int(std::floor(pos.x)), int(std::floor(pos.y)), int(std::floor(pos.z)));
			// TODO: add property `selectable` to BlockType
			if (world.getBlock(blockPos).id != 0) {
				world.setBlock(blockPos, 0);
				break;
			}
		}
	}
	if (Window::isKeyPressed(mForwardKey)) moveOriented(Vec3d(0.0, 0.0, -0.5), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mBackwardKey)) moveOriented(Vec3d(0.0, 0.0, 0.5), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mLeftKey)) moveOriented(Vec3d(-0.5, 0.0, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mRightKey)) moveOriented(Vec3d(0.5, 0.0, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mUpKey)) moveOriented(Vec3d(0.0, 0.5, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mDownKey)) moveOriented(Vec3d(0.0, -0.5, 0.0), Vec3i(0, 1, 0));
	PlayerObject::update();
}
