#ifndef BLOCKDATA_H_
#define BLOCKDATA_H_

class BlockData {
public:
	unsigned int id : 12;
	unsigned int state : 12;
	unsigned int blocklight : 4;
	unsigned int skylight : 4;

	BlockData(): id(0), state(0), blocklight(0), skylight(0) {}
	BlockData(unsigned short id_, unsigned short state_ = 0, unsigned short blocklight_ = 0, unsigned short skylight_ = 0):
		id(id_), state(state_), blocklight(blocklight_), skylight(skylight_) {}
};

#endif
