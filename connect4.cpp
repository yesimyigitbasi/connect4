#include <iostream>
#include "connect4.h"

using namespace std;

int main() {
    // Initialize game board
    vector<vector<char>> board(ROW, vector<char>(COL, '-'));

    // Initialize variables
    int row = 0;
    int currentPlayer = 1;
    bool win = false;
    bool isTie = false;
    string winner;
    while (!win && !isTie) {
        if (currentPlayer == 1) { // player's turn
            cout << "Player RED O make your move (1-7): "; //if current player is 1, print red, else print blue
            int move; // move = the column the player wants to place their piece
            cin >> move;
            move--;

            if (move < 0 || move > 6) { // check if the move is valid
                cout << "Invalid move." << endl;
                continue;
            }
            if (colFull(board, move)) { // check if the column is full
                cout << "Column " << move + 1 << " is full" << endl;
                continue;
            }
            
            board[getRow(board,move)][move] = 'O'; // update the board
            printBoard(board);
        }
        else { // AI's turn
            vector<pair<int, vector<vector<char>>>> children;
            childMaker(board, children, '0');
            childValue(children);
            if(sameValue(children)) { //if there are multiple children with the same value
                int random = rand() % children.size(); //choose random child
                board = children[random].second;
            }
            else
                board = children[0].second;

            cout << "AI MOVE" << endl;
            printBoard(board);
        }

        winner = checkWinner(board); // check who won
        win = checkWin(board); // check if there is a winner/if the game is over

        if (winner == "tie") {
            isTie = true;
        }

        // Switch the current player
        currentPlayer = (currentPlayer == 1 ? 2 : 1); // if current player is 1, switch to 2, else switch to 1  
    }

    if (win && !isTie)
        cout << "Player " << winner << " wins!" << endl;
    else if (isTie)
        cout << "It's a tie!" << endl;
    printBoard(board);

    return 0;
}
