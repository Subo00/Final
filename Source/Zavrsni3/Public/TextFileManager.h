// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>


#include "TextFileManager.generated.h"


using namespace std;
const int maxVal = 20;
/**
 *
 */

class Coord {
public:
	int x, y;
	void setCoord(int X, int Y) {
		x = X;
		y = Y;
	}
	bool compareCoord(Coord one) {
		if (one.x == x) {
			if (one.y == y) {
				return true;
			}
		}
		return false;
	}
};

class FinalState : public Coord {
public:
	int finalScore;
	void setScore(int score) {
		finalScore = score;
	}
};

//STATE
class State : public Coord {
public:
	int fuel;
	void setFuel(int f) {
		fuel = f;
	}
	void moveState(int action) {
		switch (action) { //it moves to the location depending on the action
		case 0: //LEFT
			y -= 1;
			break;
		case 1: //UP
			x -= 1;
			break;
		case 2: //DOWN
			x += 1;
			break;
		case 3: //RIGHT
			y += 1;
			break;
		}
		fuel--;
	}
};

//BARREL
class Barrel : public Coord {
public:
	bool pickedUp;
	int fuelPoints; //number of fuel points
	void setFuelP(int points) {
		fuelPoints = points;
	}
	void setPicked() {
		pickedUp = true;
	}
	void setPlaced() {
		pickedUp = false;
	}

};



typedef struct {//every tile has a neighbour
	int move[4];//[Left,Up,Down,Right]
}tile;

class Table {
public:
	tile tileMap[maxVal][maxVal]; //2D array of tiles
	int rowSize, colSize;
	void setTable(int maxX, int maxY, int fill) {
		rowSize = maxX;
		colSize = maxY;
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				for (int k = 0; k < 4; k++) {
					tileMap[i][j].move[k] = fill; //sets everything to 0
				}
			}
		}

	}
	void print(string s = " ") {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				cout << s << " [" << i << "] [" << j << "] = ";
				for (int k = 0; k < 4; k++) {
					cout << tileMap[i][j].move[k] << " ";
				}
				cout << "\n";
			}
			cout << "\n";
		}
	}

	virtual void initialize(FinalState finalState) {

	}

	int compareTable(Table one) {
		int absValue = 0;
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				for (int k = 0; k < 4; k++) {
					absValue += abs(tileMap[i][j].move[k] -
						one.tileMap[i][j].move[k]);
				}
			}
		}
		return absValue;
	}


};

class TableReward : public Table {
public:
	void initialize(FinalState finalState) override {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				if (j == 0) {//if it's the left most tile
					tileMap[i][j].move[0] = 0;
				}
				else if (j == finalState.y + 1 && i == finalState.x) {//if the goal is left of tile
					tileMap[i][j].move[0] = finalState.finalScore;
				}

				if (i == 0) { //if it's top most tile
					tileMap[i][j].move[1] = 0;
				}
				else if (i == finalState.x + 1 && j == finalState.y) {//if the goal is above the tile
					tileMap[i][j].move[1] = finalState.finalScore;
				}

				if (i == rowSize - 1) {//if it's the bottom most tile
					tileMap[i][j].move[2] = 0;
				}
				else if (i == finalState.x - 1 && j == finalState.y) {//if the goal is bellow the tile
					tileMap[i][j].move[2] = finalState.finalScore;
				}

				if (j == colSize - 1) {//if it's the right most tile
					tileMap[i][j].move[3] = 0;
				}
				else if (j == finalState.y - 1 && i == finalState.x) {//if the goal is right of tile
					tileMap[i][j].move[3] = finalState.finalScore;
				}
			}
		}
	}


};

class TableFuel : public Table {
public:
	void initializeB(Barrel finalState) {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				if (j == 0) {//if it's the left most tile
					tileMap[i][j].move[0] = 0;		//if the barrel was picked, don't put values on that place
				}
				else if (j == finalState.y + 1 && i == finalState.x) {//if the goal is left of tile
					if (finalState.pickedUp == false) {
						tileMap[i][j].move[0] = finalState.fuelPoints * 100;
					}
					else {
						tileMap[i][j].move[0] = -1;
					}
				}

				if (i == 0) { //if it's top most tile
					tileMap[i][j].move[1] = 0;
				}
				else if (i == finalState.x + 1 && j == finalState.y) {//if the goal is above the tile
					if (finalState.pickedUp == false) {
						tileMap[i][j].move[1] = finalState.fuelPoints * 100;
					}
					else {
						tileMap[i][j].move[1] = -1;
					}
				}

				if (i == rowSize - 1) {//if it's the bottom most tile
					tileMap[i][j].move[2] = 0;
				}
				else if (i == finalState.x - 1 && j == finalState.y) {//if the goal is bellow the tile
					if (finalState.pickedUp == false) {
						tileMap[i][j].move[2] = finalState.fuelPoints * 100;
					}
					else {
						tileMap[i][j].move[2] = -1;
					}
				}

				if (j == colSize - 1) {//if it's the right most tile
					tileMap[i][j].move[3] = 0;
				}
				else if (j == finalState.y - 1 && i == finalState.x) {//if the goal is right of tile
					if (finalState.pickedUp == false) {
						tileMap[i][j].move[3] = finalState.fuelPoints * 100;
					}
					else {
						tileMap[i][j].move[3] = -1;
					}
				}
			}
		}
	}


	void setWalls(FinalState finalState) {
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				if (j == 0) {//if it's the left most tile
					tileMap[i][j].move[0] = 0;
				}
				else if (j == finalState.y + 1 && i == finalState.x) {//if the goal is left of tile
					tileMap[i][j].move[0] = 0;
				}

				if (i == 0) { //if it's top most tile
					tileMap[i][j].move[1] = 0;
				}
				else if (i == finalState.x + 1 && j == finalState.y) {//if the goal is above the tile
					tileMap[i][j].move[1] = 0;
				}

				if (i == rowSize - 1) {//if it's the bottom most tile
					tileMap[i][j].move[2] = 0;
				}
				else if (i == finalState.x - 1 && j == finalState.y) {//if the goal is bellow the tile
					tileMap[i][j].move[2] = 0;
				}

				if (j == colSize - 1) {//if it's the right most tile
					tileMap[i][j].move[3] = 0;
				}
				else if (j == finalState.y - 1 && i == finalState.x) {//if the goal is right of tile
					tileMap[i][j].move[3] = 0;
				}
			}
		}

	}
};



UCLASS()
class ZAVRSNI3_API UTextFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


		UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keyword = "Save"))
		static bool SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> InputText);


		UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keyword = "Movement"))
		static TArray<FString> GetActions(TArray<int> InputText);


		UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keyword = "Movement"))
		FString GetAction(TArray<int> InputText);

};

class Qlearn
{
public:
	//Tables
	TableReward R;
	Table Q, preQ;
	TableFuel F;
	Table Qfuel;
	//States
	State currentState; //state to use while moving
	State futureState; //future state to move to
	State initialState; //state where the AI always starts
	State fuelState; //used for Qfuel
	//Final states
	vector <FinalState> finalState; //state where AI needs to finish

	//Items
	vector <Barrel> barrel; //barrel adds fuel

	const double gamma = 0.7; //dicount factor
	const double alpha = 0.6; //learning factor
	const int iterations = 100000; //number of nonchanged iterations


	void start(TArray<int> InputText) {
		int rowSize, colSize;
		int32 Index = 0;
		rowSize = InputText[Index];
		Index++;
		colSize = InputText[Index];
		Index++;
		//rowSize, colSize, fill with
		R.setTable(rowSize, colSize, -1);
		Q.setTable(rowSize, colSize, 0);
		preQ.setTable(rowSize, colSize, 0);
		F.setTable(rowSize, colSize, -1);
		Qfuel.setTable(rowSize, colSize, 0);

		int it, x, y, z;


		//inputing coords and fuel for player 
		x = InputText[Index];
		Index++;
		y = InputText[Index];
		Index++;
		z = InputText[Index];
		Index++;
		initialState.setCoord(x, y);
		initialState.setFuel(z);


		//number of final states
		FinalState tmpFinal;
		it = InputText[Index];
		Index++;
		while (it > 0) {
			x = InputText[Index];
			Index++;
			y = InputText[Index];
			Index++;
			z = InputText[Index];
			Index++;
			tmpFinal.setCoord(x, y);
			tmpFinal.setScore(z);

			//initilazes the tiles that are next to finaleState, or in this case tile
			F.setWalls(tmpFinal);
			R.initialize(tmpFinal);
			finalState.push_back(tmpFinal);
			it--;

		}

		it = InputText[Index];
		Index++;
		//this while loop sets walls in R and F table
		while (it > 0) {
			x = InputText[Index];
			Index++;
			y = InputText[Index];
			Index++;
			
			tmpFinal.setCoord(x, y);
			tmpFinal.setScore(0);

			//initilazes the tiles that are next to walls
			F.setWalls(tmpFinal);
			R.initialize(tmpFinal);
			it--;
		}


		//set barrels
		Barrel tmpBarrel;
		it = InputText[Index];
		Index++;
		while (it > 0) {
			x = InputText[Index];
			Index++;
			y = InputText[Index];
			Index++;
			z = InputText[Index];
			Index++;
			tmpBarrel.setCoord(x, y);
			tmpBarrel.setFuelP(z);
			F.initializeB(tmpBarrel);
			barrel.push_back(tmpBarrel);
			it--;
		}

		srand((unsigned)time(0)); //for rand in chooseAnAction()

	}



	void resetQfuel() {
		Qfuel.setTable(Qfuel.rowSize, Qfuel.colSize, 0);
		for (unsigned int it = 0; it < barrel.size(); it++) {
			F.initializeB(barrel.at(it));
		}
		for (int i = 0; i < iterations; i++) {
			episodeFuel();
		}
		//Qfuel.print("QF ");
		//cout <<"X: "<< fuelState.x <<" Y: "<< fuelState.y <<" Fuel: " <<fuelState.fuel <<"\n ";
	}

	void episodeFuel() {
		fuelState = currentState;

		while (!isStateFinalFuel())
			chooseAnActionFuel();
	}

	bool isStateFinalFuel() {
		for (unsigned int it = 0; it < barrel.size(); it++) {
			if (fuelState.compareCoord(barrel.at(it)) && barrel.at(it).pickedUp == false) {
				return true;
			}
		}
		return false;
	}

	void chooseAnActionFuel() {
		int possAction;
		possAction = rand() % 4; //random action (0-left,1-up,2-down,3-right) 
		if (F.tileMap[fuelState.x][fuelState.y].move[possAction] != 0) { //if it can move to that location
			//preQ = Q; //saving previous Q table, so that we can compare it later

			Qfuel.tileMap[fuelState.x][fuelState.y].move[possAction] =
				//(1-alpha) * Q(currentState,action)
				(1 - alpha) * Qfuel.tileMap[fuelState.x][fuelState.y].move[possAction] +
				//alpha * sample
				alpha * (float)rewardFuel(possAction);
			fuelState.moveState(possAction);

			//checkBarrel();
		}
	}

	int rewardFuel(int action) {//sample
							//Reward for being in the futureState given the action ->R(curState,action,futureState) + 
		return static_cast<int>(F.tileMap[fuelState.x][fuelState.y].move[action] +
			//gamma * maxValue in Q table of futureState ->Q(futureState,action)
			gamma * maximumFuel(action));
	}

	int maximumFuel(int action) {
		int winner;
		bool foundNewWinner;
		bool done = false;
		winner = 0;

		futureState = fuelState;
		futureState.moveState(action);
		do {
			foundNewWinner = false; //it's set to false so that it can exit the while loop
			for (int i = 0; i < 4; i++) {
				if ((i < winner) || (i > winner)) { //Avoid self-comparison.
					if (Qfuel.tileMap[futureState.x][futureState.y].move[i] >
						Qfuel.tileMap[futureState.x][futureState.y].move[winner]) {
						winner = i;
						foundNewWinner = true;
					}
				}
			}

			if (foundNewWinner == false) {//it will loop twice when a new winner is found
				done = true;
			}

		} while (done == false);

		return Qfuel.tileMap[futureState.x][futureState.y].move[winner];
	}


	void setBarrels() {
		for (unsigned int it = 0; it < barrel.size(); it++) {
			barrel.at(it).setPlaced();
		}
	}



	void chooseAnAction() {
		int possAction;
		possAction = rand() % 4; //random action (0-left,1-up,2-down,3-right) 
		if (R.tileMap[currentState.x][currentState.y].move[possAction] != 0) { //if it can move to that location
			preQ = Q; //saving previous Q table, so that we can compare it later

			Q.tileMap[currentState.x][currentState.y].move[possAction] =
				//(1-alpha) * Q(currentState,action)
				(1 - alpha) * preQ.tileMap[currentState.x][currentState.y].move[possAction] + alpha * (float)reward(possAction);
			currentState.moveState(possAction);

			checkBarrel();
		}
	}

	void checkBarrel() {
		for (unsigned int it = 0; it < barrel.size(); it++) {
			if (!(barrel.at(it).compareCoord(currentState))) {}
			else if (!(barrel.at(it).pickedUp)) {

				barrel.at(it).setPicked();
				int addFuel = currentState.fuel + barrel.at(it).fuelPoints;
				currentState.setFuel(addFuel);
			}
		}
	}

	bool isStateFinal() {
		for (unsigned int it = 0; it < finalState.size(); it++) {
			if (currentState.compareCoord(finalState.at(it))) {
				return true;
			}
		}
		return false;
	}



	//if the state is in barrel, return fuel value, if not return 0
	int isBarrelInCoord(State tmpState) {
		for (unsigned int it = 0; it < barrel.size(); it++) {
			if (!barrel.at(it).pickedUp && barrel.at(it).compareCoord(tmpState)) {
				return barrel.at(it).fuelPoints;
			}
		}
		return 0;
	}



	int reward(int action) {//sample
		futureState = currentState;
		futureState.moveState(action);

		int bonusPoints = isBarrelInCoord(futureState) * 2;

		//Reward for being in the futureState given the action ->R(curState,action,futureState) + 
		return static_cast<int>(R.tileMap[currentState.x][currentState.y].move[action] +
			//points from barrel
			bonusPoints +
			//gamma * maxValue in Q table of futureState ->Q(futureState,action)
			gamma * maximum(action));
	}


	int maximum(int action) {
		int winner;
		bool foundNewWinner;
		bool done = false;
		winner = 0;

		futureState = currentState;
		futureState.moveState(action);
		do {
			foundNewWinner = false; //it's set to false so that it can exit the while loop
			for (int i = 0; i < 4; i++) {
				if ((i < winner) || (i > winner)) { //Avoid self-comparison.
					if (Q.tileMap[futureState.x][futureState.y].move[i] >
						Q.tileMap[futureState.x][futureState.y].move[winner]) {
						winner = i;
						foundNewWinner = true;
					}
				}
			}

			if (foundNewWinner == false) {//it will loop twice when a new winner is found
				done = true;
			}

		} while (done == false);

		return Q.tileMap[futureState.x][futureState.y].move[winner];
	}

	void episode() {
		currentState = initialState;
		setBarrels();//barrel.setPlaced();
		do {
			chooseAnAction();
			if (isStateFinal()) {
				//cout << "I made it\n";
				break;
			}

		} while (currentState.fuel != 0);
	}


	int taxicab() {
		int maxValue = 0, stateID = 0;

		for (unsigned int it = 0; it < finalState.size(); it++) {
			if (finalState.at(it).finalScore > maxValue) {
				maxValue = finalState.at(it).finalScore;
				stateID = it;
			}
		}
		return abs(currentState.x - finalState.at(stateID).x) +
			abs(currentState.y - finalState.at(stateID).y);
	}

	void updateQ() {
		if (isBarrelInCoord(currentState)) {
			Q.tileMap[currentState.x - 1][currentState.y].move[2] = 0;
			Q.tileMap[currentState.x + 1][currentState.y].move[1] = 0;
			Q.tileMap[currentState.x][currentState.y - 1].move[3] = 0;
			Q.tileMap[currentState.x][currentState.y + 1].move[0] = 0;
			checkBarrel();
		}
	}

	TArray<FString> printMovement(TArray<int> InputText) {

		start(InputText);

		int iterator = 0;
		do {
			episode();
			if (currentState.fuel > 0) {
				if (Q.compareTable(preQ) == 0) {
					iterator++;
				}
				else {
					iterator = 0;
					//cout <<"Compared tables: " << Q.compareTable(preQ)<< "\n";
				}
			}
		} while (iterator != iterations);
		/*
		for (int i = 0; i < 10000; i++) {
			episode();
		}
		*/
		currentState = initialState;
		setBarrels();
		int nextAction = 0;

		TArray<FString> returnString;
		/*for (auto& Str : InputText) {
			returnString.Add(FString::FromInt(Str));
		}*/
		while (currentState.fuel < taxicab()) {
			resetQfuel();
			//cout <<"X: "<< currentState.x <<" Y: "<< currentState.y <<" Fuel: " <<currentState.fuel <<"\n ";
			while (!isBarrelInCoord(currentState)) {
				for (int i = 0; i < 4; i++) {
					if (Qfuel.tileMap[currentState.x][currentState.y].move[i] >
						Qfuel.tileMap[currentState.x][currentState.y].move[nextAction]) {
						nextAction = i;
					}
				}
				switch (nextAction) {
				case 0:
					returnString.Add(TEXT("LEFT"));
					break;
				case 1:
					returnString.Add(TEXT("UP"));
					break;
				case 2:
					returnString.Add(TEXT("DOWN"));
					break;
				case 3:
					returnString.Add(TEXT("RIGHT"));
					break;
				}
				currentState.moveState(nextAction);
			}
			//Qfuel.print("F U");
			updateQ();
		}
		do {
			for (int i = 0; i < 4; i++) {
				if (Q.tileMap[currentState.x][currentState.y].move[i] >
					Q.tileMap[currentState.x][currentState.y].move[nextAction]) {
					nextAction = i;
				}
			}
			switch (nextAction) {
			case 0:
				returnString.Add(TEXT("LEFT"));
				break;
			case 1:
				returnString.Add(TEXT("UP"));
				break;
			case 2:
				returnString.Add(TEXT("DOWN"));
				break;
			case 3:
				returnString.Add(TEXT("RIGHT"));
				break;
			}
			currentState.moveState(nextAction);
			updateQ();
		} while (!isStateFinal());

		return returnString;

	}


	FString printSingleMove(TArray<int> InputText) {

		start(InputText);

		int iterator = 0;
		do {
			episode();
			if (currentState.fuel > 0) {
				if (Q.compareTable(preQ) == 0) {
					iterator++;
				}
				else {
					iterator = 0;
					//cout <<"Compared tables: " << Q.compareTable(preQ)<< "\n";
				}
			}
		} while (iterator != iterations);
		/*
		for (int i = 0; i < 10000; i++) {
			episode();
		}
		*/
		currentState = initialState;
		setBarrels();
		int nextAction = 0;

		
		/*for (auto& Str : InputText) {
			returnString.Add(FString::FromInt(Str));
		}*/
		while (currentState.fuel < taxicab()) {
			resetQfuel();
			//cout <<"X: "<< currentState.x <<" Y: "<< currentState.y <<" Fuel: " <<currentState.fuel <<"\n ";
			while (!isBarrelInCoord(currentState)) {
				for (int i = 0; i < 4; i++) {
					if (Qfuel.tileMap[currentState.x][currentState.y].move[i] >
						Qfuel.tileMap[currentState.x][currentState.y].move[nextAction]) {
						nextAction = i;
					}
				}
				switch (nextAction) {
				case 0:
					return (FString("LEFT"));
				case 1:
					return (FString("UP"));
				case 2:
					return (FString("DOWN"));
				case 3:
					return (FString("RIGHT"));
				}
				currentState.moveState(nextAction);
			}
			//Qfuel.print("F U");
			updateQ();
		}
		do {
			for (int i = 0; i < 4; i++) {
				if (Q.tileMap[currentState.x][currentState.y].move[i] >
					Q.tileMap[currentState.x][currentState.y].move[nextAction]) {
					nextAction = i;
				}
			}
			switch (nextAction) {
			case 0:
				return (FString("LEFT"));
			case 1:
				return (FString("UP"));
			case 2:
				return (FString("DOWN"));
			case 3:
				return (FString("RIGHT"));
			}
			currentState.moveState(nextAction);
			updateQ();
		} while (!isStateFinal());

		return FString("NONE");

	}



};