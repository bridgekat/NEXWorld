#ifndef RENDERER_H_
#define RENDERER_H_

#include <logger.h>
#include "opengl.h"
#include <vec.h>
#include <mat.h>
#include "vertexarray.h"

class Renderer {
public:
	// Setup rendering
	static void init();

	static void setViewport(int x, int y, int width, int height) {
		glViewport(x, y, width, height);
	}

	// Reset translations/rotations (Restore transform matrixes)
	static void restoreScale() {
		setModelMatrix();
		glLoadIdentity();
	}

	// Apply translation
	static void translate(const Vec3f& delta) {
		setModelMatrix();
		glTranslatef(delta.x, delta.y, delta.z);
	}

	// Apply rotation
	static void rotate(float degrees, const Vec3f& scale) {
		setModelMatrix();
		glRotatef(degrees, scale.x, scale.y, scale.z);
	}

	// Restore projection matrix
	static void restoreProj() {
		setProjMatrix();
		glLoadIdentity();
	}

	// Perspective projection
	static void applyPerspective(float fov, float aspect, float zNear, float zFar) {
		setProjMatrix();
		glMultMatrixf(Mat4f::perspective(fov, aspect, zNear, zFar).getTranspose().data);
	}

	// Orthogonal projection
	static void applyOrtho(float left, float right, float top, float bottom, float zNear, float zFar) {
		setProjMatrix();
		glMultMatrixf(Mat4f::ortho(left, right, top, bottom, zNear, zFar).getTranspose().data);
	}

	static void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
	static void flush() { glFlush(); }
	static void waitForComplete() { glFinish(); }
	static void checkError();

private:
	static int matrixMode;

	static void setProjMatrix() {
		if (matrixMode == GL_PROJECTION) return;
		glMatrixMode(GL_PROJECTION);
		matrixMode = GL_PROJECTION;
	}

	static void setModelMatrix() {
		if (matrixMode == GL_MODELVIEW) return;
		glMatrixMode(GL_MODELVIEW);
		matrixMode = GL_MODELVIEW;
	}
};

#endif // !RENDERER_H_
