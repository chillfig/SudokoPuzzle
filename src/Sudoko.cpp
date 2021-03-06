#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include "Helper.h"
#include "Sudoko.h"
using namespace std;
using namespace std::chrono;

int main()
{
    string choice;                                            // "keyboard" indicates keyboard-entered, "file" indicates it's from file
    string saveToFile;                                        // "save" indicates user wants to save puzzle
    int difficulty;                                           // "easy", "mid", or "hard"
    enum difficultyEnum{EASY = 15, MEDIUM = 30, HARD = 45};
    cout << "Justin's Sudoku Puzzle Program" << "\n\n";
    while (true)
    {
        cout << "Enter \"keyboard\" to input a Sudoko puzzle from the keyboard" << "\n"
                "Enter \"file\" to input a Sudoko puzzle from a file" << "\n"
                "Enter \"create\" to create a Sudoko puzzle" << "\n"
                "Enter \"exit\" to exit the program" << "\n\n";

        cin >> choice;
        if(choice == "keyboard" || choice == "file"){
            puzzle newPuzzle(choice);                       // create a puzzle object from input
            puzzle *ptr = &newPuzzle;                       // create pointer to object   
            
            // capture puzzle from user
            if (choice == "keyboard"){
                newPuzzle.acceptInput(ptr->numMatrix);
            }
            else if(choice == "file"){
                ptr->fileStat = newPuzzle.fileParse(ptr->numMatrix);
            }
            /*USE THIS TO TEST RUNTIME // Get starting timepoint
            auto start = high_resolution_clock::now();
        
            newPuzzle.solvePuzzle(ptr->numMatrix, 0, 0);
            cout << "\nThe solved puzzle is:\n";
            newPuzzle.printPuzzle(ptr->numMatrix);
        
            // Get ending timepoint
            auto stop = high_resolution_clock::now();
        
            // Get duration. Substart timepoints to 
            // get durarion. To cast it to proper unit
            // use duration cast method
            auto duration = duration_cast<microseconds>(stop - start);
        
            cout << "Time taken by function: "
                << duration.count() << " microseconds" << endl;
            break;
            // Attempt to solve the valid puzzle
            if (newPuzzle.solvePuzzle(ptr->numMatrix, 0, 0)){
                cout << "\nThe solved puzzle is:\n";
                newPuzzle.printPuzzle(ptr->numMatrix);
            }else{
                cout << "The given puzzle can't be solved\n\n";
            }*/
            // Communicate validity of puzzle to user 
            if (ptr->fileStat == 0){
                ptr->validity = newPuzzle.validatePuzzle(ptr->numMatrix);
                switch (ptr->validity){
                case 1:
                    // First, print the valid puzzle to the screen
                    newPuzzle.printPuzzle(ptr->numMatrix);
                    // If this is a keyboard-entered puzzle, then give option to save
                    if (choice == "keyboard"){
                        cout << "\nEnter \"save\" to save your puzzle to a file. "
                        "Otherwise, enter anything else to continue:\n";
                        cin >> saveToFile;
                        cout << "\n";
                        if (saveToFile == "save"){
                            newPuzzle.fileSave(ptr->numMatrix);
                        }
                    }
                    // Attempt to solve the valid puzzle
                    if (newPuzzle.solvePuzzle(ptr->numMatrix, 0, 0)){
                        cout << "\nThe solved puzzle is:\n";
                        newPuzzle.printPuzzle(ptr->numMatrix);
                    }else{
                        cout << "The given puzzle can't be solved\n\n";
                    }
                    break;
                case 0:
                    cout << "The given puzzle contains repeated values\n\n";
                    break;
                case -1:
                    cout << "The given puzzle contains an invalid value\n\n";
                    break;    
                default:
                    cout << "Error\n";
                    break;
                }
            }
        } else if (choice == "create"){
            puzzle newPuzzle(choice);                       // create a puzzle object from input
            puzzle *ptr = &newPuzzle;                       // create pointer to object   
            srand((unsigned) time(0));
            int randomNumber1;
            int randomNumber2;
            vector<int>usedRandomNums;

            // Set all Sudoko Puzzle values to 0
            for (int i = 0; i < puzzleDim; i++){
                for (int j = 0; j < puzzleDim; j++){
                    (ptr->numMatrix)[i][j] = 0;
                }
            }
            // Fill the 3 diagonal 3x3 boxes of the puzzle randomly
            for(int iBox = 0; iBox < puzzleDim; iBox += 3){
                for(int jBox = 0; jBox < puzzleDim; jBox += 3){
                    if (iBox == jBox){
                        for(int i = iBox; i < iBox + 3; i++){
                            for(int j = jBox; j < jBox + 3; j++){
                                randomNumber1 = (rand() % 9) + 1;
                                while(contains(usedRandomNums,randomNumber1)){
                                    randomNumber1 = (rand() % 9) + 1;
                                }
                                (ptr->numMatrix)[i][j] = randomNumber1;
                                usedRandomNums.push_back(randomNumber1);
                            }
                        }
                    }
                    usedRandomNums.clear();
                }
            }
            newPuzzle.printPuzzle(ptr->numMatrix);

            // Fill in the rest of the puzzle recursively
            newPuzzle.solvePuzzle(ptr->numMatrix, 0, 3);

            // Print out the solved puzzle
            cout << "\nThe solved puzzle is:" << endl;
            newPuzzle.printPuzzle(ptr->numMatrix);

            // Remove number of elements according to user's difficulty preference
            // CHANGE TO 1: EASY, 2: MEDIUM, 3: HARD. IN OTHER WORDS difficulty == 1,2,3
            cout << "\nSelect the difficulty of your Sudoko Puzzle by number:" << endl
                 << "15: EASY" << endl
                 << "30: MEDIUM" << endl
                 << "45: HARD\n" << endl;
            // Obtain user's difficulty preference
            while(true){
                cin >> difficulty;
                if (difficulty == EASY || difficulty == MEDIUM || difficulty == HARD){
                    break;
                }
                cout << "\nInvalid input. Try again" << endl;
            }
            // Change puzzle according to difficulty preference
            while(difficulty){
                randomNumber1 = rand() % 9;
                randomNumber2 = rand() % 9;
                if ((ptr->numMatrix)[randomNumber1][randomNumber2] != 0){
                    (ptr->numMatrix)[randomNumber1][randomNumber2] = 0;
                    difficulty -= 1;
                }
            }
            // Print the unsovled puzzle
            cout << "\nThe unsolved puzzle is: " << endl;
            newPuzzle.printPuzzle(ptr->numMatrix);

            // Save the unsolved puzzle
            cout << "\n";
            newPuzzle.fileSave(ptr->numMatrix);

        } else if (choice == "exit"){
            cout << "Exiting...\n";
            break;
        } else{
            cout << "Invalid input. Try again\n\n";
            continue;
        }
    }
    return 0;
}