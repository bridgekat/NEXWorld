#include "vertexarray.h"

void VertexBuffer::update(const VertexArray& va, bool staticDraw) {
	mVertexes = va.vertexCount();
	mFormat = va.format();
	if (va.vertexCount() == 0) {
		mVertexes = mID = 0;
		return;
	}
	if (!OpenGL::coreProfile()) {
		if (mID == 0) glGenBuffersARB(1, &mID);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, mID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, va.vertexCount() * sizeof(float) * mFormat.vertexAttributeCount,
						va.data(), staticDraw ? GL_STATIC_DRAW_ARB : GL_STREAM_DRAW_ARB);
	} else {
		if (mID == 0) {
			Assert(mVAO == 0);
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mID);
		}
		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mID);
		glBufferData(GL_ARRAY_BUFFER, va.vertexCount() * sizeof(float) * mFormat.vertexAttributeCount,
					 va.data(), staticDraw ? GL_STATIC_DRAW : GL_STREAM_DRAW);
		int cnt = 0;
		if (mFormat.textureCount != 0) {
			glVertexAttribPointer(
				cnt, mFormat.textureCount, GL_FLOAT, GL_FALSE,
				mFormat.vertexAttributeCount * sizeof(float),
				nullptr
			);
			glEnableVertexAttribArray(cnt++);
		}
		if (mFormat.colorCount != 0) {
			glVertexAttribPointer(
				cnt, mFormat.colorCount, GL_FLOAT, GL_FALSE,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>(mFormat.textureCount * sizeof(float))
			);
			glEnableVertexAttribArray(cnt++);
		}
		if (mFormat.normalCount != 0) {
			glVertexAttribPointer(
				cnt, mFormat.normalCount, GL_FLOAT, GL_FALSE,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>((mFormat.textureCount + mFormat.colorCount) * sizeof(float))
			);
			glEnableVertexAttribArray(cnt++);
		}
		if (mFormat.coordinateCount != 0) {
			glVertexAttribPointer(
				cnt, mFormat.coordinateCount, GL_FLOAT, GL_FALSE,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>((mFormat.textureCount + mFormat.colorCount + mFormat.normalCount) * sizeof(float))
			);
			glEnableVertexAttribArray(cnt++);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

VertexBuffer::VertexBuffer(const VertexArray& va, bool staticDraw):
	mVertexes(va.vertexCount()), mFormat(va.format()), mID(0), mVAO(0) {
	update(va, staticDraw);
}

void VertexBuffer::render() const {
	if (mID == 0) return;

	if (!OpenGL::coreProfile()) {
		static bool texCoordArrayEnabled = false;
		static bool colorArrayEnabled = false;
		static bool normalArrayEnabled = false;
		static bool vertexArrayEnabled = false;

		glBindBufferARB(GL_ARRAY_BUFFER_ARB, mID);
		if (mFormat.textureCount != 0) {
			if (!texCoordArrayEnabled) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				texCoordArrayEnabled = true;
			}
			glTexCoordPointer(
				mFormat.textureCount, GL_FLOAT,
				mFormat.vertexAttributeCount * sizeof(float),
				nullptr
			);
		} else if (texCoordArrayEnabled) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			texCoordArrayEnabled = false;
		}

		if (mFormat.colorCount != 0) {
			if (!colorArrayEnabled) {
				glEnableClientState(GL_COLOR_ARRAY);
				colorArrayEnabled = true;
			}
			glColorPointer(
				mFormat.colorCount, GL_FLOAT,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>(mFormat.textureCount * sizeof(float))
			);
		} else if (colorArrayEnabled) {
			glDisableClientState(GL_COLOR_ARRAY);
			colorArrayEnabled = false;
		}

		if (mFormat.normalCount != 0) {
			if (!normalArrayEnabled) {
				glEnableClientState(GL_NORMAL_ARRAY);
				normalArrayEnabled = true;
			}
			glNormalPointer(
				/*mFormat.normalCount,*/ GL_FLOAT,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>((mFormat.textureCount + mFormat.colorCount) * sizeof(float))
			);
		} else if (normalArrayEnabled) {
			glDisableClientState(GL_NORMAL_ARRAY);
			normalArrayEnabled = false;
		}

		if (mFormat.coordinateCount != 0) {
			if (!vertexArrayEnabled) {
				glEnableClientState(GL_VERTEX_ARRAY);
				vertexArrayEnabled = true;
			}
			glVertexPointer(
				mFormat.coordinateCount, GL_FLOAT,
				mFormat.vertexAttributeCount * sizeof(float),
				reinterpret_cast<float*>((mFormat.textureCount + mFormat.colorCount + mFormat.normalCount) * sizeof(float))
			);
		} else if (vertexArrayEnabled) {
			glDisableClientState(GL_VERTEX_ARRAY);
			vertexArrayEnabled = false;
		}
	} else {
		glBindVertexArray(mVAO);
	}

	// 本来这里是有一个装逼的框的（
	glDrawArrays(GL_TRIANGLES, 0, mVertexes);
}
