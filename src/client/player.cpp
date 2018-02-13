#include "player.h"

Player::Player(): PlayerObject() {
	mForwardKey = char(Config::getInt("Player.ForwardKey", int(SDL_SCANCODE_W)));
	mBackwardKey = char(Config::getInt("Player.BackwardKey", int(SDL_SCANCODE_S)));
	mLeftKey = char(Config::getInt("Player.LeftKey", int(SDL_SCANCODE_A)));
	mRightKey = char(Config::getInt("Player.RightKey", int(SDL_SCANCODE_D)));
	mUpKey = char(Config::getInt("Player.UpKey", int(SDL_SCANCODE_SPACE)));
	mDownKey = char(Config::getInt("Player.DownKey", int(SDL_SCANCODE_LCTRL)));
	mFOV = float(Config::getDouble("Player.FieldOfView", 70.0));
	mMouseSensitivity = float(Config::getDouble("Player.MouseSensitivity", 0.3));
}

void Player::update(const Window& win) {
	MouseState mouse = win.getMouseMotion();
	rotate(Vec3d(-mouse.y * mMouseSensitivity, -mouse.x * mMouseSensitivity, 0.0));
	if (Window::isKeyPressed(mForwardKey)) moveOriented(Vec3d(0.0, 0.0, -0.5), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mBackwardKey)) moveOriented(Vec3d(0.0, 0.0, 0.5), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mLeftKey)) moveOriented(Vec3d(-0.5, 0.0, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mRightKey)) moveOriented(Vec3d(0.5, 0.0, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mUpKey)) moveOriented(Vec3d(0.0, 0.5, 0.0), Vec3i(0, 1, 0));
	if (Window::isKeyPressed(mDownKey)) moveOriented(Vec3d(0.0, -0.5, 0.0), Vec3i(0, 1, 0));
	PlayerObject::update();
}
