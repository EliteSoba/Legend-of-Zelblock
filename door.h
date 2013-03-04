#ifndef DOOR_H_
#define DOOR_H_
#include "tile.h"

class Door : public Tile {
	private:
		bool open;
	public:
		Door(bool o) : Tile(6), open(o) { }
		
		string draw() {
			if (open)
				return "";
			return "|\\|/|\n|/|\\|";
		}
		
};
#endif
