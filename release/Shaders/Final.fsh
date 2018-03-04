#version 330 core

uniform sampler2D Texture;

in vec2 VertexTexCoords;
in vec4 VertexColor;

out vec4 FragColor;

void main() {
	vec4 color = VertexColor * texture(Texture, VertexTexCoords);
	if (color.a <= 0.001f) discard;
    FragColor = color;
}
