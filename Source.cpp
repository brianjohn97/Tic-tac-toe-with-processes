#include <iostream>
#include <unistd.h>

using namespace std;
int gridSize = 5;
char board[5][5] = {{'-','-','-'},{'-','-','-'},{'-','-','-'},{'-','-','-'},{'-','-','-'}};
char board2[3][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'}};
string playerOne = "X";
string playerTwo = "O";
string winner;

void printBoard(){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            cout << "[" << board [i][j] << "]";
        }
        cout << endl;
    }
    
}
void checkWinner(){

}

int main(int argc, char * argv[]) {
    /*
    if(argc < 2){
        cout << "Grid size was not specified. Please try again with a grid size like 3 for example." << endl;
        exit(0);
    }
    try{
        gridSize = stoi(argv[1]);
    }catch (exception &err){
        cout << "Please enter the grid size in decimal format and try again." << endl;
        exit(0);
    }*/
    printBoard();
}