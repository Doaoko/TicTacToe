// TicTacToe - Human vs. computer

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Set global constants
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

// Function prototypes
void instructions();
char askYesNo(string question);
int askNumber(string question, int high, int low = 0);
char humanPiece();
char opponent(char piece);
void displayBoard(const vector<char> &board);
char winner(const vector<char> &board);
bool isLegal(int move, const vector<char> &board);
int humanMove(const vector<char> &board, char human);
int computerMove(vector<char> board, char computer);
void announceWinner(char winner, char computer, char human);

// Main function
int main()
{
replay_label:

	int move;
	const int NUM_SQUARES = 9;
	vector<char> board(NUM_SQUARES, EMPTY);

	// Instructions and set up
	instructions();
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;
	displayBoard(board);

	// Game loop
	while (winner(board) == NO_ONE)
	{
		if (turn == human)
		{
			cout << "\t0 | 1 | 2 \n";
			cout << "\t---------\n";
			cout << "\t3 | 4 | 5 \n";
			cout << "\t---------\n";
			cout << "\t6 | 7 | 8 \n\n";
			move = humanMove(board, human);
			board[move] = human;
		}
		else
		{
			move = computerMove(board, computer);
			board[move] = computer;
			displayBoard(board);
		}
		turn = opponent(turn);
	}

	announceWinner(winner(board), computer, human);

	// Asks the user if they wish to replay
	cout << "Thanks for playing!\n\n";
	char replay = askYesNo("Would you like to play again?");
	if (replay == 'y')
	{
		cout << "Then we shall begin again!\n\n";
		goto replay_label;
	}
	else
	{
		return 0;
	}
}

// Other functions

void instructions()
{
	cout << "Welcome to TicTacToe.  Man vs. machine!  This version has been uploaded to GitHub!" << endl;
	cout << "To make a move, enter a number between 0 and 8, as displayed on the board below." << endl << endl;
	cout << "\t0 | 1 | 2 \n";
	cout << "\t---------\n";
	cout << "\t3 | 4 | 5 \n";
	cout << "\t---------\n";
	cout << "\t6 | 7 | 8 \n\n";
	cout << "Good luck!\n\n";
}

char askYesNo(string question)
{
	char response = 'a';
	while (response != 'y' && response != 'n')
	{
		cout << question << " (y/n): ";
		cin >> response;
		cout << endl;
	}
	return response;
}

int askNumber(string question, int high, int low)
{
	int number = -1;
	while (number > high || number < low)
	{
		cout << question << " (" << low << " - " << high << "): ";
		cin >> number;
	}
	return number;
}

char humanPiece()
{
	char go_first = askYesNo("Do you want to go first?");
	if (go_first == 'y')
	{
		cout << "You have selected to go first!" << endl;
		return X;
	}
	else
	{
		cout << "You have selected to go second!" << endl;
		return O;
	}
}

char opponent(char piece)
{
	if (piece == X)
	{
		return O;
	}
	else
	{
		return X;
	}
}

void displayBoard(const vector<char> &board)
{
	cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
	cout << endl << endl;
}

char winner(const vector<char> &board)
{
	// All possible winning combos
	const int WINNING_ROWS[8][3] =
	{{0,1,2},
	{3,4,5},
	{6,7,8},
	{0,4,8},
	{2,4,6},
	{0,3,6},
	{1,4,7},
	{2,5,8}};

	const int TOTAL_ROWS = 8;

	// Check all possible winning combinations to see whether they all contain X or O
	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]))
		{
			return board[WINNING_ROWS[row][0]];
		}
	}

	// If nobody has won yet
	if (count(board.begin(), board.end(), EMPTY) == 0)
	{
		return TIE;
	}

	// If it's not a tie either
	return NO_ONE;
}

inline bool isLegal(int move, const vector<char> &board)
{
	return (board[move] == EMPTY);
}

int humanMove(const vector<char> &board, char human)
{
	int move = askNumber("Where would you like to place your next piece?", board.size() - 1);
	while (!isLegal(move, board))
	{
		cout << endl << "That square is already occupied, please try another.";
		move = askNumber("Where will you move?", (board.size() - 1));
	}
	cout << "Player has selected square " << move << endl;
	return move;
}

int computerMove(vector<char> board, char computer)
{
	// Note that the board isn't a constant reference, a copy is made which can be changed
	unsigned int move = 0;
	bool found = false;

	// Take winning move if available
	while (!found && move < board.size())
	{
		if (isLegal(move, board))
		{
			board[move] = computer;
			found = winner(board) == computer;
			board[move] = EMPTY;
		}
		if (!found)
		{
			++move;
		}
	}
	
	// If can't win, block a winning move if available for opponent
	if (!found)
	{
		move = 0;
		char human = opponent(computer);

		while (!found && move < board.size())
		{
			if (isLegal(move, board))
			{
				board[move] = human;
				found = winner(board) == human;
				board[move] = EMPTY;
			}
			if (!found)
			{
				++move;
			}
		}
	}

	// If nobody can win next move, take the middle square, or a corner, or whatever
	if (!found)
	{
		move = 0;
		unsigned int i = 0;

		const int BEST_MOVES[] = {4,0,2,6,8,1,3,5,7};
		while (!found && i < board.size())
		{
			move = BEST_MOVES[i];
			if (isLegal(move, board))
			{
				found = true;
			}
			++i;
		}
	}
	cout << "Computer has selected square " << move << endl;
	return move;
}

void announceWinner(char winner, char computer, char human)
{
	if (winner == computer)
	{
		cout << winner << "'s have won!  The computer is victorious.\n\n";
	}
	else if (winner == human)
	{
		cout << winner << "'s have won!  You are victorious.\n\n";
	}
	else
	{
		cout << "It's a tie?  Better luck next time!\n\n";
	}
}
