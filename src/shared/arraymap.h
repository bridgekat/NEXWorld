#ifndef CHUNKPOINTERARRAY_H_
#define CHUNKPOINTERARRAY_H_

#include <vector>
#include <algorithm>
#include "chunk.h"

template <class T>
class ArrayMap {
public:
	ArrayMap(int size, const Vec3i& centerPos): mSizeLog2(log2Ceil(size)) {
		mSize = 1 << mSizeLog2;
		mSize2 = mSize * mSize;
		mSize3 = mSize2 * mSize;
		mMask = mSize - 1;
		mOrigin = centerPos - Vec3i(mSize / 2);
		mArray = new T[mSize3];
		std::fill(mArray, mArray + mSize3, T());
	}
	~ArrayMap() { delete[] mArray; }

	// Returns removed entries
	std::vector<T> move(const Vec3i& delta) {
		// Remove out-of-bound entries to avoid collision
		std::vector<T> res;
		Vec3i prev = mapPosition(mOrigin);
		Vec3i next = mapPosition(mOrigin + delta);
		Vec3i::range(0, mSize, [this, &delta](const Vec3i& pos) {
			if (inBetween(pos.x, prev.x, next.x) || inBetween(pos.y, prev.y, next.y) || inBetween(pos.z, prev.z, next.z)) {
				res.push_back(mArray[pos.x * mSize2 + pos.y * mSize + pos.z]);
				mArray[pos.x * mSize2 + pos.y * mSize + pos.z] = T();
			}
		});
		mOrigin += delta;
		return res;
	}

	void setCenterPos(const Vec3i& centerPos) {
		Vec3i pos = centerPos - Vec3i(mSize / 2);
		move(pos - mOrigin);
	}

	bool exist(const Vec3i& pos) const {
		return pos.x >= 0 && pos.x < mSize && pos.z >= 0 && pos.z < mSize && pos.y >= 0 && pos.y < mSize;
	}

	T get(const Vec3i& pos) const {
		return exist(pos - mOrigin) ? mArray[(pos.x & mMask) * mSize2 + (pos.y & mMask) * mSize + (pos.z & mMask)] : T();
	}

	void set(const Vec3i& pos, T c) {
		if (exist(pos - mOrigin)) mArray[(pos.x & mMask) * mSize2 + (pos.y & mMask) * mSize + (pos.z & mMask)] = c;
	}

private:
	int mSizeLog2, mSize, mSize2, mSize3;
	unsigned int mMask;
	Vec3i mOrigin = 0;
	T* mArray;

	int log2Ceil(int x) {
		if (x <= 1)return 0;
		x--;
		int res = 0;
		while (x != 0) x /= 2, res++;
		return res;
	}

	// v in [a, b) or in (b, a]
	int inBetween(int v, int a, int b) {
		return (a <= b && v >= a && v < b) || (a > b && v <= a && v > b);
	}

	Vec3i mapPosition(const Vec3i& pos) {
		return Vec3i(pos.x & mMask, pos.y & mMask, pos.z & mMask);
	}
};

#endif
