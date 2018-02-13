#ifndef OPENGL_H_
#define OPENGL_H_

#include <common.h>
#include <GL/glew.h>

using VertexBufferID = GLuint;
using TextureID = GLuint;
using TextureFormat = GLenum;
constexpr TextureFormat TextureFormatRGB = GL_RGB;
constexpr TextureFormat TextureFormatRGBA = GL_RGBA;

#endif // !OPENGL_H_
