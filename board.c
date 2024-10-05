#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void fillDiagonals(int board[9][9]);
void shuffle(int* array, size_t n);
bool isSafe(int board[9][9], int row, int col, int num);
bool solveSudoku(int board[9][9]);
void removeElements(int board[9][9], int holes);
void printBoard(int board[9][9]);

int main(void) {
    srand(time(NULL));  // Seed random number generator

    int board[9][9] = {0};  // Initialize the board with zeros

    // Fill diagonals with random 3x3 blocks
    fillDiagonals(board);

    // Use backtracking to fill the entire board
    solveSudoku(board);

    // Remove elements to create a puzzle with 'holes' number of empty cells
    removeElements(board, 40);  // E.g., remove 40 elements

    // Print the Sudoku puzzle
    printBoard(board);

    return 0;
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

void printBoard(int board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0)
                printf(". ");
            else
                printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}
