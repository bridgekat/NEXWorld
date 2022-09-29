#version 330 core

uniform mat4 ProjectionMatrix;
uniform mat4 ModelViewMatrix;
uniform vec3 CameraPosition;
uniform sampler2D Texture;
uniform int SkyBoxMode;
uniform vec3 Translation;

in vec2 VertexTexCoords;
in vec4 VertexColor;
in vec3 InterpolatedCoords;

out vec4 FragColor;

// Constants

const float Eps = 1e-4;
const float Pi = 3.14159265f;
const float Gamma = 2.2f;
const vec3 SunlightDirection = normalize(vec3(0.6f, -1.0f, 0.3f));

// Utilities

vec3 divide(vec4 v) { return (v / v.w).xyz; }

// PRNG

uint hash(uint x) {
	x += x << 10u;
	x ^= x >> 6u;
	x += x << 3u;
	x ^= x >> 11u;
	x += x << 15u;
	return x;
}

uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.yz)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.yzw)); }

float constructFloat(uint m) {
	const uint IEEEMantissa = 0x007FFFFFu;
	const uint IEEEOne = 0x3F800000u;
	m = m & IEEEMantissa | IEEEOne;
	return uintBitsToFloat(m) - 1.0f;
}

float rand(vec3 v) { return constructFloat(hash(floatBitsToUint(v))); }

// Fractal Noise

float interpolatedNoise3D(in vec3 x) {
	vec3 p = floor(x), f = fract(x);
	float v1 = mix(rand(p + vec3(0.0f, 0.0f, 0.0f)), rand(p + vec3(1.0f, 0.0f, 0.0f)), f.x);
	float v2 = mix(rand(p + vec3(0.0f, 1.0f, 0.0f)), rand(p + vec3(1.0f, 1.0f, 0.0f)), f.x);
	float v3 = mix(rand(p + vec3(0.0f, 0.0f, 1.0f)), rand(p + vec3(1.0f, 0.0f, 1.0f)), f.x);
	float v4 = mix(rand(p + vec3(0.0f, 1.0f, 1.0f)), rand(p + vec3(1.0f, 1.0f, 1.0f)), f.x);
	return mix(mix(v1, v2, f.y), mix(v3, v4, f.y), f.z);
}

float fractalNoise3D(in vec3 c) {
	float res = 0.0f;
	res += interpolatedNoise3D(c * 1.0f) / 1.0f;
	res += interpolatedNoise3D(c * 2.0f) / 2.0f;
	res += interpolatedNoise3D(c * 4.0f) / 4.0f;
	res += interpolatedNoise3D(c * 8.0f) / 8.0f;
	res += interpolatedNoise3D(c * 16.0f) / 16.0f;
	res += interpolatedNoise3D(c * 32.0f) / 32.0f;
	res = clamp(abs(res - 0.5f) * 2.0f - 0.8f, 0.0f, 1.0f);
	return res < 0.3f ? clamp((res - 0.3f) * 4.0f + 0.3f, 0.0f, 1.0f) : res;
}

// Volumetric Clouds

const vec3 CloudScale = vec3(100.0, 60.0, 100.0);
const float CloudBottom = 20.0, CloudTop = 200.0, CloudTransition = 20.0;
const int CloudIterations = 16;
const float CloudStep = 8.0, CloudDistance = 400.0;

float getCloudOpacity(in vec3 pos) {
	float factor = min(
		smoothstep(CloudBottom, CloudBottom + CloudTransition, pos.y),
		1.0 - smoothstep(CloudTop - CloudTransition, CloudTop, pos.y)
	);
	return factor * 0.5 * fractalNoise3D(pos / CloudScale);
}

vec4 cloud(in vec3 org, in vec3 dir, in float stp, in float maxDist) {
	vec3 curr = org;
	vec3 res = vec3(0.0);
	float transparency = 1.0;
	
	dir = normalize(dir);
	
	if (curr.y < CloudBottom) {
		if (dir.y <= 0.0) return vec4(0.0);
		curr += dir * (CloudBottom - curr.y) / dir.y;
	} else if (curr.y > CloudTop) {
		if (dir.y >= 0.0) return vec4(0.0);
		curr += dir * (CloudTop - curr.y) / dir.y;
	}
	
	for (int i = 0; i < CloudIterations; i++) {
		curr += dir * stp;
		
		if (transparency < 0.01) break;
		if (length(curr - org) > maxDist) break;
		
		if (curr.y >= CloudBottom && curr.y <= CloudTop) {
			float factor = smoothstep(-maxDist, -maxDist * 0.6, -length(curr - org));
			float opacity = factor * getCloudOpacity(curr) * 0.5;
			if (opacity > 0.0) {
				float occulusion = getCloudOpacity(curr - SunlightDirection * stp * 4.0);
				float col = clamp(1.2 - occulusion * 0.8, 0.6, 1.0);
				res += transparency * opacity * vec3(col);
				transparency *= 1.0 - opacity;
			}
		} else break;
	}
	
	return vec4(res, 1.0 - transparency);
}

vec3 getSkyColor(in vec3 org, in vec3 dir) {
	float sun = mix(0.0f, 0.7f, clamp(smoothstep(0.996f, 1.0f, dot(dir, -SunlightDirection)), 0.0f, 1.0f)) * 4.0f;
	sun += mix(0.0f, 0.3f, clamp(smoothstep(0.1f, 1.0f, dot(dir, -SunlightDirection)), 0.0f, 1.0f));
	vec3 sky = mix(
		vec3(152.0f / 255.0f, 211.0f / 255.0f, 250.0f / 255.0f),
		vec3(90.0f / 255.0f, 134.0f / 255.0f, 206.0f / 255.0f),
		smoothstep(0.0f, 1.0f, normalize(dir).y * 2.0f)
	);
	vec3 res = mix(sky, vec3(1.0f, 1.0f, 1.0f), sun);
	vec4 cloudColor = cloud(org, dir, CloudStep, CloudDistance);
	res = cloudColor.rgb + (1.0 - cloudColor.a) * res;
	return res;
}

void main() {
	if (SkyBoxMode == 1) {
		vec3 dir = normalize(InterpolatedCoords);
		FragColor = vec4(getSkyColor(CameraPosition, dir), 1.0f);
		return;
	}
	vec3 dir = normalize(InterpolatedCoords + Translation);
	vec4 color = VertexColor * texture(Texture, VertexTexCoords);
	if (color.a <= 0.001f) discard;
	vec4 cloudColor = cloud(CameraPosition, dir, CloudStep, min(length(InterpolatedCoords + Translation), CloudDistance));
	color = vec4(cloudColor.rgb + (1.0 - cloudColor.a) * color.rgb, color.a);
    FragColor = color;
}
