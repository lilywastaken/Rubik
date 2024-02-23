#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <stdio.h>
#include <set>

#include "Cube.h"
#include "Beginner.h"

using namespace std;

// Original values
struct Set{
	string name;
	vector<int> valueList;
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



// Swap (0:4), Turn (0:8)
void directAction(vector<int> actionList, Cube &c){
	// Change side
	if(actionList[0] != 0) c.moveSide(actionList[0]-1);
	// Rotate cube
	switch(actionList[1]){
	case 1: c.U(); break;
	case 2: c.D(); break;
	case 3: c.R(); break;
	case 4: c.L(); break;
	}
}

void loadSet(vector<Set*>& setList, Cube &c){
    setList[0]->valueList = {c.front.getupper().getleft(), c.front.getupper().getright(), c.front.getlower().getleft(), c.front.getlower().getright()};
    setList[1]->valueList = {c.right.getupper().getleft(), c.right.getupper().getright(), c.right.getlower().getleft(), c.right.getlower().getright()};
    setList[2]->valueList = {c.back.getupper().getleft(), c.back.getupper().getright(), c.back.getlower().getleft(), c.back.getlower().getright()};
    setList[3]->valueList = {c.left.getupper().getleft(), c.left.getupper().getright(), c.left.getlower().getleft(), c.left.getlower().getright()};
    setList[4]->valueList = {c.up.getupper().getleft(), c.up.getupper().getright(), c.up.getlower().getleft(), c.up.getlower().getright()};
    setList[5]->valueList = {c.down.getupper().getleft(), c.down.getupper().getright(), c.down.getlower().getleft(), c.down.getlower().getright()};
}

void printPCV(PCV pcv){
	if(pcv.reversed) cout << "  Diff value : ";
	else cout << "  Same value : ";
	for(pair<Set*,int> value : pcv.valueList) cout << value.first->name << "/" << value.second << " ";
	cout << endl;
}

void printPEV(PEV pev){
	if(pev.reversed) cout << "  Not equal : ";
	else cout << "  Equal : ";
	for(pair<Set*,int> value : pev.valueList) cout << value.first->name << "/" << value.second << " ";
	cout << "with {";
	for(int equalValue : pev.equalValueList) cout << equalValue << " ";
	cout << "}" << endl;
}

void printSC(SC sc){
	for(PCV pcv : sc.pcvList) printPCV(pcv);
	for(PEV pev : sc.pevList) printPEV(pev);
}

void printTC(TC tcOriginal){
	for(int i=0; i<tcOriginal.conditionList.size(); i++){
		auto subCond = tcOriginal.conditionList[i];
		cout << "Sub condition " << i << ";" << endl;
		
		if(subCond.first.size()>0){
			cout << "  Other TC : ";
			for(TC tc : subCond.first) cout << tc.name << " ";
			cout << endl;
		}
		
		for(SC sc : subCond.second) printSC(sc);
	}
}

bool checkSC(SC scOriginal){
	for(PCV pcv : scOriginal.pcvList){
		vector<int> valueList;
		for(pair<Set*,int> value : pcv.valueList) valueList.push_back(value.first->valueList[value.second]);
		
		/*cout << "Check equal (" << pcv.reversed << "): ";
		for(int element : valueList) cout << element << " ";
		cout << endl;*/
		
		if(pcv.reversed){
			set<int> uniqueValues(valueList.begin(), valueList.end());
			if(uniqueValues.size()!=valueList.size()) return false;
		}
		else{			
			int firstVal = valueList[0];
			for(int i=1; i<valueList.size(); i++){
				if(valueList[i]!=firstVal) return false;
			}
		}
	}
	for(PEV pev : scOriginal.pevList){
	
		/*cout << "Check in list (" << pev.reversed << "): ";
		for(pair<Set,int> element : pev.valueList) cout << element.first.valueList[element.second] << " ";
		cout << "{";
		for(int element : pev.equalValueList) cout << element << " ";
		cout << "}" << endl;*/
		
		if(pev.reversed){
			for(pair<Set*,int> value : pev.valueList){
				int currentVal = value.first->valueList[value.second];
				if(find(pev.equalValueList.begin(), pev.equalValueList.end(), currentVal)!=pev.equalValueList.end()) return false;
			}
		}
		else{
			for(pair<Set*,int> value : pev.valueList){
				int currentVal = value.first->valueList[value.second];
				if(find(pev.equalValueList.begin(), pev.equalValueList.end(), currentVal)==pev.equalValueList.end()) return false;
			}
		}
	}
	return true;
}

bool checkTC(TC tcOriginal){
	for(int i=0; i<tcOriginal.conditionList.size(); i++){
		auto subCond = tcOriginal.conditionList[i];
		cout << "==== SUBCOND " << i << " ====" << endl;
		
		// Check top-condition
		bool checkedTC(true);
		for(TC tc : subCond.first){
			if(!checkTC(tc)){
				checkedTC=false;
				break;
			}
		}
		if(!checkedTC){
			cout << "Failure" << endl;
			continue;
		}
		
		// Check sub-condition
		for(SC sc : subCond.second){
			if(!checkSC(sc)) break;
			cout << "Success" << endl;
			return true;
		}
		
		cout << "Failure" << endl;
	}
	return false;
}

vector<PCV> stateComposition(vector<Set*>& setList, Cube &c){
	loadSet(setList, c);
	//c.displayCube();
	
	vector<PCV> patternCommonValueList;
	
	for(int i=0; i<setList.size(); i++){
		Set *firstSet = setList[i];
		for(int j=0; j<firstSet->valueList.size(); j++){
		
			int firstVal(firstSet->valueList[j]);
		
			bool valueAlreadyStudied(false);
			for(PCV patternCommonValue : patternCommonValueList){
			
				int previousValue(patternCommonValue.valueList[0].first->valueList[patternCommonValue.valueList[0].second]);
				
				if(previousValue == firstVal){
					valueAlreadyStudied=true;
					break;
				}
			}
			
			if(valueAlreadyStudied) continue;
		
			PCV patternCommonValue;
			patternCommonValue.valueList.push_back({firstSet,j});
			for(int k=i; k<setList.size(); k++){
				Set *secondSet = setList[k];
				for(int l=0; l<secondSet->valueList.size(); l++){
					if(i==k && l<=j) continue;
					int secondVal(secondSet->valueList[l]);
					if(firstVal == secondVal) patternCommonValue.valueList.push_back({secondSet,l});
				}
			}
			
			patternCommonValueList.push_back(patternCommonValue);
		}
	}
	
	//for(PCV pcv : patternCommonValueList) printPCV(pcv);
	//cout << endl;
	return patternCommonValueList;
	
}

vector<PCV> correctComposition(vector<PCV> pCVL1, vector<PCV> pCVL2){
	
	vector<PCV> correctedPCVList;
	
	for(PCV pcv1 : pCVL1){
		PCV retainedPCV;
		int maxSize(1);
		for(PCV pcv2 : pCVL2){
			
			PCV correctedPCV;
			
			for(pair<Set*,int> value1 : pcv1.valueList){
				for(pair<Set*,int> value2 : pcv2.valueList){
					
					if(value1.first->name == value2.first->name && value1.second == value2.second){
						correctedPCV.valueList.push_back(value1);
					}
				}
			}
			
			//cout << "========" << endl;
			//printPCV(pcv1);
			//printPCV(pcv2);
			if(correctedPCV.valueList.size()>maxSize){
				//cout << "  Retained " << correctedPCV.valueList.size() << endl;
				maxSize = correctedPCV.valueList.size();
				retainedPCV = correctedPCV;
			}
		}
		if(retainedPCV.valueList.size()>0) correctedPCVList.push_back(retainedPCV);
		//exit(1);
	}
	
	//for(PCV pcv : correctedPCVList) printPCV(pcv);
	//cout << endl;
	return correctedPCVList;
}


vector<pair<Set*,int>> getSimilarList(vector<Set> setListPrev, vector<Set*> &setListNew){
	vector<pair<Set*,int>> similarList;
	
	for(int i=0; i<setListPrev.size(); i++){
		Set setPrev = setListPrev[i];
		Set *setNew = setListNew[i];
		
		for(int j=0; j<setPrev.valueList.size(); j++){
			if(setPrev.valueList[j]==setNew->valueList[j]) similarList.push_back({setNew,j});
		}
		
	}
	
	return similarList;
	
}

streambuf* mute(){
	ofstream null_stream("/dev/null");
	streambuf* old_cout = cout.rdbuf();
	cout.rdbuf(null_stream.rdbuf());
	return old_cout;
}

void activate(streambuf* old_cout){
	cout.rdbuf(old_cout);
}

int main(){



	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	// INSTRUCTION :
	// 1) Represent states using patterns only
	// 2) Try to reach step 1, then step 2, etc
	// 3) While trying to reach states, remember the useful operations
	// 4) Create logic to reach states from experimentation (hard)
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////









	cout << "Welcome to the 2x2 Rubik's Cube Solver!" << endl << endl;
	
	// CUBE INIT
	////////////////////////////////////////////////////

    srand(23);

	Cube c, solvedCube;
	inputSolvedCube(solvedCube);
	vector<vector<Cube>> testCubeSuperList;
		
	Set setFront{"front"}, setRight{"right"}, setBack{"back"}, setLeft{"left"}, setUp{"up"}, setDown{"down"};
	vector<Set*> setList = {&setFront, &setRight, &setBack, &setLeft, &setUp, &setDown};
	
	// MODIFY TO GET VECTOR OF STATE PER CUBE
	for(int i=0; i<100; i++){
	
		c = solvedCube;
		
		streambuf* old_cout = mute();
		
		generateTestCube(c, 5000);
		vector<Cube> testCubeList = beginnersMethod(c);
		
		activate(old_cout);
		
		for(int j=1; j<testCubeList.size(); j++){
			if(testCubeList[j] == testCubeList[j-1]){
        		testCubeList.erase(testCubeList.begin() + j);
				j--;
			}
		}
		
		if(testCubeList.size()==9) testCubeSuperList.push_back(testCubeList);
		else i--;
	}
	
	//return 1;
	
	// SEARCH SIMILARITIES
	////////////////////////////////////////////////////
	
	vector<SC> scStateList;
	
	// For each state of the resolution
	for(int j=0; j<testCubeSuperList[0].size(); j++){
	
		SC scState;
		
		loadSet(setList, testCubeSuperList[0][j]);
		vector<Set> initList;
		for(Set* ptr : setList) initList.push_back(*ptr);
			
		vector<pair<Set*,int>> similarList = getSimilarList(initList, setList);
		
		// For each cube
		for(int i=1; i<testCubeSuperList.size(); i++){
		
			//testCubeSuperList[0][1].displayCube();
			loadSet(setList, testCubeSuperList[i][j]);
			//testCubeSuperList[i][1].displayCube();
			
			vector<Set> setListPrev;
			for(Set* ptr : setList) setListPrev.push_back(*ptr);
			
			loadSet(setList, testCubeSuperList[i-1][j]);
			
			vector<pair<Set*,int>> newSimilarList = getSimilarList(setListPrev, setList);
			
			
			//cout << "==========" << endl;
			vector<pair<Set*,int>> correctedList;
			for(pair<Set*,int> similar : similarList){
				for(pair<Set*,int> newSimilar : newSimilarList){
					if(similar == newSimilar){
						correctedList.push_back(newSimilar);
						break;
					}
				}
			}
			
			similarList = correctedList;
		
		}
		
		//cout << "===============" << endl;
		if(similarList.size()==0); //cout << "No similarity" << endl;
		
		else{
			for(auto similar : similarList){
				//cout << "Similar " << j << ": " << similar.first->name << " -> " << similar.second << endl;
				
				loadSet(setList, testCubeSuperList[0][j]);
				
				PEV equalValue;
				equalValue.valueList.push_back(similar);
				equalValue.equalValueList.push_back(similar.first->valueList[similar.second]);
				
				scState.pevList.push_back(equalValue);
				
			}
		}
		scStateList.push_back(scState);
	}
	
	// States to reach
	cout << endl << "States to reach" << endl;
	cout << "=============" << endl;
	for(int i=0; i<scStateList.size(); i++){
		cout << "State " << i << ":" << endl;
		printSC(scStateList[i]);
	}
	
	cout << endl;
	generateTestCube(c, 20);
	
	cout << "Goal: study path to state 1" << endl;
		
	
	return 1;
	
	// ALL VARIABLES / CONDITIONS
	////////////////////////////////////////////////////
	
	// SEARCH PATTERN
	
	PCV sUp, cF, cR1, cR2, cB, cL;
	sUp.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	cF.valueList = {{&setFront,0},{&setFront,1}};
	cR1.valueList = {{&setRight,0},{&setRight,1}};
	cR2.valueList = {{&setRight,2},{&setRight,3}};
	cB.valueList = {{&setBack,0},{&setBack,1}};
	cL.valueList = {{&setLeft,0},{&setLeft,1}};
	vector<PCV> crownState = {cF, cB, cR1, cL, sUp};
	
	PCV pcvA, pcvB, pcvC, pcvD, pcvE, pcvF;
	pcvA.valueList = {{&setFront,0},{&setFront,1},{&setFront,3},{&setDown,2}};
	pcvB.valueList = {{&setBack,0},{&setBack,1},{&setFront,2},{&setDown,3}};
	pcvC.valueList = {{&setRight,0},{&setRight,1},{&setBack,2},{&setBack,3}};
	pcvD.valueList = {{&setLeft,0},{&setLeft,1},{&setDown,0},{&setDown,1}};
	pcvE.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	pcvF.valueList = {{&setLeft,2},{&setLeft,3},{&setRight,2},{&setRight,3}};
	vector<PCV> testState = {pcvA, pcvB, pcvC, pcvD, pcvE, pcvF};
	
	//for(PCV pcv : crownState) printPCV(pcv);
	//cout << endl;
	
	// Compare every state
	for(int i=0; i<testCubeSuperList[0].size()*0+1; i++){
		
		vector<PCV> pCVL1 = stateComposition(setList, testCubeSuperList[0][i]);
		vector<PCV> pCorrected = pCVL1;
		
		//pCorrected = testState;
		pCorrected = crownState;
		
		for(int j=1; j<testCubeSuperList.size(); j++){
			
			int bestCorr(0);
			vector<PCV> bestPCVList;
			
			for(int k=0; k<testCubeSuperList[j].size()-2; k++){
			
				vector<PCV> pCVL2 = stateComposition(setList, testCubeSuperList[j][k]);
				//cout << "CHECKING" << endl;
				vector<PCV> pchecking = correctComposition(pCorrected, pCVL2);
				//cout << "FINISHED" << endl;
				
				//cout << "Check:" << endl;
				//for(PCV pcv : pchecking) printPCV(pcv);
				
				//testCubeSuperList[j][k].displayCube();
				
				int currentDataSize(0);
				for(PCV pcv : pchecking) currentDataSize += pcv.valueList.size();
				if(currentDataSize > bestCorr){
					//cout << "Better: " << k << endl;
					bestCorr = currentDataSize;
					bestPCVList = pchecking;
				}
			}
			pCorrected = bestPCVList;
			
		}
		cout << "Best:" << endl;
		for(PCV pcv : pCorrected) printPCV(pcv);
		
	}
	
	return 1;

	// ALL PATTERN WE SHOULD FIND
	////////////////////////////////////////////////////
	
	// White side correct
	PEV sUWhite, sDWhite;
	sUWhite.valueList = {{&setUp,0},{&setUp,1},{&setUp,2},{&setUp,3}};
	sUWhite.equalValueList = {0};
	sDWhite.valueList = {{&setDown,0},{&setDown,1},{&setDown,2},{&setDown,3}};
	sDWhite.equalValueList = {0};
	
	// Crown correct
	PCV sFUp, sFDown, sRUp, sRDown, sBUp, sBDown, sLUp, sLDown;
	sFUp.valueList = {{&setFront,0},{&setFront,1}};
	sFDown.valueList = {{&setFront,2},{&setFront,3}};
	sRUp.valueList = {{&setRight,0},{&setRight,1}};
	sRDown.valueList = {{&setRight,2},{&setRight,3}};
	sBUp.valueList = {{&setBack,0},{&setBack,1}};
	sBDown.valueList = {{&setBack,2},{&setBack,3}};
	sLUp.valueList = {{&setLeft,0},{&setLeft,1}};
	sLDown.valueList = {{&setLeft,2},{&setLeft,3}};
		
	// VERIFY CROWN STATE
	////////////////////////////////////////////////////
	
	// Requirement : all 6 faces set, comparition of all crown results

	SC crownCompleteUp;
	crownCompleteUp.pcvList = {sFUp, sRUp, sBUp, sLUp};
	crownCompleteUp.pevList = {sUWhite};
	
	SC crownCompleteDown;
	crownCompleteDown.pcvList = {sFDown, sRDown, sBDown, sLDown};
	crownCompleteDown.pevList = {sDWhite};

	TC crownComplete{"crownComplete"};
	crownComplete.conditionList = {{{},{crownCompleteDown}},
								   {{},{crownCompleteUp}}};
	
	printTC(crownComplete);					   
	bool crownCompleteChecked = checkTC(crownComplete);
	
	return 1;
	
	// PROCESSING
	////////////////////////////////////////////////////
	
	
	// 1) I want to see how to achieve my goal
	
	// Check if face1 complete
	
	/*systemAction({1,0,2,0}, probeValue, trashCond, c);
	systemAction({0,0,0,1}, probeValue, t2ok, c);
	systemAction({0,0,3,0}, probeValue, trashCond, c);
	systemAction({0,0,0,2}, probeValue, t3ok, c);
	systemAction({0,0,4,0}, probeValue, trashCond, c);
	systemAction({0,0,0,3}, probeValue, t4ok, c);*/
	
	// 2) Direct way (solved) not functionnal, checking way (4-correct) to solved
	
	// 3) Way to solved (4-correct) not functionnal, checking way (crown) to 4-correct
	
	// 4) Way to 4-correct (crown) FUNCTIONNAL, applying protocol crown->4-correct
	
	
	
	// check if one face is white
	// check if crown correct
	
	
	
	
	c.displayCube();
	
	return 1;
	
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	
	
	//Scond face1SolvedCond;
	//Scond rubikSolvedCond = {{}};
	
	bool face1Complete(false), face2Complete(false), face3Complete(false),
	face4Complete(false), face5Complete(false), face6Complete(false);
	
	// rubik's cube solved
	bool rubikSolved = face1Complete && face2Complete && face3Complete
						&& face4Complete && face5Complete && face6Complete;
	
	////////////////////////////////////////////////////
	////////////////////////////////////////////////////
	
	//return 1;
	
	// Orientate it like in the video
	c.moveSide(1);
	c.moveSide(1);
	c.moveSide(2);
	c.moveSide(2);
	
	c.displayCube();
	
	/////////////////////////////////////
	// SEARCH WHITE CROWN
	/////////////////////////////////////
	
	
	bool fullWhite(false);
	bool crown1(false), crown2(false), crown3(false), crown4(false);
	bool noDoubleCrown(false);
	
	vector<int> seriesOfMove = {1,1,1,2,2,2,-1};
	vector<int> crownColors;
	
	// Check white side
	for(int move : seriesOfMove){
		if(c.front.getupper().getleft()==0
		&& c.front.getupper().getright()==0
		&& c.front.getlower().getleft()==0
		&& c.front.getlower().getright()==0){
			cout << "Found you" << endl;
			fullWhite = true;
			break;
		}
		cout << "Move " << move << endl;
		c.moveSide(move);
	}
	
	c.displayCube();
	
	// Check crown 1
	c.moveSide(2);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown1 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 2
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown2 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 3
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown3 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check crown 4
	c.moveSide(1);
	c.displayCube();
	if(c.front.getlower().getleft() == c.front.getlower().getright()){
		crown4 = true;
		crownColors.push_back(c.front.getlower().getleft());
	}
	
	// Check no double crown
	if(crownColors[0] != crownColors[1] && crownColors[0] != crownColors[2] && crownColors[0] != crownColors[3]
	&& crownColors[1] != crownColors[2] && crownColors[1] != crownColors[3] && crownColors[2] != crownColors[3]){
		noDoubleCrown = true;
	}
	
	if(fullWhite && crown1 && crown2 && crown3 && crown4 && noDoubleCrown){
		cout << "FUNCTIONNAL" << endl;
	}
	
	// PROTOCOL CROWN -> 4-CORRECT
	
	bool solved1(false), solved2(false), solved3(false), solved4(false);
	
	c.moveSide(1);
	c.moveSide(1);
	c.displayCube();
	
	int val1, val2, val3, val4, val5;
	bool val1Valid, val2Valid, val3Valid;
	
	// Check solved 1
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved1 = true;
	
	// Check solved 2
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved2 = true;
	
	// Check solved 3
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved3 = true;
	
	
	// Check solved 4
	c.moveSide(1);
	c.displayCube();
	
	val1 = c.front.getupper().getright(), val2 = c.up.getlower().getright(), val3 = c.right.getupper().getleft();
	val4 = c.front.getlower().getright(), val5 = c.right.getlower().getleft();
	val1Valid = false, val2Valid = false, val3Valid = false;
	if(val1 == val4 || val1 == val5) val1Valid = true;
	if(val2 == val4 || val2 == val5) val2Valid = true;
	if(val3 == val4 || val3 == val5) val3Valid = true;
	if(val1Valid && val2Valid || val1Valid && val3Valid || val2Valid && val3Valid) solved4 = true;
	
	cout << solved1 << "/" << solved2 << "/" << solved3 << "/" << solved4 << endl;
	
	// PROTOCOL 4-CORRECT -> SOLVED
	
	// Search color section on top
	int lastColor;
	for(int i=1; i<6; i++){
		bool colorIn(false);
		for(int color : crownColors){
			if(color==i){
				colorIn = true;
				break;
			}
		}
		if(!colorIn){
			lastColor = i;
			break;
		}
	}
	
	cout << "Last color: " << lastColor << endl;
	
	// Orientate it like in the video
	c.moveSide(1);
	c.moveSide(1);
	c.moveSide(2);
	c.moveSide(2);
	
	while(c.front.getlower().getright() != lastColor && c.front.getlower().getleft() != lastColor){
		c.moveSide(1);
	}	
	
	while(!c.isSolved()){
	//for(int i=0; i<2; i++){
		
		//if(c.isSolved()) break;
		while(c.front.getlower().getright() != lastColor && c.front.getlower().getleft() != lastColor && !c.isSolved()){
			c.Dp();
		}
		
		if(c.isSolved()) break;
		c.displayCube();
		
		bool leftPart(false);
		if(c.front.getlower().getleft() == lastColor) leftPart = true;
		
		bool lastColorShifted(false);
		while(!lastColorShifted){
		
			cout << "Turn..." << endl;
		
			c.R();
			c.U();
			c.Rp();
			c.Up();
		
			if(leftPart && c.down.getupper().getleft() == lastColor) lastColorShifted = true;
			else if (c.down.getupper().getright() == lastColor)	lastColorShifted = true;
		}
		
		cout << "Ok" << endl;
	}
	
	cout << "Congrats." << endl;
	
	
	c.displayCube();
	
	
	
	
	
	
	
	
	
	
	// Crown is complete
	
	/*cout << ">> " << c.up.getupper().getleft() << endl;
	cout << ">> " << c.up.getupper().getright() << endl;
	cout << ">> " << c.up.getlower().getleft() << endl;
	cout << ">> " << c.up.getlower().getright() << endl;
	
	cout << ">> " << c.front.getupper().getleft() << endl;
	cout << ">> " << c.front.getupper().getright() << endl;
	
	cout << ">> " << c.back.getupper().getleft() << endl;
	cout << ">> " << c.back.getupper().getright() << endl;
	
	cout << ">> " << c.right.getupper().getleft() << endl;
	cout << ">> " << c.right.getupper().getright() << endl;
	
	cout << ">> " << c.left.getupper().getleft() << endl;
	cout << ">> " << c.left.getupper().getright() << endl;*/
	
	/*c.R();
	c.Rp();
	c.L();
	c.Lp();
	c.F();
	c.Fp();
	c.B();
	c.Bp();
	c.D();
	c.Dp();*/
	
	//beginnersMethod(c);
	
	return 0;
}









