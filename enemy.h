#ifndef ENEMY_H_
#define ENEMY_H_

#include <string>
using namespace std;

class Enemy {
	private:
		int x, y;
	public:
		Enemy(int r, int c) {
			x = r;
			y = c;
		}
		
		int getX() {
			return x;
		}
		
		int getY() {
			return y;
		}
		
		void moves(int dir) {
			switch(dir) {
				case 2: x--; break;
				case 4: y-=2; break;
				case 6: y+=2; break;
				case 8: x++; break;
			}
			if (x > 24)
				x -= 1;
			if (x < 0)
				x += 1;
			if (y < 0)
				y += 2;
			if (y > 60)
				y -= 2;
		}
		
};
#endif
