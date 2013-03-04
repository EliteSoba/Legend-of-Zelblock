// simple curses example; keeps drawing the inputted characters, in columns
 // downward, shifting rightward when the last row is reached, and
 // wrapping around when the last column is reached

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <string>
#include "tile.h"
#include "torch.h"
#include "block.h"
#include "switch.h"
#include "hole.h"
#include "door.h"
#include "key.h"
#include "enemy.h"
#include <curses.h> // required

using namespace std;

void drawTile(Tile* t, int row, int col);
void init();
void drawmap();
void draw(char dc);
void checkChange();
void damage();
void initEnemies();
void moveEns();

char map[11][15]; //Map of objects
bool door[11][15]; //Map of doors
Enemy* ens[8][5];
int timepass = 0;
string mapdata[24]; //Background
bool hasKey; //If Link has the key to open the door
bool win; //If Win block is hit
int rr, cc; //Saves initial coordinates in each map
int swordx, swordy;
int health; //Current health
int r,c; // current row and column
int dir = 6; //Current direction faced
int curmap = 0; //Current map/room
int nrows, // number of rows in window
ncols; // number of columns in window

void drawTile(Tile* t, int row, int col) { //Draws all the objects in a room (not walls, etc.)
	string s = t->draw();
	if (s == "")
		return;
	init_pair(t->getColor(),1,0);
	attrset(COLOR_PAIR(t->getColor()));
	move(row*2 + 1, col*4);
	for(int i = 0; i < 5; i++)
		addch(s[i]);
	move(row*2 + 2, col*4);
	for(int i = 6; i < 11; i++)
		addch(s[i]);
	attroff(COLOR_PAIR(t->getColor()));
}

void checkChange() {
	//Oh dear; hardcoded mapping because I'm afraid of compatibility between systems
	for(int i = 0; i < 11; i++) {
		for(int j = 0; j < 15; j++) {
			if (map[i][j] == '0') {
				if (curmap == 1) {
					if (2 * i + 3 == r) {
						if (4 * j + 2 == c) {
							r = 1;
							curmap = 0;
							init();
						}
					}
				}
			}
			if (map[i][j] == '1') {
				if (curmap == 0) {
					if (2 * i == r ) {
						if (4 * j + 2 == c) {
							r = 22;
							curmap = 1;
							init();
						}
					}
				}
				else if (curmap == 2) {
					if (2 * i + 3 == r) {
						if (4 * j + 2 == c) {
							r = 1;
							curmap = 1;
							init();
						}
					}
				}
			}
			if (map[i][j] == '2') {
				if (curmap == 1) {
					if (2 * i  == r) {
						if (4 * j + 2 == c) {
							r = 22;
							curmap = 2;
							init();
						}
					}
				}
				else if (curmap == 3) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j == c) {
							c = 60;
							curmap = 2;
							init();
						}
					}
				}
				else if (curmap == 5) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j + 6 == c) {
							c = 2;
							curmap = 2;
							init();
						}
					}
				}
			}
			if (map[i][j] == '3') {
				if (curmap == 2) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j + 6 == c) {
							c = 2;
							curmap = 3;
							init();
						}
					}
				}
				else if (curmap == 4) {
					if (2 * i + 3 == r) {
						if (4 * j + 2 == c) {
							r = 1;
							curmap = 3;
							init();
						}
					}
				}
			}
			if (map[i][j] == '4') {
				if (curmap == 3) {
					if (2 * i == r) {
						if (4 * j + 2 == c) {
							r = 22;
							curmap = 4;
							init();
						}
					}
				}
			}
			if (map[i][j] == '5') {
				if (curmap == 2) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j == c) {
							c = 60;
							curmap = 5;
							init();
						}
					}
				}
				else if (curmap == 6) {
					if ( 2 * i + 3 == r) {
						if (4 * j + 2 == c) {
							r = 1;
							curmap = 5;
							init();
						}
					}
				}
			}
			if (map[i][j] == '6') {
				if (curmap == 5) {
					if (2 * i == r) {
						if (4 * j + 2 == c) {
							r = 22;
							curmap = 6;
							init();
						}
					}
				}
				else if (curmap == 7) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j == c) {
							c = 60;
							curmap = 6;
							init();
						}
					}
				}
			}
			if (map[i][j] == '7') {
				if (curmap == 6) {
					if (2 * i + 1 == r || 2 * i + 2 == r) {
						if (4 * j + 6 == c) {
							c = 2;
							curmap = 7;
							init();
						}
					}
				}
			}
		}
	}
}

void init() { //Initialize current map
	//Set entry point (For when falling in holes)
	rr = r;
	cc = c;
	
	for (int i = 0; i < 11; i++) { //Reset map
		for (int j = 0; j < 15; j++) {
			map[i][j] = 0;
			door[i][j] = false;
		}
	}
	for(int i = 0; i < 24; i++)
		mapdata[i] = "";
		
	//Read current map data from file
	string file = "map";
	file += 'a'+curmap;
	file += ".in";
	ifstream in(file.c_str());
	
	//Parse info from file
	string s;
	int x, y;
	char c;
	while (in.peek() != 'Q') {
		in >> x >> y >> c;
		if (c != 'D')
			map[x][y] = c;
		else
			door[x][y] = true;
		in.ignore('\n', 10);
	}
	getline(in, s);
	for (int i = 0; !in.eof(); i++) {
		getline(in, s);
		mapdata[i] = s;
	}
}

void drawmap() { //Draws the current map

	//Prints map background (walls, etc.)
	for (int i = 0; i < 24; i++) {
		const char* s = mapdata[i].c_str();
		printw("%s", s);
		printw("\n");
	}
	
	//Prints current health 
	init_pair(20, 1, 0);
	attrset(COLOR_PAIR(20));
	for (int i = 0; i < health; i++) {
	move(23, i);
		if (i % 2 == 0)
			addch('<');
		else
			addch('3');
	}
	attroff(COLOR_PAIR(20));
	
	moveEns();
	
	//Prints objects in map
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 15; j++) {
			if (map[i][j] == '@') { //Decorative torch
				drawTile(new Torch(), i, j);
			}
			if (map[i][j] == 'M') { //Movable block
				drawTile(new Block(true), i, j);
			}
			if (map[i][j] == 'B') {//Immovable block
				drawTile(new Block(false), i, j);
			}
			if (map[i][j] == 'S') {//Switch
				drawTile(new Switch(), i, j);
			}
			if (map[i][j] == 'K') {//The Key
				drawTile(new Key(map[1][7]!='S'), i, j);
			}
			if (map[i][j] == 'X') {//Holes in floor
				if (map[i][j-1] == 'X' && map[i][j+1] == 'X')
					drawTile(new Hole(0), i, j);
				if (map[i][j-1] != 'X' && map[i][j+1] == 'X')
					drawTile(new Hole(1), i, j);
				if (map[i][j-1] == 'X' && map[i][j+1] != 'X')
					drawTile(new Hole(-1), i, j);
				if (map[i][j-1] != 'X' && map[i][j+1] != 'X')
					drawTile(new Hole(2), i, j);
			}
			if (door[i][j] && !(i == (r-1)/2 && (j == c/4 || j == (c-1)/4))) {//Door drawing if Link is not currently in that doorway
				if (curmap == 1) {
					drawTile(new Door(map[3][7] == 'B'), i, j);
				}
				if (curmap == 2) {
					drawTile(new Door(hasKey), i, j);
				}
				if (curmap == 3) {
					drawTile(new Door(ens[3][0] == NULL && ens[3][1] == NULL && ens[3][2] == NULL), i, j);
				}
				if (curmap == 6) {
					drawTile(new Door(map[7][11] != 'M'), i, j);
				}
				if (curmap == 5) {
					drawTile(new Door(map[1][4] != 'S'), i, j);
				}
			}
		}
	}
}

void damage() {//Damages Link and returns him to the beginning of the room
	r = rr;
	c = cc;
	health--;
}

void moveEns() {//Moves enemies
	//init_pair(23, 1, 0);
	//attrset(COLOR_PAIR(23));
	int mover = 0;
	int x = 0, y = 0;
	for(int i = 0; i < 5; i++) {
		if (ens[curmap][i] !=NULL) {
			if (timepass % 15 == 0) {
				mover = 2 * (rand() % 4 + 1);
				ens[curmap][i]->moves(mover);
				
				move(ens[curmap][i]->getX(),ens[curmap][i]->getY());
				if (inch() != ' ' && inch() != '_' && inch() != '=') {
					ens[curmap][i]->moves(10-mover);
				}
				move(ens[curmap][i]->getX(),ens[curmap][i]->getY()+1);
				if (inch() != ' ' && inch() != '_' && inch() != '=') {
					ens[curmap][i]->moves(10-mover);
				}
				
			}
			
			x = ens[curmap][i]->getX();
			y = ens[curmap][i]->getY();
			move(x, y);
			addch('X');
			move(x, y+1);
			addch('X');
			if (x == r && (y == c)) {
				damage();
			}
		}
	}
	//attroff(COLOR_PAIR(23));
}

void draw(char dc) { //Controlling of Link
	
	//Resets and redraws map
	clear();
	drawmap();
	
	
	/*All of the following controls movement*/
	//Old coordinates to reset to if new location is invalid
	int oldr = r;
	int oldc = c;
	//Reads input and converts that to a direction
	switch(dc) {
		case 3: r--; dir = 8; break;
		case 2: r++; dir = 2; break;
		case 4: c-=2; dir = 4; break;
		case 5: c+=2; dir = 6; break;
		default: break;
	}
	//moved deals with special cases for blocks to prevent extra movement
	bool moved = false;
	
	move(r,c); // move cursor to row r, column c
	/*The following revolves around moving blocks around.
	Blocks can only be moved once, to make puzzles more difficult.*/
	int a = (r-1)/2, b = (c)/4;
	int ao = a, bo = b;
	if (map[a][b] == 'M') {
		map[a][b] = 0;
		switch(dc) {
			case 3: a-= map[a-1][b] == 'X' || map[a-1][b] == 'S' || map[a-1][b] == 0 ? 1:0; break;
			case 2: a+= map[a+1][b] == 'X' || map[a+1][b] == 'S' || map[a+1][b] == 0 ? 1:0; break;
			case 4: b-= map[a][b-1] == 'X' || map[a][b-1] == 'S' || map[a][b-1] == 0 ? 1:0; break;
			case 5: b+= map[a][b+1] == 'X' || map[a][b+1] == 'S' || map[a][b+1] == 0 ? 1:0; break;
			default: break;
		}
		if (ao == a && bo == b)
			map[a][b] = 'M';
		else if (map[a][b] != 'X')
			map[a][b] = 'B';
		moved = true;
		clear();
		drawmap();
	}
	
	b = (c-1)/4; //The way I set up coordinates and movement makes it necessary to check two different squares for collision.
	if (map[a][b] == 'M') {
		map[a][b] = 0;
		switch(dc) {
			case 3: a-= map[a-1][b] == 'X' || map[a-1][b] == 'S' || map[a-1][b] == 0 ? 1:0; break;
			case 2: a+= map[a+1][b] == 'X' || map[a+1][b] == 'S' || map[a+1][b] == 0 ? 1:0; break;
			case 4: b-= map[a][b-1] == 'X' || map[a][b-1] == 'S' || map[a][b-1] == 0 ? 1:0; break;
			case 5: b+= map[a][b+1] == 'X' || map[a][b+1] == 'S' || map[a][b+1] == 0 ? 1:0; break;
			default: break;
		}
		if (ao == a && bo == b)
			map[a][b] = 'M';
		else if (map[a][b] != 'X')
			map[a][b] = 'B';
		moved = true;
		clear();
		drawmap();
	}
	
	//Checks if Link's moving onto a switch
	if (map[a][b] == 'S') {
		map[a][b] = 0;
		moved = true;
	}
	b = (c)/4;
	if (map[a][b] == 'S') {
		map[a][b] = 0;
		moved = true;
	}
	
	//Checks to see if Link fell into a hole
	if(map[a][b] == 'X' && !moved) {
		damage();
		moved = true;
	}
	b = (c-1)/4;
	if (map[a][b] == 'X' && !(r==rr && c==cc) && !moved) {
		damage();
		moved = true;
	}
	
	//Checks to see if the key should appear and if Link gets it
	if (map[1][7] != 'S') {
		if (map[a][b] == 'K') {
			map[a][b] = 0;
			hasKey = true;
		}
		b = c/4;
		if (map[a][b] == 'K') {
			map[a][b] = 0;
			hasKey = true;
		}
	}
	
	//Checks to see if you win
	if (curmap == 7) {
		if (map[a][b] == 'W') {
			health = 0;
			win = true;
			return;
		}
		b = (c-1)/4;
		if (map[a][b] == 'W') {
			health = 0;
			win = true;
			return;
		}
	}
	
	
	//Check for invalid movement
	move(r,c);
	if (inch() == 'X') {
		damage();
		moved = true;
	}
	
	if (!moved) {
		if (inch() != ' ' && inch() != '_' && inch() != '=') {
			r = oldr;
			c = oldc;
		}
		move(r,c+1);
		if (inch() != ' ' && inch() != '_' && inch() != '=') {
			r = oldr;
			c = oldc;
		}
	}
	
	if (r == 24)
		r = 23;
	if (r == 23 && curmap == 0)
		r = 22;
	move(r, c);
	
	//Checks map changing
	checkChange();
	
	//Drawing Link
	init_pair(2, 2, 0);
	attrset(COLOR_PAIR(2));
	addch('<'); // replace character under cursor by dc
	addch('O');
	attroff(COLOR_PAIR(2));
	
	
	//SWORD TIME
	switch(dc) {
		case 3: swordx = r-1; swordy = c; break;
		case 2: swordx = r+1; swordy = c; break;
		case 4: swordx = r; swordy = c-2; break;
		case 5: swordx = r; swordy = c+2; break;
	}
	
	if (dc == 'z') {
		move(swordx, swordy);
		if (dir == 2 || dir == 8) {
			move(swordx, swordy+1);
			addch('|');
		}
		else {
			addch('-');
			move(swordx, swordy+1);
			addch('-');
		}
		int x, y;
		for (int i = 0; i < 5; i++) {
			if (ens[curmap][i] != NULL) {
				x = ens[curmap][i]->getX();
				y = ens[curmap][i]->getY();
				if (dir == 2 || dir == 8) {
					if (swordx == x && (swordy == y || swordy + 1 == y))
						ens[curmap][i] = NULL;
				}
				else {
					if (swordx == x && swordy == y)
						ens[curmap][i] = NULL;
				}
			}
		}
	}
	
	refresh(); // update screen
	move(0,0);
	
}

void initEnemies() { //Initiates enemy positions and maps
	for(int i = 0; i < 8; i++)
		for(int j = 0; j < 5; j++)
			ens[i][j] = NULL;
	ens[2][0] = new Enemy(7, 10);
	ens[2][1] = new Enemy(10, 30);
	ens[2][2] = new Enemy(7, 50);
	
	ens[3][0] = new Enemy(10, 20);
	ens[3][1] = new Enemy(10, 30);
	ens[3][2] = new Enemy(5, 30);
	
	ens[6][0] = new Enemy(17, 30);
	ens[6][1] = new Enemy(17, 40);
}

int main(int argc, char** argv)  { 
	//Initialization stuff
	srand ( time(NULL) ); //
	char d, i;
	curmap = 0;
	hasKey = false;
	win = false;
	WINDOW *wnd;
	if (argc > 1)
		curmap = argv[1][0]-'a';

	wnd = initscr(); // initialize window
	cbreak(); // no waiting for Enter key
	start_color(); //Enables colors
	keypad(stdscr,TRUE); //Enables arrow keys
	noecho(); // no echoing
	getmaxyx(wnd,nrows,ncols); // find size of window
	clear(); // clear screen, send cursor to position (0,0)
	refresh(); // implement all changes since last refresh
	move(0,0);
	health = 6;
	initEnemies();
	init();
	timeout(100);
	r = 22; c = 30;
	while (health > 0) {
	    i = getch(); // input from keyboard
			d = i;
			//I don't think this does anything, but just to be safe
			if (d == KEY_UP)
				d = 3;
			if (d == KEY_DOWN)
				d = 2;
			if (d == KEY_LEFT)
				d = 4;
			if (d == KEY_RIGHT)
				d = 5;
		timepass++;
		if (timepass == 100000)
			timepass = 0;
		if (d == 'q') break; // quit?
			draw(d); // draw the character
	}
	
	clear();
	if (!win)
		printw("GAME OVER");
	else
		printw("You completed this dungeon!\nTo be continued in Legend of Zelda: Oracle of Ages.");
	timeout(5000);
	i = getch();
	clear();
	timeout(600000);
	printw("Play again? (y/n)");
	i = getch();
	if (i == 'y')
		main(argc, argv);

	endwin(); // restore the original window
}

