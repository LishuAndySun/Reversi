#include<iostream>
#include<limits>
#include<thread>
#include<chrono>

using namespace std;

struct pos{
    int row;
    int col;
};

const int BOARD_SIZE  = 8;
const int PIECE_BLACK = 1;
const int PIECE_WHITE = -1;
const int PIECE_EMPTY = 0;  

int ComputerPlayer;
int HumanPlayer;

int board[BOARD_SIZE][BOARD_SIZE];

struct Position{
    int row;
    int col;
};

const int DIR_ROW[8] = {0, 1, 1, 1, 0, -1, -1, -1};
const int DIR_COL[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

const int POSITION_WEIGHTS[BOARD_SIZE][BOARD_SIZE] =
{
    {50, 5, 30, 30, 30, 30,  5, 50},
    {30, 10,20, 20, 20, 20, 10, 30},
    {30, 10,20, 20, 20, 20, 10, 30},
    {30, 10,20, 20, 20, 20, 10, 30},
    {30, 10,20, 20, 20, 20, 10, 30},
    {30, 10,20, 20, 20, 20, 10, 30},
    {30, 10,20, 20, 20, 20, 10, 30},
    {50, 5, 30, 30, 30, 30,  5, 50}
};

void initBoard();
void chooseSides(); //finish
bool isGameOVer();
void displayBoard();
char getPieceSymbol(int PieceColour); //finish
bool hasValidMoves(int playerColour);
bool isGameOver();
bool isValidMove(int playerColor, int row, int col);
void placePieceAndFlip(int playerColour, int row, int col);
bool canFlipInDirection(int playerColor, int row, int col, int dir);
void computerMakeMove();
int DynamicalGetPositionScore(int row, int col);
void flipPieceInDirection(int playerColour, int row, int col, int directionIndex);
void announceWinner();

int main()
{
    int inputRow, inputCol;
    int currentPlayer;
    initBoard();
    chooseSides();
    currentPlayer = PIECE_BLACK;
    while(isGameOver() == false)
    {
        displayBoard();
        if(currentPlayer == HumanPlayer)
        {
            if(hasValidMoves(HumanPlayer) == true)
            {
                cout <<"You are '" << getPieceSymbol(HumanPlayer) << "'. It's your turn." << endl;
                while(true)
                {
                    if(!(cin >> inputRow >> inputCol))
                    {
                        if(cin.eof())
                        {
                            return 0;
                        }

                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input! Please input row and col:" << endl;
                        continue;
                    }

                    if(isValidMove(HumanPlayer, inputRow, inputCol))
                    {
                        break;
                    }

                    cout << "Invalid move! Please check your input and input again(row, col):" << endl;
                }
                placePieceAndFlip(HumanPlayer, inputRow, inputCol);
            }
            else
            {
                cout << "Human player has no valid moves, computer will move in 2 seconds......";
                this_thread::sleep_for(chrono::seconds(2));
            }
        }
        else
        {
            // Computer's turn
            if(hasValidMoves(ComputerPlayer))
            {
                computerMakeMove();
            }
            else
            {
                cout << "Computer has no valid moves, human player will move in 2 seconds." << endl;
                this_thread::sleep_for(chrono::seconds(2));
            }
        }
        //Switch to the another player.
        currentPlayer = (currentPlayer == PIECE_BLACK) ? PIECE_WHITE : PIECE_BLACK;
    }
    displayBoard();
    cout << "=== Game Over" << endl;
    announceWinner();
    return 0;
}

void chooseSides()
{
    int choice;
    choice = 0;
    cout << "=====================" << endl;
    cout << "Choose your side:" << endl;
    cout << "1. Black ($ moves first)" << endl;
    cout << "2. White (* moves first)" << endl; 
    while(true)
    {
        cout << "Your choice: ";
        if(!(cin >> choice))
        {
            if(cin.eof())
            {
                exit(0);
            }

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please input 1 or 2." << endl;
            continue;
        }

        if(choice == 1 || choice == 2)
        {
            break;
        }

        cout << "Invalid input! Please input 1 or 2." << endl;
    }
    if(choice == 1)
    {
        HumanPlayer = PIECE_BLACK;
        ComputerPlayer = PIECE_WHITE;
    }
    else
    {
        HumanPlayer = PIECE_WHITE;
        ComputerPlayer = PIECE_BLACK;
    }
    cout << "You are '" << getPieceSymbol(HumanPlayer) << "', the computer is '" << getPieceSymbol(ComputerPlayer) << "'." << endl;
}

char getPieceSymbol(int PieceColour)
{
    return (PieceColour == PIECE_BLACK) ? '$' : '*';
}  

void initBoard()
{
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0 ; j < BOARD_SIZE; j++)
        {
            board[i][j] = PIECE_EMPTY;
        }
    }

    board[3][3] = board[4][4] = PIECE_BLACK;
    board[3][4] = board[4][3] = PIECE_WHITE;
}

bool isGameOver()
{
    if(!hasValidMoves(HumanPlayer) && !hasValidMoves(ComputerPlayer))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void displayBoard()
{
    cout << endl;
    cout << "  ";
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        cout << i << " ";
    }
    cout << endl;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        cout << i << " ";
        for(int j = 0 ; j < BOARD_SIZE; j++)
        {
            if(board[i][j] == PIECE_BLACK)
            {
                cout << "$ ";
            }
            else if (board[i][j] == PIECE_WHITE)
            {
                cout << "* ";
            }
            else
            {
                cout << ". ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool hasValidMoves(int playerColour)
{
    bool flag = false;
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(isValidMove(playerColour, i, j))
            {
                flag = true;
                break;
            }
        }
        if (flag == true)
        {
            break;
        }
    }
    return flag;
}

// Justify if the movement (computer or player) is valid.
bool isValidMove(int playerColor, int row, int col)
{
    if(row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        return false;
    }

    if (board[row][col] != PIECE_EMPTY)
    {
        return false;
    }

    for(int i = 0 ; i < 8; i++)
    {
        if(canFlipInDirection(playerColor, row, col, i))
        {
            return true;
        }
    }
    return false;
}

// Check if pieces can be flipped in the given direction.
bool canFlipInDirection(int playerColor, int row, int col, int directionIndex)
{
    int NextRow = row + DIR_ROW[directionIndex];
    int NextCol = col + DIR_COL[directionIndex];
    bool hasOpponentPieceBetween = false;

    while(NextRow >= 0 && NextRow < BOARD_SIZE && NextCol >= 0 && NextCol < BOARD_SIZE && board[NextRow][NextCol] != PIECE_EMPTY)
    {
        if(board[NextRow][NextCol] == playerColor)
        {
            // Opponent pieces are sandwiched and the endpoint is our own piece.s
            return hasOpponentPieceBetween;
        }
        // Opponent Piece founded, keep moving along this direction.
        hasOpponentPieceBetween = true;
        NextRow += DIR_ROW[directionIndex];
        NextCol += DIR_COL[directionIndex];
    }
    return false;
}

//Computer evaluate & move
void computerMakeMove()
{
    Position BestPos = {-1, -1};
    int MaxScore = 0;
    for(int i = 0 ; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            if(isValidMove(ComputerPlayer, i, j))
            {
                int CurrentScore = DynamicalGetPositionScore(i, j);
                if(CurrentScore > MaxScore)
                {
                    MaxScore = CurrentScore;
                    BestPos.row = i;
                    BestPos.col = j;
                }
            }
        }
    }
    if(MaxScore > 0 && BestPos.row != -1)
    {
        placePieceAndFlip(ComputerPlayer, BestPos.row, BestPos.col);
    }
}

int DynamicalGetPositionScore(int row, int col)
{
    int emptyCellCount = 0;
    int occupiedCellCount = 0;
    int flippedPieceCount = 0;

    for(int boardRow = 0; boardRow < BOARD_SIZE; boardRow++)
    {
        for(int boardCol = 0; boardCol < BOARD_SIZE; boardCol++)
        {
            if(board[boardRow][boardCol] == PIECE_EMPTY)
            {
                emptyCellCount++;
            }
            else
            {
                occupiedCellCount++;
            }
        }
    }

    for(int directionIndex = 0; directionIndex < 8; directionIndex++)
    {
        if(canFlipInDirection(ComputerPlayer, row, col, directionIndex))
        {
            int nextRow = row + DIR_ROW[directionIndex];
            int nextCol = col + DIR_COL[directionIndex];
            while(board[nextRow][nextCol] != ComputerPlayer)
            {
                flippedPieceCount++;
                nextRow += DIR_ROW[directionIndex];
                nextCol += DIR_COL[directionIndex];
            }
        }
    }

    int dynamicScore = POSITION_WEIGHTS[row][col];
    if(emptyCellCount <= 20)
    {
        dynamicScore += occupiedCellCount + flippedPieceCount * 3;
    }
    else if(emptyCellCount <= 44)
    {
        dynamicScore += flippedPieceCount * 2;
    }

    if((row == 0 || row == BOARD_SIZE - 1) &&
       (col == 0 || col == BOARD_SIZE - 1))
    {
        dynamicScore += 100;
    }

    return dynamicScore;
}

void placePieceAndFlip(int playerColour, int row, int col)
{
    for(int i = 0; i < 8; i++)
    {
        if(canFlipInDirection(playerColour, row, col, i))
        {
            flipPieceInDirection(playerColour, row, col, i);
        }
    }
    board[row][col] = playerColour;
}

void flipPieceInDirection(int playerColour, int row, int col, int directionIndex)
{
    int CurrentRow = row + DIR_ROW[directionIndex];
    int CurrentCol = col + DIR_COL[directionIndex];
    while(board[CurrentRow][CurrentCol] != playerColour)
    {
        board[CurrentRow][CurrentCol] = playerColour;
        CurrentRow += DIR_ROW[directionIndex];
        CurrentCol += DIR_COL[directionIndex];
    }
}

void announceWinner()
{
    int BlackCount = 0;
    int WhiteCount = 0;

    for(int i = 0 ; i < BOARD_SIZE; i++)
    {
        for(int j = 0 ; j < BOARD_SIZE; j++)
        {
            if(board[i][j] == PIECE_BLACK)
            {
                BlackCount++;
            }
            else if(board[i][j] == PIECE_WHITE)
            {
                WhiteCount++;
            }
        }
    }
    cout << "Black ($):" << BlackCount << ", White(*):" << WhiteCount << endl;
    if(BlackCount > WhiteCount)
    {
        cout << "Black($) wins!" << endl;
    }
    else if(WhiteCount > BlackCount)
    {
        cout << "White(*) wins!" << endl;
    }
    else
    {
        cout << "It's a draw!" << endl;
    }
}
