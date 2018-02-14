#include "vertexarray.h"

void VertexBuffer::update(const VertexArray& va, bool staticDraw) {
	vertexes = va.vertexCount();
	format = va.format();
	if (va.vertexCount() == 0) {
		vertexes = id = 0;
		return;
	}
	if (id == 0) glGenBuffersARB(1, &id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, va.vertexCount() * sizeof(float) *
					format.vertexAttributeCount,
					va.data(), staticDraw ? GL_STATIC_DRAW_ARB : GL_STREAM_DRAW_ARB);
}

VertexBuffer::VertexBuffer(const VertexArray& va, bool staticDraw):
	vertexes(va.vertexCount()), format(va.format()) {
	if (va.vertexCount() == 0) {
		vertexes = id = 0;
		return;
	}
	glGenBuffersARB(1, &id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, va.vertexCount() * sizeof(float) *
					format.vertexAttributeCount,
					va.data(), staticDraw ? GL_STATIC_DRAW_ARB : GL_STREAM_DRAW_ARB);
}

void VertexBuffer::render() const {
	if (id == 0) return;

	static bool texCoordArrayEnabled = false;
	static bool colorArrayEnabled = false;
	static bool normalArrayEnabled = false;
	static bool vertexArrayEnabled = false;

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	if (format.textureCount != 0) {
		if (!texCoordArrayEnabled) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			texCoordArrayEnabled = true;
		}
		glTexCoordPointer(
			format.textureCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			nullptr
		);
	} else if (texCoordArrayEnabled) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		texCoordArrayEnabled = false;
	}

	if (format.colorCount != 0) {
		if (!colorArrayEnabled) {
			glEnableClientState(GL_COLOR_ARRAY);
			colorArrayEnabled = true;
		}
		glColorPointer(
			format.colorCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>(format.textureCount * sizeof(float))
		);
	} else if (colorArrayEnabled) {
		glDisableClientState(GL_COLOR_ARRAY);
		colorArrayEnabled = false;
	}

	if (format.normalCount != 0) {
		if (!normalArrayEnabled) {
			glEnableClientState(GL_NORMAL_ARRAY);
			normalArrayEnabled = true;
		}
		glNormalPointer(
			/*format.normalCount,*/ GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>((format.textureCount + format.colorCount) * sizeof(float))
		);
	} else if (normalArrayEnabled) {
		glDisableClientState(GL_NORMAL_ARRAY);
		normalArrayEnabled = false;
	}

	if (format.coordinateCount != 0) {
		if (!vertexArrayEnabled) {
			glEnableClientState(GL_VERTEX_ARRAY);
			vertexArrayEnabled = true;
		}
		glVertexPointer(
			format.coordinateCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>((format.textureCount + format.colorCount + format.normalCount) * sizeof(float))
		);
	} else if (vertexArrayEnabled) {
		glDisableClientState(GL_VERTEX_ARRAY);
		vertexArrayEnabled = false;
	}

	// 本来这里是有一个装逼的框的（
	glDrawArrays(GL_TRIANGLES, 0, vertexes);
}
