#ifndef KEY_H_
#define KEY_H_
#include "tile.h"

class Key : public Tile {
	private:
		bool visible;
	public:
		Key(bool b) : Tile(7), visible(b) { }
		
		string draw() {
			if (!visible)
				return "";
			return " KEY \n KEY ";
		}
};
#endif
