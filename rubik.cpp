#include "utils.h"

vector<vector<Cube>> grabCubeState(int amount){

	vector<vector<Cube>> cubeStateSuperList;
	
	Cube c;
	
	for(int i=0; i<amount; i++){
	
		inputSolvedCube(c);
		
		streambuf* old_cout = mute();
		
		generateTestCube(c, 5000);
		vector<Cube> cubeStateList = beginnersMethod(c);
		
		activate(old_cout);
		
		for(int j=1; j<cubeStateList.size(); j++){
			if(cubeStateList[j] == cubeStateList[j-1]){
        		cubeStateList.erase(cubeStateList.begin() + j);
				j--;
			}
		}
		
		if(cubeStateList.size()==8) cubeStateSuperList.push_back(cubeStateList);
		else i--;
	}
	
	return cubeStateSuperList;
}

vector<SC> getCommonSC(vector<vector<Cube>> cubeStateSuperList, vector<Set*> &setList){

	vector<SC> scStateList;

	// For each state of the resolution
	for(int j=0; j<cubeStateSuperList[0].size(); j++){
	
		// Load data from first cube
		SC scState;
		loadSet(setList, cubeStateSuperList[0][j]);
		vector<Set> initList;
		for(Set* ptr : setList) initList.push_back(*ptr);
		
		// Compare with itself to get all characteristics
		vector<pair<Set*,int>> similarList = getSimilarList(initList, setList);
		
		// Compare found characteristics with each other cube
		for(int i=1; i<cubeStateSuperList.size(); i++){
		
			// Load next cube
			loadSet(setList, cubeStateSuperList[i][j]);
			
			// Search similarities
			vector<pair<Set*,int>> newSimilarList = getSimilarList(initList, setList);
			
			// Keep all the same values
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
		
		// For each common pattern between all cubes from a state
		for(auto similar : similarList){
		
			// Load data to store in Sub-Condition state
			loadSet(setList, cubeStateSuperList[0][j]);
			
			// Exctraction of the common values
			PEV equalValue;
			equalValue.valueList.push_back(similar);
			equalValue.equalValueList.push_back(similar.first->valueList[similar.second]);
			
			// Push to Sub-Condition
			scState.pevList.push_back(equalValue);
			
		}
		scStateList.push_back(scState);
	}
	
	return scStateList;

}

Map acquireMap(vector<vector<int>> actionSuperList, vector<Set*> &setList){

	Map map;
	Cube c;

	for(int i=0; i<actionSuperList.size(); i++){
	
		// PHASE 1 : DETECT CHANGES
		vector<pair<Set*,int>> differentList;
		for(int j=0; j<10; j++){
		
			streambuf* old_cout = mute();
			
			generateTestCube(c, 500);
			
			loadSet(setList, c);
			// Create a copy of the states
			vector<Set> initList;
			for(Set* ptr : setList) initList.push_back(*ptr);
			
			for(int action : actionSuperList[i]) directAction({0,action},c);
			
			loadSet(setList, c);
			vector<pair<Set*,int>> newDifferentList = getDifferentList(initList, setList);
			
			activate(old_cout);
			
			for(pair<Set*,int> newDifferent : newDifferentList){
				bool foundDiff(true);
				for(pair<Set*,int> different : differentList){
					if(newDifferent == different){
						foundDiff=false;
						break;
					}
				}
				if(foundDiff) differentList.push_back(newDifferent);
			}
		}
		
		// SORT AND DISPLAY
    	sort(differentList.begin(), differentList.end(), sortSet);
		/*cout << "------------------" << endl;
		for(int action : actionSuperList[i]) cout << action << " ";
		cout << endl;
		for(pair<Set*,int> different : differentList){
			cout << different.first->name << "/" << different.second << endl;
		}
		cout << "==================" << endl;*/
		
		
		vector<pair<Set*,int>> firstPos;
		vector<pair<Set*,int>> secondPos;
		
		// Action representation
		ActionState newActionState;
		newActionState.actionList = actionSuperList[i];
		
		// PHASE 2 : LINK CHANGES
		for(pair<Set*,int> different : differentList){
			vector<pair<int,int>> commonValuefinal;
			for(int j=0; j<10; j++){
			
				streambuf* old_cout = mute();
				generateTestCube(c, 500);
				
				// Extract states
				loadSet(setList, c);
				vector<Set> initList;
				for(Set* ptr : setList) initList.push_back(*ptr);
				
				// Do actions
				for(int action : actionSuperList[i]) directAction({0,action},c);
				
				// Extract states
				loadSet(setList, c);
				activate(old_cout);
				
				vector<pair<int,int>> commonValue = searchCommonValue(initList, different);
				
				if(j==0) commonValuefinal = commonValue;
				else{
					for(int k=0; k<commonValuefinal.size(); k++){
						pair<int,int> value = commonValuefinal[k];
						if(find(commonValue.begin(), commonValue.end(), value) == commonValue.end()){
							commonValuefinal.erase(commonValuefinal.begin() + k);
							k--;
						}
					}
				}
			}
			
			// Now place that in action state :)
			for(pair<int,int> value : commonValuefinal){
				//cout << setList[value.first]->name << "/" << value.second << " goes at " << different.first->name << "/" << different.second << endl;
				newActionState.firstPos.push_back({setList[value.first],value.second});
				newActionState.secondPos.push_back({different});
			}
		}			
		map.actionStateList.push_back(newActionState);
		map.outChecked.push_back(false);
	}
	
	return map;
}


bool checkStateInMap(vector<vector<bool>> &uniqueIdentifiersList, ActionState &newActionState){
	
	vector<bool> uniqueIdentifiers = generateUniqueIdentifiers(newActionState.firstPos, newActionState.secondPos);

    if(binarySearch(uniqueIdentifiersList, uniqueIdentifiers)) return true;
    insertSorted(uniqueIdentifiersList, uniqueIdentifiers);
    return false;
	
}

void getActionStateGeneration(Map &basicMap, Map &fullMap, int ID, vector<vector<bool>> &uniqueIdentifiersList){

	ActionState currentActionState = fullMap.actionStateList[ID];
	
	for(int i=0; i<basicMap.actionStateList.size(); i++){
	
		ActionState basicActionState = basicMap.actionStateList[i];
		
		ActionState newActionState;
		for(int action : currentActionState.actionList) newActionState.actionList.push_back(action);
		for(int action : basicActionState.actionList) newActionState.actionList.push_back(action);
		
		vector<bool> fuzedValueList(basicActionState.firstPos.size(), false);
		vector<bool> touchedValueList(currentActionState.firstPos.size(), false);
		
		for(int j=0; j<basicActionState.firstPos.size(); j++){
			for(int k=0; k<currentActionState.firstPos.size(); k++){
				
				if(currentActionState.secondPos[k] == basicActionState.firstPos[j]){
					if(currentActionState.firstPos[k] != basicActionState.secondPos[j]){
						newActionState.firstPos.push_back(currentActionState.firstPos[k]);
						newActionState.secondPos.push_back(basicActionState.secondPos[j]);
					}
					
					touchedValueList[k] = true;
					fuzedValueList[j] = true;
					break;
				}
			}
		}
		
		for(int j=0; j<currentActionState.firstPos.size(); j++){
			if(touchedValueList[j]) continue;
			newActionState.firstPos.push_back(currentActionState.firstPos[j]);
			newActionState.secondPos.push_back(currentActionState.secondPos[j]);
		}
		
		for(int j=0; j<basicActionState.firstPos.size(); j++){
			if(fuzedValueList[j]) continue;
			newActionState.firstPos.push_back(basicActionState.firstPos[j]);
			newActionState.secondPos.push_back(basicActionState.secondPos[j]);
		}
		
		if(!checkStateInMap(uniqueIdentifiersList, newActionState)){
			fullMap.actionStateList.push_back(newActionState);
			fullMap.outChecked.push_back(false);
		}
		
	}
}

Map expandMap(Map basicMap, int depth){

	Map fullMap;
	fullMap.actionStateList.push_back(ActionState());
	fullMap.outChecked.push_back(false);
	
	int mapSize = fullMap.actionStateList.size();
	vector<vector<bool>> uniqueIdentifiersList;
	uniqueIdentifiersList.push_back(vector<bool>(576, false));
			
	for(int j=0; j<depth; j++){
		cout << "Checking move " << j+1 << "..." << endl;
		
		for(int i=0; i<mapSize; i++){
			if(fullMap.outChecked[i]) continue;
			getActionStateGeneration(basicMap, fullMap, i, uniqueIdentifiersList);
			fullMap.outChecked[i] = true;
		}
		mapSize = fullMap.actionStateList.size();
		cout << "Found outcome: " << fullMap.actionStateList.size() << endl;
	}
	
	return fullMap;

}

vector<pair<Set*,int>> searchEqualValue(vector<Set*> &setList, int equalValue){

	vector<pair<Set*,int>> equalValueList;
	
	for(Set* set : setList){
		for(int i=0; i<set->valueList.size(); i++){
			if(set->valueList[i] == equalValue){
				//cout << "      Found: " << set->name << i << endl;
				equalValueList.push_back({set,i});
			}
		}
	}
	
	return equalValueList;
	
}


int searchAppropriateMove(Map fullMap, vector<vector<pair<Set*,int>>> elementPositionList, vector<pair<Set*,int>> destinationList){
	
	// For every action sequence
	for(int i=0; i<fullMap.actionStateList.size(); i++){
		ActionState actionState = fullMap.actionStateList[i];
		
		vector<bool> respectedMove(destinationList.size(), false);
	
		// For every value shift
		for(int j=0; j<destinationList.size(); j++){
		
			for(pair<Set*,int> element : elementPositionList[j]){
				if(element==destinationList[j]){
					respectedMove[j] = true;
					break;
				}
			}
		
			for(int k=0; k<actionState.firstPos.size(); k++){
			
				// Extract current shift
				pair<Set*,int> firstPos = actionState.firstPos[k];
				pair<Set*,int> secondPos = actionState.secondPos[k];
				
				if(destinationList[j]!=secondPos) continue;
				respectedMove[j] = false;
				
				for(pair<Set*,int> element : elementPositionList[j]){
					if(firstPos == element){
						respectedMove[j] = true;
						break;
					}
				}
				break;
			}
			
			if(respectedMove[j] == false) break;
		}
		
		if(all_of(respectedMove.begin(), respectedMove.end(), [](bool val){ return val; })) return i;			
	}
	
	return -1;
	
}



int main(){

	cout << "Welcome to the 2x2 Rubik's Cube Solver!" << endl << endl;
	
	// CUBE AND SET INIT
	////////////////////////////////////////////////////

    srand(26);
	Cube c;
		
	Set setFront{"front"}, setRight{"right"}, setBack{"back"}, setLeft{"left"}, setUp{"up"}, setDown{"down"};
	vector<Set*> setList = {&setFront, &setRight, &setBack, &setLeft, &setUp, &setDown};
	
	
	// ACQUIRE TEMPORARY STATES FROM CORRECTION
	////////////////////////////////////////////////////
	
	vector<vector<Cube>> cubeStateSuperList = grabCubeState(100);
	
	
	// IDENTIFY SIMILARITIES IN STATES
	////////////////////////////////////////////////////
	
	cout << "Analyzing states from correction..." << endl;
	vector<SC> scStateList = getCommonSC(cubeStateSuperList, setList);
	cout << "List of temporary states to reach established!" << endl << endl;
	
	
	// GENERATE LIST OF ACTIONS (AND THEIR OUTCOME)
	////////////////////////////////////////////////////
	
	inputSolvedCube(c);
	
	vector<vector<int>> actionSuperList;
	
	cout << "Acquiring first set of moves..." << endl;
	getActionList(1, actionSuperList, vector<int>(), c);
	cout << "Acquired!" << endl << endl;
	
	
	// MAP EVERY ACTION
	////////////////////////////////////////////////////
	
	Map basicMap = acquireMap(actionSuperList, setList);
	Map fullMap = expandMap(basicMap, 4);
	
	// DISPLAY SUB-CONDITIONS TO REACH STATES
	////////////////////////////////////////////////////
	
	cout << endl << "States to reach" << endl;
	cout << "=============" << endl;
	for(int i=1; i<scStateList.size(); i++){
		cout << "State " << i << ":" << endl;
		printSC(scStateList[i]);
	}
	
	// REACH ALL STATES FROM KNOWN MOVES ON RANDOM CUBE
	////////////////////////////////////////////////////
	
	cout << endl;
	generateTestCube(c, 20);
	
	cout << "<-------------------------->" << endl;
	cout << "METHOD : EXPLORE ALL COMBINATION" << endl;
	cout << "<-------------------------->" << endl << endl;
    auto start = chrono::high_resolution_clock::now();
	
	// 5 897 milliseconds for 4 trials
	// 68 751 milliseconds for 5 trials
	for(int i=0; i<scStateList.size(); i++){
		cout << "GOAL : STATE " << i << endl;
	
		vector<vector<int>> actionSeriesList;
		
		cout << "----------------" << endl;
		
		int nbTrial = 4;
		for(int j=1; j<nbTrial+1; j++){
	
			if(findState(j, actionSeriesList, vector<int>(), setList, c, scStateList[i])){
				cout << "Found solution [Action: ";
				if(actionSeriesList.size()>0){
					for(int action : actionSeriesList.back()){
						directAction({0, action}, c);
						cout << action << " ";
					}
				}
				cout << "]" << endl;
				break;	
			}
			
			actionSeriesList.clear();
			if(j==nbTrial){
				cout << "No solution found..." << endl;
				c = cubeStateSuperList[0][i];
			}
		}
		
		cout << "----------------" << endl;
	
		c.displayCube();
	}
	
    auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "<==========================>" << endl;
    cout << "Time taken (all): " << int(duration.count()/1000) << " milliseconds" << endl;
	cout << "<==========================>" << endl << endl;
	
	
	generateTestCube(c, 20);
	
	cout << "<-------------------------->" << endl;
	cout << "METHOD : USE GENERATED CHART" << endl;
	cout << "<-------------------------->" << endl << endl;
    start = chrono::high_resolution_clock::now();
	
	// 99 milliseconds for 4 trials
	// 785 milliseconds microseconds for 5 trials
	for(int i=0; i<scStateList.size(); i++){
		loadSet(setList, c);
		cout << "GOAL : STATE " << i << endl;
		
		// LOCATE ALL SPECIFIC VALUES
		vector<vector<pair<Set*,int>>> elementPositionList;
		vector<pair<Set*,int>> destinationList;
		
		// For each PEV condition of the lsit
		for(PEV statePEV : scStateList[i].pevList){
			
			// For each position value of the condition
			for(pair<Set*,int> value : statePEV.valueList){
				
				// For each accepted value
				for(int equalValue : statePEV.equalValueList){
					//cout << endl << "     = " << equalValue << endl;
				
					// Define destination
					destinationList.push_back({value.first, value.second});
					
					// Acquire list of values around
					elementPositionList.push_back(searchEqualValue(setList, equalValue));
				}	
			}
		}
		
		int actionStateID = searchAppropriateMove(fullMap, elementPositionList, destinationList);
		
		cout << "----------------" << endl;
		if(actionStateID!=-1){
			cout << "Found path n°" << actionStateID;
			vector<int> actionSeries = fullMap.actionStateList[actionStateID].actionList;
			cout << " [Action: ";
			for(int action : actionSeries){
				directAction({0, action}, c);
				cout << action << " ";
			}
			cout << "]" << endl;
		}
		else{
			cout << "No path found..." << endl;
			c = cubeStateSuperList[0][i];
		}
		cout << "----------------" << endl;
		
		c.displayCube();
		
	}
	
    end = chrono::high_resolution_clock::now();
	duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "<==========================>" << endl;
    cout << "Time taken (map): " << int(duration.count()/1000) << " milliseconds" << endl;
	cout << "<==========================>" << endl;
	

}






























