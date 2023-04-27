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
#include <limits>
#include <sched.h>
#include <linux/sched.h>
#include <sys/syscall.h>
#include <memory>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#define MAX_ELEMENTS 3

using namespace std;

char playerOne = 'X';
char playerTwo = 'O';
bool winner = false;

char currPlayer = playerOne;

int gridSize;


//prints the board to the user
void printBoard(char* inc) {

    char board[gridSize][gridSize];
     for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            board[i][j] = inc[i * gridSize + j];
        }
     }
     


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
//gets the grid size from command line savees the grid size to a variable and creates the board from it
void getGridSize(int argc, char **argv) {
    
    while (1){
        //alerts the user that they did not input a gridsize
        if(argc < 2){
            cout << "Grid size Must be between 3 and 10!\n";
            cout << "Grid size was not specified. Please try again. " << endl;
            exit(0);
        }

        //checks if the user inputted num can be converted to an integer
        //if it can then its saved into a variable
        try{
            gridSize = stoi(argv[1]);

        //if it cant be converted it catches the error and alerts the user that it needs to be in decimal format
        }catch (exception &err){
            cout << "Grid size Must be between 3 and 10!\n";
            cout << "Please enter the grid size as a number in decimal format and try again." << endl;
            exit(0);
        }

        if(gridSize > 10 || gridSize < 3){
            cout << "Grid size Must be between 3 and 10!\n";
            cout << "Grid size was too big! Please choose a better size! \n";
            exit(0);
        }
        break;
    }
    
}
bool turn(char* inc){

    char board[gridSize][gridSize];
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            board[i][j] = inc[i * gridSize + j];
        }
     }

    while(1){
        int row, col;

        //get row
        cout << "Enter row: " ;
        cin >> row;
        if(!cin || row  < 0  || row >= gridSize){
            cout << "the inputed row does not exist please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        //get column
        cout << "enter col: ";
        cin >> col;
        if(!cin || row  < 0  || row >= gridSize){
            cout << "the inputed column does not exist please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cout << endl;

        //checks if the space is occupied before placing piece down
        if(board[row][col] != '-'){
            cout << "that space has already been taking please choose from a free empty space.\n";
            continue;
        }
        board[row][col] = inc[100];

        for (int i = 0; i < gridSize; i++){
            for (int j = 0; j < gridSize; j++){
                inc[i * gridSize + j] = board[i][j];
            }
        }
        
        int diagconter1 = 0;
        int diagconter2 = 0;

        for (int i = 0; i < gridSize; ++i) {
            int rowCounter = 0;
            int colCounter = 0;

            //checks for diagonals from upper left to lower right
            if (board[i][i] == inc[9]){diagconter1++;}
            if (board[i][gridSize-1-i] == inc[9]){diagconter2++;}

            for (int j = 0; j < gridSize; ++j) {
                //checks rows
                if(board[i][j] == inc[9]){rowCounter++;}

                //checks columns
                if(board[j][i] == inc[9]){colCounter++;}
            }
            //checks if all the counters are equal to the grid size
            //to find the winner
            if(rowCounter == gridSize
            || colCounter == gridSize
            || diagconter1 == gridSize
            || diagconter2 == gridSize){return true;}
        }

        
        break;
    }
    return false;
}
int main(int argc, char * argv[]) {

    getGridSize(argc, argv);

    vector<vector<char>> board;
    vector<char> temp;
    int currTurn = 0;
    int totalTurns = gridSize * gridSize;

    const char* name = "/my_shared_memory";
    const int SIZE = sizeof(int)*10;

    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        perror("shm_open");
        exit(1);
    }

    if(ftruncate(fd, SIZE) == -1){
        perror("ftruncate");
        exit(1);    
    }

    void  *sharedArr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char* map = static_cast<char*>(sharedArr);
    for(int i=0; i< totalTurns;i++){
        map[i] = '-';
    }
    map[100] = 'X';
    map[101] = '0';
    int pid = fork();
        if(pid == 0){

            int i = 1;

            while (i == 1){
                
                if(map[100] == 'X'){
                    while(1){ 
                        if ( map[100] == 'O' ) { 
                            break; 
                        } 
                    }
                }
                if(map[101] == '1'){
                    cout << "Player2: Child: O has Lost!\n";
                    break;
                }
                cout << "Player 2 has 15 seconds to play their turn or they forfeit their turn" << endl;
                cout << "current player: Child: " << map[100] << "\n";
                printBoard(map);
                bool won = turn(map);
                if(won){
                    map[101] = '2';
                    cout << "Player2: child: O has won!\n";
                }
                map[100] = 'X';
            }
            
            
        }else{
            
            kill(pid, SIGSTOP);
            do{
                
                cout << "current player: parent: "<< map[100] << "\n";
                printBoard(map);
                bool won = turn(map);     

                map[100] = 'O';
            
                currTurn++;
                if(won){
                    cout << "Player1: Parent: X has Won!\n";
                    map[101] = '1';
                }
                if(currTurn < totalTurns){
                    kill(pid, SIGCONT);
                    sleep(15);
                    currTurn++;
                    kill(pid, SIGSTOP);
                    if(map[101] == '2'){
                        cout << "Player1: Parent: X has Lost!\n";
                        break;
                    }else if(map[101] == '1'){
                        break;
                    }
                }
                
                
            }while(currTurn < totalTurns);

            if(currTurn == totalTurns){
                cout << "No Winner: TIE!  \n";
            }
            kill(pid, SIGKILL);
            printBoard(map);
        }
        if(munmap(sharedArr, SIZE) == -1){
            perror("munmap");
            exit(1);
        }
        if(close(fd) == -1){
            perror("close");
            exit(1);
        }
        return 0;
}