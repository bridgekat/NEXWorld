#include "vertexarray.h"

void VertexBuffer::update(const VertexArray& va) {
	vertexes = va.getVertexCount();
	format = va.getFormat();
	if (id == 0) glGenBuffersARB(1, &id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, va.getVertexCount() * sizeof(float) *
					format.vertexAttributeCount,
					va.getData(), GL_STATIC_DRAW_ARB);
}

VertexBuffer::VertexBuffer(const VertexArray& va) :vertexes(va.getVertexCount()), format(va.getFormat()) {
	glGenBuffersARB(1, &id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, va.getVertexCount() * sizeof(float) *
					format.vertexAttributeCount,
					va.getData(), GL_STATIC_DRAW_ARB);
}

void VertexBuffer::render() const {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
	if (format.textureCount != 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(
			format.textureCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			nullptr
		);
	}
	if (format.colorCount != 0) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(
			format.colorCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>(format.textureCount * sizeof(float))
		);
	}
	if (format.normalCount != 0) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(
			/*format.normalCount,*/ GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>((format.textureCount + format.colorCount) * sizeof(float))
		);
	}
	if (format.coordinateCount != 0) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(
			format.coordinateCount, GL_FLOAT,
			format.vertexAttributeCount * sizeof(float),
			reinterpret_cast<float*>((format.textureCount + format.colorCount + format.normalCount) * sizeof(float))
		);
	}

	glDrawArrays(GL_QUADS, 0, vertexes);

	if (format.textureCount != 0) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (format.colorCount != 0) glDisableClientState(GL_COLOR_ARRAY);
	if (format.normalCount != 0) glDisableClientState(GL_NORMAL_ARRAY);
	if (format.coordinateCount != 0) glDisableClientState(GL_VERTEX_ARRAY);
}
