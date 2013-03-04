#ifndef BLOCK_H_
#define BLOCK_H_
#include "tile.h"

class Block : public Tile {
	private:
		bool movable;
	public:
		Block(bool m) : Tile(3), movable(m) {}
		
		string draw() {
			string s = "||B||\n";
					s+="|/_\\|";
			return s;
		}
		
		void unmove() {
			movable = false;
		}
};
#endif
