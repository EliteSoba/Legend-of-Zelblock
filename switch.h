#ifndef SWITCH_H_
#define SWITCH_H_
#include "tile.h"

class Switch : public Tile {
	private:
		bool switched;
	public:
		Switch() : Tile(7), switched(false) { }
		
		void change() {
			switched = true;
		}
		
		string draw() {
			string s = "  _  \n";
			s+= " |_| ";
			return s;
		}
		
		bool isSwitched() {
			return switched;
		}
};
#endif
