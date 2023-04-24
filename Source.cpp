#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h> 
#include <sys/mman.h>
#include <signal.h>


using namespace std;

char playerOne = 'X';
char playerTwo = 'O';
bool winner = false;

char currPlayer = playerOne;

int gridSize = 3;


//prints the board to the user
void printBoard(vector<vector<char>> board) {
    for (int i = 0; i < gridSize; i++){
            if(i == 0){
                cout << "   " <<  i << "  "; 
                continue;   
            }
            cout <<  i << "  ";
        }
        cout << endl;

    for (int i = 0; i < gridSize; i++){
        cout << i << " " ;
        for (int j = 0; j < gridSize; j++){
           
            cout << "[" << board [i][j] << "]";
        }
        cout << endl;
    }
    cout << endl;
}
//checks the rows columns and diagonals for a winner and
//changes the winner boolean variable accordingly
void checkWinner(vector<vector<char>> board){

    int diagconter1 = 0;
    int diagconter2 = 0;

    for (int i = 0; i < gridSize; ++i) {
        int rowCounter = 0;
        int colCounter = 0;

        //checks for diagonals from upper left to lower right
        if (board[i][i] == currPlayer){diagconter1++;}
        if (board[i][gridSize-1-i] == currPlayer){diagconter2++;}

        for (int j = 0; j < gridSize; ++j) {
            //checks rows
            if(board[i][j] == currPlayer){rowCounter++;}

            //checks columns
            if(board[j][i] == currPlayer){colCounter++;}
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
}
void turn(vector<vector<char>> &board){
    while(1){
        int row, col;

        //get row
        cout << "Enter row: " ;
        cin >> row;
        if(!cin || row  < 0  || row >= gridSize){
            cout << "the inputed row does not exitst please try again.\n";
            continue;
        }

        //get column
        cout << "enter col: ";
        cin >> col;
        if(!cin || row  < 0  || row >= gridSize){
            cout << "the inputed row does not exitst please try again.\n";
            continue;
        }

        //checks if the space is occupied before placing piece down
        if(board[row][col] == playerOne || board[row][col] == playerTwo){
            cout << "that space has already been taking please choose from a free empty space.\n";
            continue;
        }
        board[row][col] = currPlayer;
        checkWinner(board);
        break;
    }
}
void signal(int sig){
    signal(SIGUSR1, signal);
}
int main(int argc, char * argv[]) {

    //getGridSize(argc, argv);

    vector<vector<char>> board;
    vector<char> temp;
    int currTurn = 0;

    

    //create the board
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; ++j) {
            temp.push_back({'-'});
        }
        board.push_back(temp);
    }
    pid_t pid;
    int loop = gridSize * gridSize;
    pid = vfork();

    //for (int i = 1; i <= loop; i++){
        if(pid > 0){
            waitpid(pid, NULL, WUNTRACED);
            cout << "i am the parent,  current player: "<< currPlayer << "\n";
            printBoard(board);
            turn(board);     

            currPlayer = playerOne;
        
            cout << "end parent\n";
            currTurn++;
            
        }else{
                cout << "begin child, current player: " << currPlayer << "\n";
                printBoard(board);
                turn(board);

                cout << "\ni am done in the child\n";
                currPlayer = playerTwo;
                currTurn++;

                exit(0);
        }
    
}