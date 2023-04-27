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

using namespace std;

int gridSize;

//prints the board to the user
void printBoard(char* inc) {

    //converts the 1d array to a 2d one
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

        //makes sure that the grid size isnt too big or too small for play
        if(gridSize > 10 || gridSize < 3){
            cout << "Grid size Must be between 3 and 10!\n";
            cout << "Grid size was too big! Please choose a better size! \n";
            exit(0);
        }
        break;
    }
    
}
bool turn(char* inc){

    //converts the 1d array to a 2d one
    char board[gridSize][gridSize];
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            board[i][j] = inc[i * gridSize + j];
        }
     }

    while(1){
        int row, col;
        int diagconter1 = 0;
        int diagconter2 = 0;

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

        //places the piece downm
        board[row][col] = inc[100];

        //converts the 2d array back into the 1d array
        for (int i = 0; i < gridSize; i++){
            for (int j = 0; j < gridSize; j++){
                inc[i * gridSize + j] = board[i][j];
            }
        }
        for (int i = 0; i < gridSize; ++i) {
            int rowCounter = 0;
            int colCounter = 0;

            //checks for diagonals from upper left to lower right
            if (board[i][i] == inc[100]){diagconter1++;}
            if (board[i][gridSize-1-i] == inc[100]){diagconter2++;}

            for (int j = 0; j < gridSize; ++j) {
                //checks rows
                if(board[i][j] == inc[100]){rowCounter++;}

                //checks columns
                if(board[j][i] == inc[100]){colCounter++;}
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

    //gets the grid size
    getGridSize(argc, argv);

    //initialize the turns 
    int currTurn = 0;
    int totalTurns = gridSize * gridSize;

    //creates the shared memory name and saves the size of it
    const char* name = "/my_shared_memory";
    const int SIZE = sizeof(int)*10;

    //maps out a location for the memory and returns error if its not able to do so
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
        perror("shm_open");
        exit(1);
    }
    if(ftruncate(fd, SIZE) == -1){
        perror("ftruncate");
        exit(1);    
    }

    //creates shared memory obj and cast it to the map variable
    void  *sharedArr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    char* map = static_cast<char*>(sharedArr);

    //initializes the board with dash pieces so you know whats open
    for(int i=0; i< totalTurns;i++){
        map[i] = '-';
    }

    //saves current player and winner
    map[100] = 'X';
    map[101] = '0';

    //creates the two processes
    int pid = fork();
        if(pid == 0){

            while (1){
                //if it looped before the parent sleep timer has ended wait here until it is over
                if(map[100] == 'X'){
                    while(1){ 
                        //when this process continues again it will need to break out of this inner loop
                        if ( map[100] == 'O' ) { 
                            break; 
                        } 
                    }
                }

                //player1 won print out it lost
                if(map[101] == '1'){
                    cout << "Player2: Child: O has Lost!\n";
                    cout << "Will print the final game momentarily!\n";
                    break;
                }

                //let the player know that they only have 15 seconds for their whole turn then print the board and have them go their turn
                cout << "Player 2 has 15 seconds to play their turn or they forfeit their turn" << endl;
                cout << "current player: Child: " << map[100] << "\n";
                printBoard(map);
                bool won = turn(map);

                //check if the player has won. if so then print it and update that it won in the map
                if(won){
                    map[101] = '2';
                    cout << "Player2: child: O has won!\n";
                    cout << "Will print the final game momentarily!\n";
                }

                //update the current player
                map[100] = 'X';
            }    
        }else{
            //stop the child process before it does anything
            kill(pid, SIGSTOP);
            do{
                //print the current player and current board then the player takes their turn
                cout << "current player: parent: "<< map[100] << "\n";
                printBoard(map);
                bool won = turn(map);     

                //update current player and current turn 
                map[100] = 'O';
                currTurn++;

                //checks if the player has one. if so print it out and update that they have won
                if(won){
                    cout << "Player1: Parent: X has Won!\n";
                    map[101] = '1';
                }

                //starts the child process, gives them 15 seconds for their turn updates the currernt turn and stops the child process
                if(currTurn < totalTurns){
                    kill(pid, SIGCONT);
                    sleep(15);
                    currTurn++;
                    kill(pid, SIGSTOP);

                    //check if player1 or player 2 has won and breaks out of the loop
                    if(map[101] == '2'){
                        cout << "Player1: Parent: X has Lost!\n";
                        break;
                    }else if(map[101] == '1'){
                        break;
                    }
                }
            }while(currTurn < totalTurns);

            //prints the tie kills the child process and prints the final board
            if(currTurn == totalTurns){
                cout << "No Winner: TIE!  \n";
            }
            kill(pid, SIGKILL);
            printBoard(map);
        }

        //unmaps the memory location and closes it for further uses
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