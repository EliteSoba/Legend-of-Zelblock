#ifndef TORCH_H_
#define TORCH_H_
#include "tile.h"

class Torch : public Tile {
	private:
		
	public:
		Torch() : Tile(1) {}
		
		string draw() {
			string s = "|\\_/|\n";
					s+="||@||";
			return s;
		}
};

#endif
