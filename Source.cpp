#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

char playerOne = 'X';
char playerTwo = 'O';
bool winner = false;

int gridSize;
vector<vector<char>> board;

//prints the board to the user
void printBoard() {
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            cout << "[" << board [i][j] << "]";
        }
        cout << endl;
    }
}

//create a dynamically size tic-tac-toe board using vectors
void createBoard(){
    vector<char> temp;

    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; ++j) {
            temp.push_back({'0'});
        }
        board.push_back(temp);
    }
}
//checks the rows columns and diagonals for a winner and
//changes the winner boolean variable accordingly
void checkWinner(){

    int diagconter1 = 0;
    int diagconter2 = 0;

    for (int i = 0; i < gridSize; ++i) {
        int rowCounter = 0;
        int colCounter = 0;

        //checks for diagonals from upper left to lower right
        if (board[i][i] == playerOne){diagconter1++;}
        if (board[i][gridSize-1-i] == playerOne){diagconter2++;}

        for (int j = 0; j < gridSize; ++j) {
            //checks rows
            if(board[i][j] == playerOne){rowCounter++;}

            //checks columns
            if(board[j][i] == playerOne){colCounter++;}
        }
        //checks if all the counters are equal to the grid size
        //to find the winner
        if(rowCounter == gridSize
        || colCounter == gridSize
        || diagconter1 == gridSize
        || diagconter2 == gridSize){winner = true;}
    }

}
//gets the grid size from command line savees the grid size to a variable and creates the board from it
void getGridSize(int argc, char **argv) {
    //alerts the user that they did not input a gridsize
    if(argc < 2){
        cout << "Grid size was not specified. Please try again with a grid size like 3 for example." << endl;
        exit(0);
    }

    //checks if the user inputted num can be converted to an integer
    //if it can then its saved into a variable
    try{
        gridSize = stoi(argv[1]);

        //if it cant be converted it catches the error and alerts the user that it needs to be in decimal format
    }catch (exception &err){
        cout << "Please enter the grid size in decimal format and try again." << endl;
        exit(0);
    }
    createBoard();
}
int main(int argc, char * argv[]) {

    getGridSize(argc, argv);
    printBoard();
}