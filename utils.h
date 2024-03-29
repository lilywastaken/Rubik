#ifndef MEM_UTILS_H
#define MEM_UTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <cstdint>
#include <set>
#include <unordered_set>

#include "Cube.h"
#include "Beginner.h"

using namespace std;

//STRUCTURES
////////////////////////

// Original values
struct Set{
	string name;
	vector<int> valueList;
};

struct ActionState{
	vector<int> actionList;
	vector<pair<Set*,int>> firstPos;
	vector<pair<Set*,int>> secondPos;
	bool operator==(const ActionState& other) const {
        return firstPos == other.firstPos &&
               secondPos == other.secondPos;
    }
};

struct Map{
	vector<ActionState> actionStateList; // search minimum action to determine if should check
	vector<bool> outChecked;
};

// Pattern common value
struct PCV{
	vector<pair<Set*,int>> valueList;
	bool reversed = false;
};

// Pattern equal value
struct PEV{
	vector<pair<Set*,int>> valueList;
	vector<int> equalValueList;
	bool reversed = false;
};

// Sub-Condition
struct SC{
	vector<PCV> pcvList;
	vector<PEV> pevList;
	int checkState = 0; // Undefined - false - true
};

// Top-Condition
struct TC{
	string name;
	vector<pair<vector<TC>,vector<SC>>> conditionList; // Respect one of them
	int checkState = 0; // Undefined - false - true
};


//FUNCTIONS
////////////////////////

void loadSet(vector<Set*>& setList, Cube &c);
bool sortSet(const pair<Set*, int>& a, const pair<Set*, int>& b);
bool compareVectors(const vector<bool>& a, const vector<bool>& b);
void insertSorted(vector<vector<bool>>& sortedVector, const vector<bool>& vec);
bool binarySearch(const vector<vector<bool>>& sortedVector, const vector<bool>& target);
size_t generateIdentifier(const pair<Set*,int> &firstPos, const pair<Set*,int> &secondPos);
vector<bool> generateUniqueIdentifiers(const vector<pair<Set*,int>>& firstPos, const vector<pair<Set*,int>>& secondPos);

void directAction(vector<int> actionList, Cube &c);
void printPCV(PCV pcv);
void printPEV(PEV pev);
void printSC(SC sc);
void printTC(TC tcOriginal);
bool checkSC(SC scOriginal);
bool checkTC(TC tcOriginal);
vector<pair<int,int>> searchCommonValue(vector<Set> initList, pair<Set*,int> different);
vector<PCV> stateComposition(vector<Set*>& setList, Cube &c);
vector<PCV> correctComposition(vector<PCV> pCVL1, vector<PCV> pCVL2);
vector<pair<Set*,int>> getSimilarList(vector<Set> setListPrev, vector<Set*> &setListNew);
vector<pair<Set*,int>> getDifferentList(vector<Set> setListPrev, vector<Set*> &setListNew);
void getActionList(int remainingMove, vector<vector<int>> &actionSeriesList, vector<int> currentActionSeries, Cube c);
bool findState(int remainingMove, vector<vector<int>> &actionSeriesList, vector<int> currentActionSeries, vector<Set*>& setList, Cube c, SC scCondition);

void inputSolvedCube(Cube &c);
void cubeManip(Cube &c);
void generateTestCube(Cube &c, int iteration);

streambuf* mute();
void activate(streambuf* old_cout);

#endif


















