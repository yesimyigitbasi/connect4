#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Game Board Size
const int ROW = 6;
const int COL = 7;

// Colors
#define RESET "\033[0m"
#define RED "\033[31m"
#define BLUE "\033[34m"

bool colFull(const vector<vector<char>>&, int) ;
int getRow(vector<vector<char>>, int);
void printBoard(const vector<vector<char>>&);
void childMaker(vector<vector<char>>, vector<pair<int, vector<vector<char>>>> &, char);
string checkWinner(const vector<vector<char>>&);
bool checkWin(const vector<vector<char>>);
double heuristic(vector<vector<char>>);
int minimax(pair<int, vector<vector<char>>>, bool, int, int, int);
bool compareChildren(const pair<int, vector<vector<char>>> &, const pair<int, vector<vector<char>>> &);
void childValue(vector<pair<int, vector<vector<char>>>> &);
bool sameValue(vector<pair<int, vector<vector<char>>>>);

// function to check if the inputted column is full
// Time complexity: O(n) -> n=ROW
// Space complexity: O(1) -> board is passed by reference
bool colFull(const vector<vector<char>>& board, int col) {
    int count = 0;
    for (int i = 0; i < ROW; ++i) {
        if (board[i][col] != '-')
            count++;
    }
    return count == ROW; // return true if the column is full
}

// function to get the first available row of the inputted column
// Time complexity: O(n) -> n=ROW
// Space complexity: O(1) -> size of row is fixed (we can say O(ROW) too)
int getRow(vector<vector<char>> board, int col) {
    int row;
    for (int i = ROW-1; i >= 0; i--) { // loop through the column from bottom to top
        if (board[i][col] == '-') { // check if the row is empty
            row = i;
            break;
        }
    }
    return row;
}

// Print the game board
// Time complexity: O(n^2) -> ROW x COL
// Space complexity: O(1) -> board is passed by reference
void printBoard(const vector<vector<char>>& board) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == 'O') 
                cout << RED << board[i][j] << RESET << " ";  // Red for 'O'
            else if (board[i][j] == '0') 
                cout << BLUE << board[i][j] << RESET << " ";  // Blue for '0'
            else
                cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// Make children of the current board and put them in the children vector
// Time complexity: O(n) -> n=COL
// Space complexity: O(n) -> n=COL (we can say O(1) considering COL is fixed)
void childMaker(vector<vector<char>> board, vector<pair<int, vector<vector<char>>>> &children, char player) {
    if (player == '0') { // if the player is the AI
        for (int i = 0; i < COL; i++) { // loop through the columns
            vector<vector<char>> childBoard = board;
            if (!colFull(board,i)) {
                childBoard[getRow(board,i)][i] = '0'; // update the board
                children.push_back({0,childBoard}); // add the child to the children vector and assign a dummy value
            }
        }
    } 
    else { // if the player is the user
        for(int i = 0; i < COL; i++){
            vector<vector<char>> childBoard = board;
            if (!colFull(board,i)) {
                childBoard[getRow(board,i)][i] = 'O'; // update the board
                children.push_back({0,childBoard}); // add the child to the children vector and assign a dummy value
            }
        }
    }
}

// Function to check who is the winner (O -> user, 0 -> AI)
// Time complexity: O(n^2) -> ROW x COL
// Space complexity: O(1) -> board is passed by reference
string checkWinner(const vector<vector<char>>& board) {
    // Check horizontal
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL - 3; j++) {
            if (board[i][j] == 'O' && board[i][j + 1] == 'O' && board[i][j + 2] == 'O' && board[i][j + 3] == 'O')
                return "O";
            if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0') 
                return "0";
        }
    }
    // Check vertical
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == 'O' && board[i + 1][j] == 'O' && board[i + 2][j] == 'O' && board[i + 3][j] == 'O') 
                return "O";
            if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0') 
                return "0";
        }
    }
    // Check ascending diagonal
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = 0; j < COL - 3; j++) {
            if (board[i][j] == 'O' && board[i + 1][j + 1] == 'O' && board[i + 2][j + 2] == 'O' && board[i + 3][j + 3] == 'O') 
                return "O";
            if (board[i][j] =='0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0') 
                return "0";
        }
    }
    // Check descending diagonal
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = COL - 1; j > 2; j--) {
            if (board[i][j] == 'O' && board[i + 1][j - 1] =='O' && board[i + 2][j - 2] =='O' && board[i + 3][j - 3] == 'O') 
                return "O";
            if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0') 
                return "0";
        }
    }
    // Check for a tie
    bool tie = true;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == '-') {
                tie = false;
                break;
            }
        }
        if (!tie)
            break;
    }
    if (tie)
        return "tie";
    return "";
}

// Function to check if anyone won
// Time complexity: O(n^2) -> ROW x COL (checkWinner function is called)
// Space complexity: O(1)
bool checkWin(const vector<vector<char>> board) {
    bool flag;
    if (checkWinner(board) == "O" || checkWinner(board) == "0" || checkWinner(board) == "tie")
        flag = true; // there is a winner
    else
        flag = false; // game isnt over yet
    return flag;
}

// the maximazing player is the letter S (user) and the minimizing player is the letter O (AI)
// evaluates the board depending on the consecutive pieces of each player
// time complexity: O(n^2) -> ROW x COL
// space complexity: O(n^2) -> ROW x COL (we can also say O(1) considering ROW and COL are fixed, the size of the board is fixed)
double heuristic(vector<vector<char>> board){
    double score = 0;
    // Check horizontal
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL - 3; j++) {
            if (board[i][j] == 'O' && board[i][j + 1] == 'O' && board[i][j + 2] == 'O' && board[i][j + 3] == 'O') {
                score += 100;
            }
            if (board[i][j] == '0' && board[i][j + 1] == '0' && board[i][j + 2] == '0' && board[i][j + 3] == '0') {
                score -= 100;
            }
        }
    }
    // Check vertical
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = 0; j < COL; j++) {
            if (board[i][j] == 'O' && board[i + 1][j] == 'O' && board[i + 2][j] == 'O' && board[i + 3][j] == 'O') {
                score += 100;
            }
            if (board[i][j] == '0' && board[i + 1][j] == '0' && board[i + 2][j] == '0' && board[i + 3][j] == '0') {
                score -= 100;
            }
        }
    }
    // Check ascending diagonal
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = 0; j < COL - 3; j++) {
            if (board[i][j] == 'O' && board[i + 1][j + 1] == 'O' && board[i + 2][j + 2] == 'O' && board[i + 3][j + 3] == 'O') {
                score += 100;
            }
            if (board[i][j] =='0' && board[i + 1][j + 1] == '0' && board[i + 2][j + 2] == '0' && board[i + 3][j + 3] == '0') {
                score -= 100;
            }
        }
    }
    // Check descending diagonal
    for (int i = 0; i < ROW - 3; i++) {
        for (int j = COL - 1; j > 2; j--) {
            if (board[i][j] == 'O' && board[i + 1][j - 1] =='O' && board[i + 2][j - 2] =='O' && board[i + 3][j - 3] == 'O') {
                score += 100;
            }
            if (board[i][j] == '0' && board[i + 1][j - 1] == '0' && board[i + 2][j - 2] == '0' && board[i + 3][j - 3] == '0') {
                score -= 100;
            }
        }
    }
    return score;
}

// Minimax algorithm
// time complexity: O(b^d) -> b=branching factor, d=depth (in the worst case, however alpha-beta pruning may reduce it to b^(d/2))
// space complexity: O(d) -> d=depth
int minimax(pair<int, vector<vector<char>>> parent, bool isMaximizing, int alpha, int beta, int depth) {
    if (depth == 0 || checkWin(parent.second)) // if the depth is 0 or the game is over
        return heuristic(parent.second); // return the heuristic value of the board
    if (isMaximizing) { // if the player is the user
        int maxEval = INT_MIN;
        vector<pair<int, vector<vector<char>>>> children; 
        childMaker(parent.second, children, 'O'); // make children of the current board
        for (auto child : children) { // loop through the children
            int eval = minimax(child, false, alpha, beta, depth - 1); // get the value of the child
            maxEval = max(maxEval, eval); // update the maxEval
            alpha = max(alpha, eval); // update alpha
            if (beta <= alpha) {
                break; //prune
            }
        }
        return maxEval; 
    }
    else { // if the player is minimizing (AI) 
        int minEval = INT_MAX;
        vector<pair<int, vector<vector<char>>>> children;
        childMaker(parent.second, children, '0'); // make children of the current board
        for (auto child : children) { // loop through the children
            int eval = minimax(child, true, alpha, beta, depth - 1); // get the value of the child
            minEval = min(minEval, eval); // update the minEval
            beta = min(beta, eval); // update beta
            if (beta <= alpha)
                break; // prune
        }
        return minEval;
    }
}

// Assign values to the children
// Time complexity: O(n*b^d) -> n=number of children, b=branching factor, d=depth (n * time complexity of minimax)
// Space complexity: O(d) -> d=depth (space complexity of minimax)
void childValue(vector<pair<int, vector<vector<char>>>> &children) {
    for (auto &child : children) { // loop through the children
        child.first = minimax(child, true, INT_MIN, INT_MAX, 7); // assign the value to the child
    }
    sort(children.begin(), children.end()); // sort the children, time complexity: O(nlogn)
}

// check if all children have the same value
// time complexity: O(n)
// space complexity: O(1) -> considering children have a fixed size
bool sameValue(vector<pair<int, vector<vector<char>>>> children) {
    bool flag = true;
    for (int i = 0; i < children.size() - 1; i++) { // loop through the children
        if (children[i].first != children[i + 1].first) { // if the values are not the same
            flag = false;
            break;
        }
    }
    return flag;
}
