#include <iostream>
#include <vector>
#include <random>

using namespace std;

/* Class created to store in an object information of the 
score after each pass. This will improve the efficiency of
the program*/
class pathScores {

private:
	int number;
	int turn;
	int score;
	static int totalScores;
public:
	pathScores(int number, int turn, int score) {
		this->number = number;
		this->turn = turn;
		this->score = score;
		totalScores++;
	}
	int getNumber() {
		return number;
	}
	int getTurn() {
		return turn;
	}
	int getScore() {
		return score;
	}

};

int pathScores::totalScores = 0;

// Function prototypes
int traverse (int n, int turn);
int max (int scoreX, int scoreY, int scoreZ);
void determineWinner (int turn);
int playerTurn (int turn, int number);
int computerTurn (int turn, int number);
void switchTurn(int& turn);
void playerVsComputer();
void computerVsComputer();
int randomNumber();

// Global variable to store the scores from each path
vector<pathScores> scoreVector;

int main() {

	system("clear");

	// Version 1 of the game Player vs Computer
	playerVsComputer();

	/* Version 2 of the game Computer vs Computer
	 * NOTE: Uncomment the code below to test this version of the game!
	 */
	// computerVsComputer();

	return 0;

}

/* Function to generate a random number ranging from [1, 99]
 * NOTE: Again the mt19937 was used to generate a more distributed random numbers
 */
int randomNumber() {

	int number;
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_int_distribution<int> distribution(1, 99);
	number = distribution(generator);
	return number;

}

// Function for version 1 of the game (Player vs Computer)
void playerVsComputer() {

	int number;
	int turn;

	cout << "The starting number is: ";
	number = randomNumber();
	cout << number << endl << endl;

	while (number != 0) {

		turn = 1;
		int playerOption = playerTurn(turn, number);
		cout << "Player 1 option is " << playerOption << endl;
		number -= playerOption;
		cout << "Remaining after player 1 turn is " << number << endl << endl;

		if (number != 0) {
				
			turn = 2;
			int computerOption = computerTurn(turn, number);
			number -= computerOption;

			cout << "Remaining after player " << turn << " turn is " << number << endl << endl;

		}

	}

	determineWinner(turn);

}

// Function for version 2 of the game (Computer vs Computer)
void computerVsComputer() {

	int number;
	int turn;

	cout << "Insert a starting number: ";
	cin >> number;
	cout << endl;

	while (number != 0) {

		turn = 1;
		int computer_1_Option = computerTurn(turn, number);
		number -= computer_1_Option;
		cout << "Remaining after player " << turn << " turn is " << number << endl << endl;

		if (number != 0) {
				
			turn = 2;
			int computerOption = computerTurn(turn, number);
			number -= computerOption;

			cout << "Remaining after player " << turn << " turn is " << number << endl << endl;

		}

	}

	determineWinner(turn);

}

/* Function that allows the player to input the option desired
 * It validates if the user input is 4, 3, or 1. It also makes sure that
 * player cannot input anything other than those 3 numbers. Furthermore,
 * it doesn't let the player to subtract more of the remaining quantity
 */
int playerTurn (int turn, int number) {

	int playerValue = 0;
	turn = 1;

	cout << "Insert number to subtract (4, 3, or 1): ";

	cin >> playerValue;

	cout << endl;

	while ((playerValue > number) || ((playerValue != 4) && (playerValue != 3) && (playerValue != 1))) {

		if ((playerValue != 4) && (playerValue != 3) && (playerValue != 1)) {

			cout << "Please try again! Insert number to subtract (4, 3, or 1): ";
			cin.clear();
			cin.ignore();
			cin >> playerValue;
			cout << endl;

		}

		else if (playerValue > number) {

			cout << "Invalid subtraction. Select a smaller number (4, 3, or 1): ";
			cin.clear();
			cin.ignore();
			cin >> playerValue;
			cout << endl;

		}

	}

	return playerValue;

}

/* Function that works like the AI of the game. It checks if the remaining of the number
 * is 4, 3, or 1. If it falls in any of those cases, the computer automatically wins the game.
 * Otherwise, it will calculate recursively the best option to take in order to win the game.
 */
int computerTurn (int turn, int number) {

	int pathX;
	int pathY;
	int pathZ;
	int bestOption;

	if (number - 4 == 0) {
		cout << "Player " << turn << " inserted: " << 4 << endl;
		return 4;
	}
	else if (number - 3 == 0) {
		cout << "Player " << turn << " inserted: " << 3 << endl;
		return 3;
	}
	else if (number - 1 == 0) {
		cout << "Player " << turn << " inserted: " << 1 << endl;
		return 1;
	}
	
	pathZ = traverse(number - 1, turn);	
	pathY = traverse(number - 3, turn);
	pathX = traverse(number - 4, turn);

	bestOption = max(pathX, pathY, pathZ);

	cout << "Player " << turn << " inserted: " << bestOption << endl;

	return bestOption;

}

/* Recursive funtion to calculate each possible path. This function was developed
 * by using dynamic programing to save intermediate values and avoid repetition.
 * Each intermediate value is saved in a vector that stores objects name pathScores.
 * This object contains 3 fields, which are number, turn (to distinguish the score for the players),
 * and the score of the path. By saving intermediate values, the program efficiency increases and
 * avoid running out of memory.
 */
int traverse (int n, int turn) {

	int compScore = 0;
	int pathX = 0; 
	int pathY = 0; 
	int pathZ = 0;

	if (n == 0 && turn == 2) {
		compScore++;
		return compScore;
	}
	else  if (n == 0 && turn == 1){
		compScore--;
		return compScore;
	}

	if (scoreVector.size() != 0) {
		for (int i = 0; i < scoreVector.size(); i++) {
			if (scoreVector.at(i).getNumber() == n && scoreVector.at(i).getTurn() == turn) {
				return scoreVector.at(i).getScore();
			}
		}
	}

	switchTurn(turn);

	if ((n - 4) >= 0) {
		
		pathX += traverse (n - 4, turn);
		scoreVector.push_back(pathScores(n - 4, turn, pathX));
		compScore = 0;
		
	}
	if ((n - 3) >= 0) {
		
		pathY += traverse (n - 3, turn);
		scoreVector.push_back(pathScores(n - 3, turn, pathY));
		compScore = 0;
	}
	if ((n - 1) >= 0) {
		
		pathZ += traverse (n - 1, turn);
		scoreVector.push_back(pathScores(n - 1, turn, pathZ));
		compScore = 0;
	}

	return pathX + pathY + pathZ;

}

// Function that changes the turn when the current player finishes his move
void switchTurn(int& turn) {

	if (turn == 2) {
		turn = 1;
	}
	else {
		turn = 2;
	}

}

// Function that determines who wins the game
void determineWinner (int turn) {

	if (turn == 1) {
		cout << "---------------------------------------------" << endl;
		cout << "Player " << turn << " wins the game" << endl;
		cout << "---------------------------------------------" << endl << endl;
	}

	else {
		cout << "---------------------------------------------" << endl;
		cout << "Player " << turn << " wins the game" << endl;
		cout << "---------------------------------------------" << endl << endl;
	}

}

/* Function that returns the option that the computer will use to subtract
 * the number based on the score of each path. This will take the path that
 * has the highest score, which represents the highest chance for the computer
 * to win
 */ 
int max (int scoreX, int scoreY, int scoreZ) {

	if ((scoreX >= scoreY) && (scoreX >= scoreZ) && (scoreX != 0)) {
		return 4;
	}
	else if ((scoreY >= scoreX) && (scoreY >= scoreZ) && (scoreY != 0)) {
		return 3;
	}
	else {
		return 1;
	}

}