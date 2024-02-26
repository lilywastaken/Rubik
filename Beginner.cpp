#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <stdio.h>

#include "Cube.h"

using namespace std;

string iv = "Invalid cube -- no solutions possible. Make sure you have entered your cube's state correctly.";

int readChar() {
	while (true) {
		char c = getchar();
		if (cin.fail()) { cerr << "Input error."; exit(0); }
		c = tolower(c);
		switch (c) {
		case 'w': return 0;
		case 'r': return 1;
		case 'b': return 2;
		case 'o': return 3;
		case 'g': return 4;
		case 'y': return 5;
		}
		cout << "Input invalid. Please enter the first letter of the color: " << endl;
	}
}

bool opposite(int a, int b) {
	/* Returns true if the two colors are opposites */
	if (a == 1 && b == 3) { return true; }
	if (a == 3 && b == 1) { return true; }
	if (a == 2 && b == 4) { return true; }
	if (a == 4 && b == 2) { return true; }
	if (a == 0 && b == 5) { return true; }
	if (a == 5 && b == 0) { return true; }
	return false;
}

void firstLayer(Cube &c, vector<Cube> &cubeList) {
	/* Reference: 0 = up, 1 = right, 2 = left, 3 = front, 4 = back, 5 = down */
	if (c.getSide(0).same() && c.getSide(0).color == 0) {
		cout << "Side is already solved.";
		return;
	}

	/* Color notation:
	* 0: white
	* 1: red
	* 2: blue
	* 3: orange
	* 4: green
	* 5: yellow
	*/
	
	Cube temp = c;
	string a = "Cubie is already in position.";

	/* We want the top face to be all white */
	/* Find and place top left cubie */
	cout << "Placing the top left cubie: ";
	Side u = c.getSide(0);
	Side b = c.getSide(4);
	Side l = c.getSide(2);
	Side r = c.getSide(1);
	Side f = c.getSide(3);
	Side d = c.getSide(5);
	if (!u.getupper().getleft() &&
		b.getupper().getright() == 3 &&
		l.getupper().getleft() == 4) {
		// already in correct position
		cout << a;
	}
	// check for cubie in top left of another face
	else if (!l.getupper().getleft() && u.getupper().getleft() == 3) {
		c.Up();
		c.Lp();
	}
	else if (!f.getupper().getleft() && u.getlower().getleft() == 3) {
		c.Lp();
	}
	else if (!r.getupper().getleft() && u.getlower().getright() == 3) {
		c.U();
		c.Lp();
	}
	else if (!b.getupper().getleft() && u.getupper().getright() == 3) {
		c.U2();
		c.Lp();
	}
	else if (!d.getupper().getleft() && f.getlower().getleft() == 3) {
		c.L2();
	}
	// check for cubie in top right
	else if (!u.getupper().getright() && r.getupper().getright() == 3) {
		c.Up();
	}
	else if (!l.getupper().getright() && f.getupper().getleft() == 3) {
		c.Up();
		c.Fp();
		c.Lp();
	}
	else if (!f.getupper().getright() && r.getupper().getleft() == 3) {
		c.Fp();
		c.Lp();
	}
	else if (!r.getupper().getright() && b.getupper().getleft() == 3) {
		c.U();
		c.Fp();
		c.Lp();
	}
	else if (!b.getupper().getright() && l.getupper().getleft() == 3) {
		c.U2();
		c.Fp();
		c.Lp();
	}
	else if (!d.getupper().getright() && f.getlower().getright() == 4) {
		c.R();
		c.Fp();
		c.Lp();
	}
	// check for cubie in bottom left
	else if (!u.getlower().getright() && f.getupper().getright() == 3) {
		c.U2();
	}
	else if (!l.getlower().getleft() && b.getlower().getright() == 3) {
		c.D();
		c.F();
		c.Lp();
	}
	else if (!f.getlower().getleft() && l.getlower().getright() == 3) {
		c.F();
		c.Lp();
	}
	else if (!r.getlower().getleft() && f.getlower().getright() == 3) {
		c.Dp();
		c.F();
		c.Lp();
	}
	else if (!b.getlower().getleft() && r.getlower().getright() == 3) {
		c.D2();
		c.F();
		c.Lp();
	}
	else if (!d.getlower().getleft() && b.getlower().getright() == 4) {
		c.L2();
		c.U();
	}
	// check for cubie in bottom right
	else if (!u.getlower().getleft() && l.getupper().getright() == 3) {
		c.U();
	}
	else if (!l.getlower().getright() && f.getlower().getleft() == 4) {
		c.D();
		c.F2();
		c.Lp();
	}
	else if (!f.getlower().getright() && r.getlower().getleft() == 4) {
		c.F2();
		c.Lp();
	}
	else if (!r.getlower().getright() && b.getlower().getleft() == 4) {
		c.Dp();
		c.F2();
		c.Lp();
	}
	else if (!b.getlower().getright() && l.getlower().getleft() == 4) {
		c.D2();
		c.F2();
		c.Lp();
	}
	else if (!d.getlower().getright() && b.getlower().getleft() == 3) {
		c.D2();
		c.L2();
	}
	else {
		temp.displayCube();
		throw iv;
	}
	// check that cube was properly placed
	u = c.getSide(0);
	b = c.getSide(4);
	l = c.getSide(2);
	r = c.getSide(1);
	f = c.getSide(3);
	d = c.getSide(5);
	if (!(!u.getupper().getleft() &&
		b.getupper().getright() == 3 &&
		l.getupper().getleft() == 4)) {
		cout << endl;
		temp.displayCube();
		throw iv;
	}
	cout << flush << endl;

	/* The top left cubie is placed correctly */
	/* Find and place the top right cubie */
	temp = c;
	cubeList.push_back(c);
	cout << "Placing the top right cubie: ";
	if (!u.getupper().getright() &&
		b.getupper().getleft() == 3 &&
		r.getupper().getright() == 2) {
		// already in correct position
		cout << a;
	}
	// check for cubie in top left of another face
	else if (!u.getupper().getleft() && b.getupper().getright() == 2) {
		// not possible
		throw iv;
	}
	else if (!l.getupper().getleft() && u.getupper().getleft() == 2) {
		// not possible
		throw iv;
	}
	else if (!f.getupper().getleft() && u.getlower().getleft() == 2) {
		c.F();
		c.R();
	}
	else if (!r.getupper().getleft() && u.getlower().getright() == 2) {
		c.U();
		c.Fp();
		c.Up();
		c.F2();
		c.R();
	}
	else if (!b.getupper().getleft() && u.getupper().getright() == 2) {
		c.R2();
		c.Fp();
		c.R();
	}
	else if (!d.getupper().getleft() && f.getlower().getleft() == 2) {
		c.D();
		c.R2();
	}
	// check for cubie in top right
	else if (!l.getupper().getright() && f.getupper().getleft() == 2) {
		c.F();
		c.Rp();
		c.Fp();
		c.R();
	}
	else if (!f.getupper().getright() && r.getupper().getleft() == 2) {
		c.R();
	}
	else if (!r.getupper().getright() && b.getupper().getleft() == 2) {
		c.U();
		c.F();
		c.Up();
		c.Fp();
		c.R();
	}
	else if (!b.getupper().getright() && l.getupper().getleft() == 2) {
		c.U2();
		c.F();
		c.U2();
		c.Fp();
		c.R();
	}
	else if (!d.getupper().getright() && f.getlower().getright() == 3) {
		c.R2();
	}
	// check for cubie in bottom left
	else if (!u.getlower().getleft() && l.getupper().getright() == 2) {
		c.Fp();
		c.D();
		c.Fp();
		c.R();
	}
	else if (!l.getlower().getleft() && b.getlower().getright() == 2) {
		c.D();
		c.F2();
		c.R();
	}
	else if (!f.getlower().getleft() && l.getlower().getright() == 2) {
		c.F2();
		c.R();
	}
	else if (!r.getlower().getleft() && f.getlower().getright() == 2) {
		c.Dp();
		c.F2();
		c.R();
	}
	else if (!b.getlower().getleft() && r.getlower().getright() == 2) {
		c.D2();
		c.F2();
		c.R();
	}
	else if (!d.getlower().getleft() && b.getlower().getright() == 3) {
		c.D2();
		c.R2();
	}
	// check for cubie in bottom right
	else if (!u.getlower().getright() && f.getupper().getright() == 2) {
		c.Rp();
		c.Fp();
		c.R();
	}
	else if (!l.getlower().getright() && f.getlower().getleft() == 3) {
		c.D();
		c.Fp();
		c.R();
	}
	else if (!f.getlower().getright() && r.getlower().getleft() == 3) {
		c.Fp();
		c.R();
	}
	else if (!r.getlower().getright() && b.getlower().getleft() == 3) {
		c.Dp();
		c.Fp();
		c.R();
	}
	else if (!b.getlower().getright() && l.getlower().getleft() == 3) {
		c.D2();
		c.Fp();
		c.R();
	}
	else if (!d.getlower().getright() && b.getlower().getleft() == 2) {
		c.Dp();
		c.R2();
	}
	else {
		temp.displayCube();
		throw iv;
	}
	// check that cube was properly placed
	u = c.getSide(0);
	b = c.getSide(4);
	l = c.getSide(2);
	r = c.getSide(1);
	f = c.getSide(3);
	d = c.getSide(5);
	if (!(!u.getupper().getleft() &&
		b.getupper().getright() == 3 &&
		l.getupper().getleft() == 4) ||
		!(!u.getupper().getright() &&
			b.getupper().getleft() == 3 &&
			r.getupper().getright() == 2)) {
		cout << endl;
		temp.displayCube();
		throw iv;
	}
	cout << flush << endl;

	/* The top cubies are placed correctly */
	/* Find and place the bottom left cubie */
	temp = c;
	cubeList.push_back(c);
	cout << "Placing the bottom left cubie: ";
	if (!u.getlower().getleft() &&
		f.getupper().getleft() == 1 &&
		l.getupper().getright() == 4) {
		// already in correct position
		cout << a;
	}
	// check for cubie in top left of another face
	else if (!u.getupper().getleft() && l.getupper().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!l.getupper().getleft() && u.getupper().getleft() == 4) {
		// not possible
		throw iv;
	}
	else if (!f.getupper().getleft() && u.getlower().getleft() == 4) {
		c.Fp();
		c.Bp();
		c.Lp();
		c.B();
	}
	else if (!r.getupper().getleft() && u.getlower().getright() == 4) {
		c.Fp();
	}
	else if (!b.getupper().getleft() && u.getupper().getright() == 4) {
		// not possible
		throw iv;
	}
	else if (!d.getupper().getleft() && f.getlower().getleft() == 4) {
		c.D();
		c.F2();
	}
	// check for cubie in top right
	else if (!u.getupper().getright() && r.getupper().getright() == 4) {
		// not possible
		throw iv;
	}
	else if (!l.getupper().getright() && f.getupper().getleft() == 4) {
		c.F();
		c.Rp();
		c.Dp();
		c.R();
		c.F();
	}
	else if (!f.getupper().getright() && r.getupper().getleft() == 4) {
		c.F();
		c.Dp();
		c.F();

	}
	else if (!r.getupper().getright() && b.getupper().getleft() == 4) {
		// not possible
		throw iv;
	}
	else if (!b.getupper().getright() && l.getupper().getleft() == 4) {
		// not possible
		throw iv;
	}
	else if (!d.getupper().getright() && f.getlower().getright() == 1) {
		c.F2();
	}
	// check for cubie in bottom left
	else if (!l.getlower().getleft() && b.getlower().getright() == 4) {
		c.D();
		c.Bp();
		c.Lp();
		c.B();
	}
	else if (!f.getlower().getleft() && l.getlower().getright() == 4) {
		c.Bp();
		c.Lp();
		c.B();
	}
	else if (!r.getlower().getleft() && f.getlower().getright() == 4) {
		c.Dp();
		c.Bp();
		c.Lp();
		c.B();
	}
	else if (!b.getlower().getleft() && r.getlower().getright() == 4) {
		c.D2();
		c.Bp();
		c.Lp();
		c.B();
	}
	else if (!d.getlower().getleft() && b.getlower().getright() == 1) {
		c.D2();
		c.F2();
	}
	// check for cubie in bottom right
	else if (!u.getlower().getright() && f.getupper().getright() == 4) {
		c.Rp();
		c.Dp();
		c.R();
		c.F();
	}
	else if (!l.getlower().getright() && f.getlower().getleft() == 1) {
		c.F();
	}
	else if (!f.getlower().getright() && r.getlower().getleft() == 1) {
		c.Dp();
		c.F();
	}
	else if (!r.getlower().getright() && b.getlower().getleft() == 1) {
		c.D2();
		c.F();
	}
	else if (!b.getlower().getright() && l.getlower().getleft() == 1) {
		c.D();
		c.F();
	}
	else if (!d.getlower().getright() && b.getlower().getleft() == 4) {
		c.Dp();
		c.F2();
	}
	else {
		temp.displayCube();
		throw iv;
	}
	// check that cube was properly placed
	u = c.getSide(0);
	b = c.getSide(4);
	l = c.getSide(2);
	r = c.getSide(1);
	f = c.getSide(3);
	d = c.getSide(5);
	if (!(!u.getupper().getleft() &&
			b.getupper().getright() == 3 &&
			l.getupper().getleft() == 4) ||
		(!(!u.getupper().getright() &&
			b.getupper().getleft() == 3 &&
			r.getupper().getright() == 2)) ||
		!(!u.getlower().getleft() &&
			f.getupper().getleft() == 1 &&
			l.getupper().getright() == 4)) {
		cout << endl;
		temp.displayCube();
		throw iv;
	}
	cout << flush << endl;

	/* The top and left cubies are placed correctly */
	/* Find and place the final bottom right cubie */
	temp = c;
	cubeList.push_back(c);
	cout << "Placing the bottom right cubie: ";
	if (!u.getlower().getright() &&
		f.getupper().getright() == 1 &&
		r.getupper().getleft() == 2) {
		// already in correct position
	}
	// check for cubie in top left of another face
	else if (!l.getupper().getleft() && u.getupper().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!f.getupper().getleft() && u.getlower().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!r.getupper().getleft() && u.getlower().getright() == 1) {
		c.Rp();
		c.Dp();
		c.R();
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	else if (!b.getupper().getleft() && u.getupper().getright() == 1) {
		// not possible
		throw iv;
	}
	else if (!d.getupper().getleft() && f.getlower().getleft() == 1) {
		c.D();
		c.Rp();
		c.D2();
		c.R();
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	// check for cubie in top right
	else if (!l.getupper().getright() && f.getupper().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!f.getupper().getright() && r.getupper().getleft() == 1) {
		c.Rp();
		c.D();
		c.R();
		c.B();
		c.R();
		c.Bp();
	}
	else if (!r.getupper().getright() && b.getupper().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!b.getupper().getright() && l.getupper().getleft() == 1) {
		// not possible
		throw iv;
	}
	else if (!d.getupper().getright() && f.getlower().getright() == 2) {
		c.Rp();
		c.D2();
		c.R();
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	// check for cubie in bottom left
	else if (!l.getlower().getleft() && b.getlower().getright() == 1) {
		c.D2();
		c.Lp();
		c.Fp();
		c.L();
	}
	else if (!f.getlower().getleft() && l.getlower().getright() == 1) {
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	else if (!r.getlower().getleft() && f.getlower().getright() == 1) {
		c.Lp();
		c.Fp();
		c.L();
	}
	else if (!b.getlower().getleft() && r.getlower().getright() == 1) {
		c.Dp();
		c.Lp();
		c.Fp();
		c.L();
	}
	else if (!d.getlower().getleft() && b.getlower().getright() == 2) {
		c.D2();
		c.Lp();
		c.F2();
		c.L();
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	// check for cubie in bottom right
	else if (!l.getlower().getright() && f.getlower().getleft() == 2) {
		c.D();
		c.B();
		c.R();
		c.Bp();
	}
	else if (!f.getlower().getright() && r.getlower().getleft() == 2) {
		c.B();
		c.R();
		c.Bp();
	}
	else if (!r.getlower().getright() && b.getlower().getleft() == 2) {
		c.Dp();
		c.B();
		c.R();
		c.Bp();
	}
	else if (!b.getlower().getright() && l.getlower().getleft() == 2) {
		c.D2();
		c.B();
		c.R();
		c.Bp();
	}
	else if (!d.getlower().getright() && b.getlower().getleft() == 1) {
		c.Dp();
		c.Lp();
		c.F2();
		c.L();
		c.D();
		c.Lp();
		c.Fp();
		c.L();
	}
	else {
		temp.displayCube();
		throw iv;
	}
	// check that cube was properly placed
	u = c.getSide(0);
	b = c.getSide(4);
	l = c.getSide(2);
	r = c.getSide(1);
	f = c.getSide(3);
	d = c.getSide(5);
	if ((!(!u.getupper().getleft() &&
			b.getupper().getright() == 3 &&
			l.getupper().getleft() == 4) ||
		!(!u.getlower().getright() &&
			f.getupper().getright() == 1 &&
			r.getupper().getleft() == 2)) ||
		(!(!u.getupper().getright() &&
			b.getupper().getleft() == 3 &&
			r.getupper().getright() == 2)) ||
		!(!u.getlower().getleft() &&
			f.getupper().getleft() == 1 &&
			l.getupper().getright() == 4)) {
		cout << endl;
		temp.displayCube();
		throw iv;
	}
	cout << flush;
}

void orientFinal(Cube &c, vector<Cube> &cubeList) {
	/* Reference: 0 = up, 1 = right, 2 = left, 3 = front, 4 = back, 5 = down */
	if (c.getSide(5).same() && c.getSide(5).getupper().getleft() == 5) {
		cout << "Flipping the cube: ";
		c.L2();
		c.R2();
		cout << endl; 
		cout << "Side is already solved.";
		return;
	}
	if (!c.isValid()) { throw iv; }

	/* Color notation:
	* 0: white
	* 1: red
	* 2: blue
	* 3: orange
	* 4: green
	* 5: yellow
	*/

	Cube temp = c;

	cout << "Flipping the cube: ";
	c.L2();
	c.R2();
	cout << endl;

	Side u = c.getSide(0);
	Side b = c.getSide(4);
	Side l = c.getSide(2);
	Side r = c.getSide(1);
	Side f = c.getSide(3);
	Side d = c.getSide(5);
	
	/* Determine which of the orientations the cube is in */
	int ori = -1; // orientation
	cubeList.push_back(c);
	cout << "To align: ";
	string n = "No alignment necessary.";
	// U orientation == 0
	if ((u.getupper().getright() == 5 && u.getlower().getright() == 5) &&
		(l.getupper().getleft() == 5 && l.getupper().getright() == 5)){
		cout << n;
		ori= 0;
	}
	else if ((u.getupper().getleft() == 5 && u.getupper().getright() == 5) &&
		(f.getupper().getleft() == 5 && f.getupper().getright() == 5)) {
		c.U();
		ori = 0;
	}
	else if ((u.getupper().getleft() == 5 && u.getlower().getleft() == 5) &&
		(r.getupper().getleft() == 5 && r.getupper().getright() == 5)) {
		c.U2();
		ori = 0;
	}
	else if ((u.getlower().getleft() == 5 && u.getlower().getright() == 5) &&
		(b.getupper().getleft() == 5 && b.getupper().getright() == 5)) {
		c.Up();
		ori = 0;
	}
	// T orientation == 1
	else if ((u.getupper().getright() == 5 && u.getlower().getright() == 5) &&
		(b.getupper().getright() == 5 && f.getupper().getleft() == 5)) {
		cout << n;
		ori = 1;
	}
	else if ((u.getupper().getleft() == 5 && u.getupper().getright() == 5) &&
		(l.getupper().getright() == 5 && r.getupper().getleft() == 5)) {
		c.U();
		ori = 1;
	}
	else if ((u.getupper().getleft() == 5 && u.getlower().getleft() == 5) &&
		(b.getupper().getleft() == 5 && f.getupper().getright() == 5)) {
		c.U2();
		ori = 1;
	}
	else if ((u.getlower().getleft() == 5 && u.getlower().getright() == 5) &&
		(l.getupper().getleft() == 5 && r.getupper().getright() == 5)) {
		c.Up();
		ori = 1;
	}
	// L orientation == 2
	else if ((u.getupper().getleft() == 5 && u.getlower().getright() == 5) &&
		(r.getupper().getright() == 5 && f.getupper().getleft() == 5)) {
		cout << n;
		ori = 2;
	}
	else if ((u.getupper().getright() == 5 && u.getlower().getleft() == 5) &&
		(b.getupper().getright() == 5 && r.getupper().getleft() == 5)) {
		c.U();
		ori = 2;
	}
	else if ((u.getupper().getleft() == 5 && u.getlower().getright() == 5) &&
		(l.getupper().getright() == 5 && b.getupper().getleft() == 5)) {
		c.U2();
		ori = 2;
	}
	else if ((u.getupper().getright() == 5 && u.getlower().getleft() == 5) &&
		(f.getupper().getright() == 5 && l.getupper().getleft() == 5)) {
		c.Up();
		ori = 2;
	}
	// S orientation == 3
	else if ((u.getlower().getleft() == 5 && b.getupper().getright() == 5) &&
		(r.getupper().getright() == 5 && f.getupper().getright() == 5)) {
		cout << n;
		ori = 3;
	}
	else if ((u.getlower().getright() == 5 && l.getupper().getright() == 5) &&
		(b.getupper().getright() == 5 && r.getupper().getright() == 5)) {
		c.U();
		ori = 3;
	}
	else if ((u.getupper().getright() == 5 && f.getupper().getright() == 5) &&
		(l.getupper().getright() == 5 && b.getupper().getright() == 5)) {
		c.U2();
		ori = 3;
	}
	else if ((u.getupper().getleft() == 5 && r.getupper().getright() == 5) &&
		(f.getupper().getright() == 5 && l.getupper().getright() == 5)) {
		c.Up();
		ori = 3;
	}
	// As orientation == 4
	else if ((u.getupper().getright() == 5 && r.getupper().getleft() == 5) &&
		(f.getupper().getleft() == 5 && l.getupper().getleft() == 5)) {
		cout << n;
		ori = 4;
	}
	else if ((u.getupper().getleft() == 5 && b.getupper().getleft() == 5) &&
		(f.getupper().getleft() == 5 && r.getupper().getleft() == 5)) {
		c.U();
		ori = 4;
	}
	else if ((u.getlower().getleft() == 5 && l.getupper().getleft() == 5) &&
		(b.getupper().getleft() == 5 && r.getupper().getleft() == 5)) {
		c.U2();
		ori = 4;
	}
	else if ((u.getlower().getright() == 5 && f.getupper().getleft() == 5) &&
		(l.getupper().getleft() == 5 && b.getupper().getleft() == 5)) {
		c.Up();
		ori = 4;
	}
	// Pi orientation == 5
	else if ((b.getupper().getleft() == 5 && f.getupper().getright() == 5) &&
		(l.getupper().getleft() == 5 && l.getupper().getright() == 5)) {
		cout << n;
		ori = 5;
	}
	else if ((l.getupper().getleft() == 5 && r.getupper().getright() == 5) &&
		(f.getupper().getleft() == 5 && f.getupper().getright() == 5)) {
		c.U();
		ori = 5;
	}
	else if ((b.getupper().getright() == 5 && f.getupper().getleft() == 5) &&
		(r.getupper().getleft() == 5 && r.getupper().getright() == 5)) {
		c.U2();
		ori = 5;
	}
	else if ((l.getupper().getright() == 5 && r.getupper().getleft() == 5) &&
		(b.getupper().getleft() == 5 && b.getupper().getright() == 5)) {
		c.Up();
		ori = 5;
	}
	// H orientation == 6
	else if ((b.getupper().getleft() == 5 && b.getupper().getright() == 5) &&
		(f.getupper().getleft() == 5 && f.getupper().getright() == 5)) {
		cout << n;
		ori = 6;
	}
	else if ((l.getupper().getleft() == 5 && l.getupper().getright() == 5) &&
		(r.getupper().getleft() == 5 && r.getupper().getright() == 5)) {
		c.U();
		ori = 6;
	}
	else {
		temp.displayCube();
		throw iv;
	}
	cout << flush << endl;
	if (c.getSide(5).same() && c.getSide(5).getupper().getleft() == 5) {
		cout << "Side is solved.";
		return;
	}

	/* Solve based on orientation */
	temp = c;
	cubeList.push_back(c);
	cout << "Solve layer and orient sides: ";
	switch (ori) {
	case 0: 
		c.F();
		cout << "( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ";
		c.Fp();
		break;
	case 1:
		cout << "( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.F();
		c.R();
		c.Fp();
		cout << ")";
		break;
	case 2:
		c.F();
		cout << "( ";
		c.R();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.Up();
		c.R();
		c.U();
		cout << ") ( ";
		c.Rp();
		c.Fp();
		cout << ")";
		break;
	case 3:
		cout << "( ";
		c.R();
		c.U();
		c.Rp();
		c.U();
		cout << ") ( ";
		c.R();
		c.U2();
		c.Rp();
		cout << ")";
		break;
	case 4:
		cout << "( ";
		c.R();
		c.U2();
		c.Rp();
		c.Up();
		cout << ") ( ";
		c.R();
		c.Up();
		c.Rp();
		cout << ")";
		break;
	case 5:
		c.F();
		cout << "( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ";
		c.Fp();
		break;
	case 6:
		c.R2();
		c.U2();
		c.R();
		c.U2();
		c.R2();
		break;
	default: throw iv;
	}

	u = c.getSide(0);
	if (!(u.same() && u.color == 5)) {
		cout << endl;
		temp.displayCube();
		throw "Invalid cube or logic error -- cube cannot be solved.";
	}
	cout << flush;
}

void solveFinal(Cube &c, vector<Cube> &cubeList) {
	/* Reference: 0 = up, 1 = right, 2 = left, 3 = front, 4 = back, 5 = down */
	if (c.isSolved()) {
		cout << "The final layer is already solved.";
		return;
	}
	if (!c.isValid()) { throw iv; }

	/* Color notation:
	* 0: white
	* 1: red
	* 2: blue
	* 3: orange
	* 4: green
	* 5: yellow
	*/

	Cube temp = c;

	Side u = c.getSide(0);
	Side b = c.getSide(4);
	Side l = c.getSide(2);
	Side r = c.getSide(1);
	Side f = c.getSide(3); // orange is now the front side
	Side d = c.getSide(5);

	/* Determine which of the two cases we're in */
	int ori = -1;
	string a = "No alignment necessary.";
	cout << "To align: ";
	// If already solved, but not aligned
	if (f.getupper().same() && l.getupper().same()
		&& b.getupper().same() && r.getupper().same()) {
		if (f.getupper().getleft() == 4){
			c.U();
		}
		else if (f.getupper().getleft() == 2){
			c.Up();
		}
		else{
			c.U2();
		}
	}
	// Adjacent corners
	else if (f.getupper().same()) {
		if (f.getupper().getleft() == 3) c.Dp();
		else if (f.getupper().getleft() == 1) c.D();
		else if (f.getupper().getleft() == 2) c.D2();
		c.U();
		ori = 0;
	}
	else if (l.getupper().same()) {
		if (l.getupper().getleft() == 4) cout << a;
		else if (l.getupper().getleft() == 3) c.Dp();
		else if (l.getupper().getleft() == 1) c.D();
		else if (l.getupper().getleft() == 2) c.D2();
		ori = 0;
	}
	else if (b.getupper().same()) {
		if (b.getupper().getleft() == 3) c.Dp();
		else if (b.getupper().getleft() == 1) c.D();
		else if (b.getupper().getleft() == 2) c.D2();
		c.Up();
		ori = 0;
	}
	else if (r.getupper().same()) {
		if (r.getupper().getleft() == 3) c.Dp();
		if (r.getupper().getleft() == 1) c.D();
		if (r.getupper().getleft() == 2) c.D2();
		c.U2();
		ori = 0;
	}
	// Diagonal corners
	else if (opposite(f.getupper().getleft(), b.getupper().getleft())) {
		if (f.getupper().getleft() == 3) cout << a;
		else if (f.getupper().getright() == 3) c.U2();
		else if (f.getupper().getleft() == 4) c.Up();
		else if (f.getupper().getright() == 4) c.U();
		ori = 1;
	}
	else {
		temp.displayCube();
		throw iv;
	}
	cout << flush << endl;
	if (c.isSolved()) {
		cout << "The final layer is solved.";
		return;
	}

	/* Perform algorithm */
	temp = c;
	if (!ori ) {
		cout << "Adjacent corners algorithm: ";
		cout << "( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.F();
		cout << ") ( ";
		c.R2();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.Up();
		c.R();
		c.U();
		cout << ") ( ";
		c.Rp();
		c.Fp();
		cout << ")";
	}
	else if (ori == 1) {
		cout << "Diagonal corners algorithm: ";
		cout << "( ";
		c.F();
		c.R();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.Up();
		c.R();
		c.U();
		cout << ") ( ";
		c.Rp();
		c.Fp();
		cout << ") ( ";
		c.R();
		c.U();
		c.Rp();
		c.Up();
		cout << ") ( ";
		c.Rp();
		c.F();
		c.R();
		c.Fp();
		cout << ")";
	}
	else {
		temp.displayCube();
		throw iv;
	}
}

vector<Cube> beginnersMethod(Cube &c) {
	/* Solve cube using beginner's method */
	cout << endl << "----- SOLUTION -----" << endl;
	cout << "STEP 1: To solve the first layer:" << endl;
	vector<Cube> cubeList = {c};
	firstLayer(c, cubeList);
	cout << endl << endl << "Your cube should now look like this:";
	c.displayCube();
	cubeList.push_back(c);
	cout << "STEP 2: To orient the final layer:" << endl;
	orientFinal(c, cubeList);
	cout << endl << endl << "Your cube should now look like this:";
	c.displayCube();
	cubeList.push_back(c);
	cout << "STEP 3: To solve the final layer:" << endl;
	solveFinal(c, cubeList);
	cout << endl << endl;
	if (c.isSolved()) {
		cout << "Your cube is solved." << endl << endl;
	}
	else cout << "Logic error: cube cannot be solved." << endl << endl;
	
	while(c.front.getupper().getleft()!=4){
		c.U();
		c.Dp();
	}
	
	cubeList.push_back(c);
	return cubeList;
}



