#ifndef TILE_H_
#define TILE_H_
#include <string>
using namespace std;

class Tile {
	protected:
		int color;
	public:
		Tile() {
			color = 0;
		}
		
		Tile(int c) {
			color = c;
		}
		
		virtual string draw() = 0;
		
		int getColor() {
			return color;
		}
};
#endif
