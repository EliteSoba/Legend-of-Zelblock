#ifndef HOLE_H_
#define HOLE_H_
#include "tile.h"

class Hole : public Tile {
	private:
		int side;
	public:
		Hole(int s) : Tile(4) {side = s;}
		
		string draw() {
			if (side == -1)
				return "XXXX|\nXXXX|";
			if (side == 0)
				return "XXXXX\nXXXXX";
			if (side == 1)
				return "|XXXX\n|XXXX";
			else
				return "|XXX|\n|XXX|";
		}
};
#endif
