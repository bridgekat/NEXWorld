#ifndef MAT_H_
#define MAT_H_

#include <cassert>
#include "common.h"
#include "debug.h"

#ifdef NEXWORLD_DEBUG

// Mat4Row for range checking when debugging is enabled
template <typename T>
class Mat4Row {
public:
	T* ptr;

	explicit Mat4Row(T* p): ptr(p) {}

	T& operator[](size_t index) {
		Assert(index < 4);
		return ptr[index];
	}
};

#endif

template <typename T>
class Mat4 {
public:
	static constexpr double PI = 3.1415926535897932;

	T data[16];

	Mat4() { memset(data, 0, sizeof(data)); }
	Mat4(const Mat4& rhs) { memcpy(data, rhs.data, sizeof(data)); }
	explicit Mat4(T x) {
		memset(data, 0, sizeof(data));
		data[0] = data[5] = data[10] = data[15] = x; // Identity matrix
	}
	explicit Mat4(T* src) { memcpy(data, src, sizeof(data)); }

#ifdef NDEBUG

	T* operator[](size_t index) { return data + index * 4; }

#else

	Mat4Row<T> operator[](size_t index) {
		Assert(index < 4);
		return Mat4Row<T>(data + index * 4);
	}

#endif

	Mat4& operator+=(const Mat4& rhs) {
		for (int i = 0; i < 16; i += 4) {
			data[i + 0] += rhs.data[i + 0];
			data[i + 1] += rhs.data[i + 1];
			data[i + 2] += rhs.data[i + 2];
			data[i + 3] += rhs.data[i + 3];
		}
		return *this;
	}

	Mat4 operator*(const Mat4& rhs) const {
		Mat4 res;
		res.data[0] = data[0] * rhs.data[0] + data[1] * rhs.data[4] + data[2] * rhs.data[8] + data[3] * rhs.data[12];
		res.data[1] = data[0] * rhs.data[1] + data[1] * rhs.data[5] + data[2] * rhs.data[9] + data[3] * rhs.data[13];
		res.data[2] = data[0] * rhs.data[2] + data[1] * rhs.data[6] + data[2] * rhs.data[10] + data[3] * rhs.data[14];
		res.data[3] = data[0] * rhs.data[3] + data[1] * rhs.data[7] + data[2] * rhs.data[11] + data[3] * rhs.data[15];
		res.data[4] = data[4] * rhs.data[0] + data[5] * rhs.data[4] + data[6] * rhs.data[8] + data[7] * rhs.data[12];
		res.data[5] = data[4] * rhs.data[1] + data[5] * rhs.data[5] + data[6] * rhs.data[9] + data[7] * rhs.data[13];
		res.data[6] = data[4] * rhs.data[2] + data[5] * rhs.data[6] + data[6] * rhs.data[10] + data[7] * rhs.data[14];
		res.data[7] = data[4] * rhs.data[3] + data[5] * rhs.data[7] + data[6] * rhs.data[11] + data[7] * rhs.data[15];
		res.data[8] = data[8] * rhs.data[0] + data[9] * rhs.data[4] + data[10] * rhs.data[8] + data[11] * rhs.data[12];
		res.data[9] = data[8] * rhs.data[1] + data[9] * rhs.data[5] + data[10] * rhs.data[9] + data[11] * rhs.data[13];
		res.data[10] = data[8] * rhs.data[2] + data[9] * rhs.data[6] + data[10] * rhs.data[10] + data[11] * rhs.data[14];
		res.data[11] = data[8] * rhs.data[3] + data[9] * rhs.data[7] + data[10] * rhs.data[11] + data[11] * rhs.data[15];
		res.data[12] = data[12] * rhs.data[0] + data[13] * rhs.data[4] + data[14] * rhs.data[8] + data[15] * rhs.data[12];
		res.data[13] = data[12] * rhs.data[1] + data[13] * rhs.data[5] + data[14] * rhs.data[9] + data[15] * rhs.data[13];
		res.data[14] = data[12] * rhs.data[2] + data[13] * rhs.data[6] + data[14] * rhs.data[10] + data[15] * rhs.data[14];
		res.data[15] = data[12] * rhs.data[3] + data[13] * rhs.data[7] + data[14] * rhs.data[11] + data[15] * rhs.data[15];
		return res;
	}

	Mat4& operator*=(const Mat4& rhs) {
		*this = *this * rhs;
		return *this;
	}

	// Swap row r1, row r2
	void swapRows(size_t r1, size_t r2) {
		assert(r1 < 4 && r2 < 4);
		std::swap(data[r1 * 4 + 0], data[r2 * 4 + 0]);
		std::swap(data[r1 * 4 + 1], data[r2 * 4 + 1]);
		std::swap(data[r1 * 4 + 2], data[r2 * 4 + 2]);
		std::swap(data[r1 * 4 + 3], data[r2 * 4 + 3]);
	}

	// Row r *= k
	void multRow(size_t r, T k) {
		assert(r < 4);
		data[r * 4 + 0] *= k;
		data[r * 4 + 1] *= k;
		data[r * 4 + 2] *= k;
		data[r * 4 + 3] *= k;
	}

	// Row dst += row src * k
	void multAndAdd(size_t src, size_t dst, T k) {
		assert(dst < 4 && src < 4);
		data[dst * 4 + 0] += data[src * 4 + 0] * k;
		data[dst * 4 + 1] += data[src * 4 + 1] * k;
		data[dst * 4 + 2] += data[src * 4 + 2] * k;
		data[dst * 4 + 3] += data[src * 4 + 3] * k;
	}

	// Transpose matrix
	void transpose() {
		std::swap(data[1], data[4]);
		std::swap(data[2], data[8]);
		std::swap(data[3], data[12]);
		std::swap(data[6], data[9]);
		std::swap(data[7], data[13]);
		std::swap(data[11], data[14]);
	}

	// Get transposed matrix
	Mat4 getTranspose() const {
		Mat4 res;
		res.data[0] = data[0], res.data[1] = data[4], res.data[2] = data[8], res.data[3] = data[12];
		res.data[4] = data[1], res.data[5] = data[5], res.data[6] = data[9], res.data[7] = data[13];
		res.data[8] = data[2], res.data[9] = data[6], res.data[10] = data[10], res.data[11] = data[14];
		res.data[12] = data[3], res.data[13] = data[7], res.data[14] = data[11], res.data[15] = data[15];
		return res;
	}

	// Inverse matrix
	Mat4& inverse() {
		Mat4 res(T(1));
		for(int i=0; i<4; i++) {
			int p=i;
			for(int j=i+1; j<4; j++)if(data[j*4+i]!=T(0))p=j;
			res.swapRows(i,p);
			swapRows(i,p);
			res.multRow(i,T(1)/data[i*4+i]);
			multRow(i,T(1)/data[i*4+i]);
			for(int j=i+1; j<4; j++) {
				res.multAndAdd(i,j,-data[j*4+i]);
				multAndAdd(i,j,-data[j*4+i]);
			}
		}
		for(int i=3; i>=0; i--) {
			for(int j=0; j<i; j++) {
				res.multAndAdd(i,j,-data[j*4+i]);
				multAndAdd(i,j,-data[j*4+i]);
			}
		}
		(*this)=res;
		return *this;
	}

	// Construct a perspective projection matrix
	static Mat4 perspective(T fov, T aspect, T zNear, T zFar) {
		Mat4 res;
		T f = T(1) / tan(fov * T(PI) / T(180) / T(2));
		T a = zNear - zFar;
		res.data[0] = f / aspect;
		res.data[5] = f;
		res.data[10] = (zFar + zNear) / a;
		res.data[11] = T(2) * zFar * zNear / a;
		res.data[14] = T(-1);
		return res;
	}

	// Construct an orthogonal projection matrix
	static Mat4 ortho(T left, T right, T top, T bottom, T zNear, T zFar) {
		T a = right - left;
		T b = top - bottom;
		T c = zFar - zNear;
		Mat4 res;
		res.data[0] = T(2) / a;
		res.data[3] = -(right + left) / a;
		res.data[5] = T(2) / b;
		res.data[7] = -(top + bottom) / b;
		res.data[10] = T(-2) / c;
		res.data[11] = -(zFar + zNear) / c;
		res.data[15] = T(1);
		return res;
	}
};

typedef Mat4<float> Mat4f;

#endif
