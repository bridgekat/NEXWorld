#ifndef BLOCKDATA_H_
#define BLOCKDATA_H_

class BlockData {
public:
	unsigned int id;

	BlockData(): id(0) {}
	BlockData(unsigned short id_): id(id_) {}
};

#endif
