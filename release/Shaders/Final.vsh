#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec3 Color;
layout(location = 2) in vec3 Position;

out vec2 VertexTexCoords;
out vec4 VertexColor;
out vec3 InterpolatedCoords;

void main() {
	VertexColor = vec4(Color, 1.0f);
	VertexTexCoords = TexCoords;
	InterpolatedCoords = Position;
    gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(Position.xyz, 1.0f);
}
