#include <iostream>
#include <math.h>

#define N 24
using namespace std;

// Function that configures board and state randomly
void configureRandomly(int board[][N], int* state)
{

	srand(time(0));

// Iterating through the column indices
	for (int i = 0; i < N; i++) {

		// Getting a random row index
		state[i] = rand() % N;

		// Placing an archer on the obtained place in the board
		board[state[i]][i] = 1;
	}
}

// Function that prints the board
void printBoard(int board[][N])
{

	for (int i = 0; i < N; i++) {
		cout << " ";
		for (int j = 0; j < N; j++) {
			cout << board[i][j] << " ";
		}
		cout <<endl;
	}
}

// Function that prints the board state
void printState(int* state)
{

	for (int i = 0; i < N; i++) {
		cout << " " << state[i] << " ";
	}
	cout << endl;
}

// Function that compares two arrays, state1 and state2
// and returns true if they are equal and false if they are not
bool compareStates(int* state1, int* state2)
{

	for (int i = 0; i < N; i++) {
		if (state1[i] != state2[i]) {
			return false;
		}
	}
	return true;
}

// Function that fills the board with values
void fill(int board[][N], int value)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			board[i][j] = value;
		}
	}
}

// Function that calculates the objective value of the state
// that means archers attacking each other
int calculateObjective(int board[][N], int* state)
{
// For each archer in a column, we check for other archers in the line
// of our current archer and if found, the variable attacking is incremented

	int attacking = 0;
	int row, col;

	for (int i = 0; i < N; i++) {

// At each column i, the archer is placed at row state[i]

// To the left of same row (row=constant ; col--)
		row = state[i], col = i - 1;
		while (col >= 0 && board[row][col] != 1) {
			col--;
		}
		if (col >= 0 && board[row][col] == 1) {
			attacking++;
		}

// To the right of same row (row=constant ; col++)
		row = state[i], col = i + 1;
		while (col < N && board[row][col] != 1) {
			col++;
		}
		if (col < N && board[row][col] == 1) {
			attacking++;
		}

// Diagonally to the left up (row-- ; col--)
		row = state[i] - 1, col = i - 1;
		while (col >= 0 && row >= 0 && board[row][col] != 1) {
			col--;
			row--;
		}
		if (col >= 0 && row >= 0 && board[row][col] == 1) {
			attacking++;
		}

// Diagonally to the right down (row++ ; col++)
		row = state[i] + 1, col = i + 1;
		while (col < N && row < N && board[row][col] != 1) {
			col++;
			row++;
		}
		if (col < N && row < N && board[row][col] == 1) {
			attacking++;
		}

// Diagonally to the left down (col-- ; row++)
		row = state[i] + 1, col = i - 1;
		while (col >= 0 && row < N && board[row][col] != 1) {
			col--;
			row++;
		}
		if (col >= 0 && row < N && board[row][col] == 1) {
			attacking++;
		}

// Diagonally to the right up (col++ ; row--)
		row = state[i] - 1, col = i + 1;
		while (col < N && row >= 0 && board[row][col] != 1) {
			col++;
			row--;
		}
		if (col < N && row >= 0 && board[row][col] == 1) {
			attacking++;
		}
	}

	return (int)(attacking / 2);
}

// Function that generates a board configuration with a state
void generateBoard(int board[][N], int* state)
{
	fill(board, 0);
	for (int i = 0; i < N; i++) {
		board[state[i]][i] = 1;
	}
}

// Function that copies state2 contents to state1
void copyState(int* state1, int* state2)
{
	for (int i = 0; i < N; i++) {
		state1[i] = state2[i];
	}
}

// This function gets the neighbour of the current state
void getNeighbour(int board[][N], int* state)
{
// Declaring and initializing the optimal board and state
// with the current board and state
	int opBoard[N][N];
	int opState[N];

	copyState(opState, state);
	generateBoard(opBoard, opState);

// Initializing the optimal objective value
	int opObjective = calculateObjective(opBoard, opState);


// Declaring and initializing the temporary board and state
	int NeighbourBoard[N][N];
	int NeighbourState[N];

	copyState(NeighbourState, state);
	generateBoard(NeighbourBoard, NeighbourState);

// Iterating through all possilbe neighbours
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			if (j != state[i]) {

			// Initializing temporary neighbour with current neighbour

				NeighbourState[i] = j;
				NeighbourBoard[NeighbourState[i]][i] = 1;
				NeighbourBoard[state[i]][i] = 0;

			// Calculating the objective value of neighbour
				int temp = calculateObjective(NeighbourBoard, NeighbourState);

			// Comparing temporary and optimal neighbour objectives and update accordingly
				if (temp <= opObjective) {
					opObjective = temp;
					copyState(opState,NeighbourState);
					generateBoard(opBoard,opState);
				}

			// Reseting to the original configuration for next iteration
				NeighbourBoard[NeighbourState[i]][i] = 0;
				NeighbourState[i] = state[i];
				NeighbourBoard[state[i]][i] = 1;
			}
		}
	}

// Copying the optimal board and state to the current board and state
	copyState(state, opState);
	fill(board, 0);
	generateBoard(board, state);
}

void hillClimbing(int board[][N], int* state)
{
// Declaring and initializing the neighbour with current board and state
	int neighbourBoard[N][N] = {};
	int neighbourState[N];

	copyState(neighbourState, state);
	generateBoard(neighbourBoard, neighbourState);

	do {

	// Copying the neighbour board and state to the current board and state
		copyState(state, neighbourState);
		generateBoard(board, state);

	// Getting the optimal neighbour
		getNeighbour(neighbourBoard, neighbourState);

		if (compareStates(state, neighbourState)){

		// If neighbour and current are equal, no optimal neighbour exists
		// and if that is the case print the board and break
			printBoard(board);
			break;
		}
		else if (calculateObjective(board, state) == calculateObjective(neighbourBoard, neighbourState)){
		// If neighbour and current are not equal but their objectives are
		// then we are going to encounter a shoulder or a local optimum
		// and we are going to jump to a random neighbour

        // Random neighbour
			neighbourState[rand() % N] = rand() % N;

			generateBoard(neighbourBoard, neighbourState);
		}

	} while (true);
}

int main()
{

	int state[N] = {};
	int board[N][N] = {};

	// Starting point by randomly configuring the board
	configureRandomly(board, state);

	// Do hill climbing on the board
	hillClimbing(board, state);

	return 0;
}