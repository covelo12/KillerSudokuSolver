#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void fillDiagonals(int board[9][9]);
void shuffle(int* array, size_t n);
bool isSafe(int board[9][9], int row, int col, int num);
bool solveSudoku(int board[9][9]);
void removeElements(int board[9][9], int holes);
void printBoard(int board[9][9], int cages[9][9]);
void createCages(int board[9][9],int cages[9][9],int sumCage[81]);
void printCages(int cages[9][9], int sumCage[81]);
void updateCageSums(int board[9][9], int cages[9][9], int newSumCages[81]);
bool solveSudokuKiller(int board[9][9], int cages[9][9], int sumCages[81], int newSumCages[81]);

const char* colors[] = {
    "\033[0;31m", // Red
    "\033[0;32m", // Green
    "\033[0;33m", // Yellow
    "\033[0;34m", // Blue
    "\033[0;35m", // Magenta
    "\033[0;36m", // Cyan
    "\033[0;37m", // White
};

int main(void) {
    srand(time(NULL));  // Seed random number generator

    int board[9][9] = {0};  // Initialize the board with zeros

    // Fill diagonals with random 3x3 blocks
    fillDiagonals(board);

    // Use backtracking to fill the entire board
    solveSudoku(board);

    int cages[9][9] = {0};
    int sumCage[81] = {0};

    // Create Cages
    createCages(board, cages, sumCage);
    printf("\nCAGES FOR YOUR GAME!\n");
    printCages(cages, sumCage);

    // Remove elements to create a puzzle with 'holes' number of empty cells
    removeElements(board, 40);  // E.g., remove 40 elements

    // Print the Sudoku puzzle
    printf("\nKILLER SUDOKU SOLVE IT!\n");
    printBoard(board, cages);

    int updatedSumCage[81] = {0};
    updateCageSums(board, cages, updatedSumCage);

    solveSudokuKiller(board, cages, sumCage, updatedSumCage);

    // Print the Sudoku puzzle Solved
    printf("\nKILLER SUDOKU SOLVED!\n");
    printBoard(board, cages);

    return 0;
}

//Validates if cage can be created
bool isValidCell(int cageGrid[9][9], int row, int col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9 && cageGrid[row][col] == 0;
}

void createCages(int board[9][9],int cages[9][9],int sumCage[81]) {
        int currentCage = 1;

        for (int i = 0; i< 9; i++){
                for (int j = 0; j < 9; j++){
                        if (cages[i][j] == 0){

                                size_t cageSize = rand() % 4 + 1; // cage btw 4 and 1
                                int row = i;
                                int  col = j;
                                for ( int s = 0; s < cageSize; s++){
                                        cages[row][col] = currentCage;
                                        sumCage[currentCage-1] += board[row][col];

                                        int direction = rand() % 2; //direction 0 right , 1 down
                                        // Validates possibility of going down or left
                                        bool canGoDown = isValidCell(cages, row+1, col);
                                        bool canGoRight = isValidCell(cages, row, col+1);
                                        //if both valid chooses at random else goes to the only possible
                                        if(canGoDown && canGoRight){
                                                if (direction == 0)
                                                        row++;
                                                else
                                                        col++;
                                        }
                                        else if(canGoDown){
                                                row++;
                                        }
                                        else if(canGoRight){
                                                col++;
                                        }
                                        else
                                                break;
                                }
                                currentCage++;
                        }

                }
        }

}
void fillDiagonals(int board[9][9]) {
    int sample[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (int i = 0; i <= 6; i += 3) {
        shuffle(sample, 9);  // Shuffle the array to randomize values
        int pointer = 0;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                board[i + j][i + k] = sample[pointer++];
            }
        }
    }
}

void shuffle(int* array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

bool isSafe(int board[9][9], int row, int col, int num) {
    // Check the row
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;

    // Check the column
    for (int x = 0; x < 9; x++)
        if (board[x][col] == num)
            return false;

    // Check the 3x3 subgrid
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool findEmptyCell(int board[9][9], int *row, int *col) {
    for (*row = 0; *row < 9; (*row)++) {
        for (*col = 0; *col < 9; (*col)++) {
            if (board[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

bool solveSudoku(int board[9][9]) {
    int row, col;

    // If there is no empty space, we are done
    if (!findEmptyCell(board, &row, &col))
        return true;

    // Try numbers 1-9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            // Recursively try to fill the rest of the board
            if (solveSudoku(board))
                return true;

            // If it leads to no solution, reset the cell and backtrack
            board[row][col] = 0;
        }
    }
    return false;
}

void updateCageSums(int board[9][9], int cages[9][9], int newSumCages[81]) {

    // Calculate the new sums based on the current board state
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int cageNum = cages[i][j];
            if (cageNum > 0 && board[i][j] > 0) { // Only sum up non-zero cells
                newSumCages[cageNum - 1] += board[i][j];
            }
        }
    }
}

bool solveSudokuKiller(int board[9][9], int cages[9][9], int sumCages[81], int newSumCages[81]) {
    int row, col;

    // If there is no empty space, we are done
    if (!findEmptyCell(board, &row, &col))
        return true;

    // Try numbers 1-9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        // Check if the number can be placed
        if (isSafe(board, row, col, num) &&
            (newSumCages[cages[row][col]-1] + num <= sumCages[cages[row][col]-1])) {

            board[row][col] = num;  // Place the number
            newSumCages[cages[row][col]-1] += num;  // Update the cage sum

            // Recursively try to fill the rest of the board
            if (solveSudokuKiller(board, cages, sumCages, newSumCages))
                return true;

            // If it leads to no solution, reset the cell and backtrack
            newSumCages[cages[row][col]-1] -= num;
            board[row][col] = 0;
        }
    }
    return false;  // No valid number found, trigger backtrack
}

void removeElements(int board[9][9], int holes) {
    // Randomly remove 'holes' number of elements from the board
    for (int i = 0; i < holes; i++) {
        int row = rand() % 9;
        int col = rand() % 9;

        // Ensure we're removing an existing number, not an already empty cell
        while (board[row][col] == 0) {
            row = rand() % 9;
            col = rand() % 9;
        }

        board[row][col] = 0;  // Remove the number by setting it to 0
    }
}

void printBoard(int board[9][9], int cages[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int cageNum = cages[i][j];
            int color = cageNum > 0 ? (cageNum % 7) : 0;  // Get color for cage or default to 0 if it's empty
            if (board[i][j] == 0) {
                printf("%s .%s ", "\033[0m", "\033[0m"); // Print empty cell as dot
            } else {
                // Print the number with its cage color
                printf("%s%2d%s ", colors[color], board[i][j], "\033[0m");
            }
        }
        printf("\n");
    }
}


void printCages(int cages[9][9], int sumCages[81]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
                int cageNum = cages[i][j];
                int color = cageNum % 7;
                printf("%s%3d%s ", colors[color], sumCages[cages[i][j]-1], "\033[0m");
        }
        printf("\n");
    }
}