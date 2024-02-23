#ifndef Beginner_h
#define Beginner_h
#include "Cube.h"

using namespace std;

vector<int> getAllState(Cube c);
void messWithCube(int remainingMove, vector<int> &similarList, vector<vector<int>> &actionSeriesList, vector<int> currentActionSeries, Cube &c, vector<int> allStateOriginal);
vector<Cube> beginnersMethod(Cube &c);
void inputSolvedCube(Cube &c);
void cubeManip(Cube &c);
void generateTestCube(Cube &c, int iteration);


/* Algorithm notation: 
 * U, R, L, F, B, D corresponds to the up, right, left, front, back, and down faces.
 * The default is clockwise; ' (prime) denotes counterclockwise.
 */

/* Color notation:
 * 0: white
 * 1: red
 * 2: blue
 * 3: orange
 * 4: green
 * 5: yellow
 */
 
#endif
